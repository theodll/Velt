#include "vtpch.h"
#include "Core/Core.h"
#include "Core/Input.h"
#include "imgui_impl_vulkan.h"
#include "EditorViewportPanel.h"
#include "Editor/EditorGuizmos.h"
#include "Core/Application.h"
#include "Velt/Platform/Vulkan/VulkanContext.h"


namespace Velt::Editor 
{

	void ViewportPanel::Init(u32 width, u32 height)
	{
		VT_PROFILE_FUNCTION();
		Application::Get()->UsingEditorViewport = true;
		Application::Get()->RenderableWidth = width;
		Application::Get()->RenderableHeight = height;

		m_Width = width;
		m_Height = height;

		CreateResources();
	}

	void ViewportPanel::Shutdown()
	{
		VT_PROFILE_FUNCTION();
	}

	void ViewportPanel::Resize(u32 width, u32 height)
	{
		VT_PROFILE_FUNCTION();
		if (width == m_Width && height == m_Height)
			return;

		m_Width = width;
		m_Height = height;

		Application::Get()->RenderableWidth = width;
		Application::Get()->RenderableHeight = height;

		CreateResources();
	}

	void ViewportPanel::SetRenderTarget(RenderTarget target)
	{
		VT_PROFILE_FUNCTION();
		if (target == m_RenderTarget)
			return;

		m_RenderTarget = target;

		Application::Get()->SelectedRenderTarget = target;
		CreateResources();
	}

	void ViewportPanel::CreateResources()
	{
		VT_PROFILE_FUNCTION();
		Renderer::RecreateRenderTargets(m_Width, m_Height);

		m_Image = Renderer::GetRenderTarget(m_RenderTarget)->GetImage();
		m_ImageView = Renderer::GetRenderTarget(m_RenderTarget)->GetImageView();
		m_Sampler = Renderer::GetRenderTarget(m_RenderTarget)->GetSampler();

		const auto& resourceUploader = RHI::VulkanContext::GetResourceUploader();
		resourceUploader->Begin();

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = m_Image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(
			resourceUploader->GetCommandBuffer(),
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);
		resourceUploader->End();

		CreateDescriptorSet();
	}

	void ViewportPanel::CreateDescriptorSet()
	{
		VT_PROFILE_FUNCTION();

		m_DescriptorSet = ImGui_ImplVulkan_AddTexture(m_Sampler, m_ImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		if (m_DescriptorSet == VK_NULL_HANDLE)
		{
			VT_CORE_ERROR("Failed to create ImGui descriptor set for scene viewport!");
		}
	}



	void ViewportPanel::OnUpdate(Timestep ts)
	{
		VT_PROFILE_FUNCTION();
	}

	void ViewportPanel::OnImGuiRender2()
	{
		VT_PROFILE_FUNCTION();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

		ImGui::Begin("Scene Viewport", nullptr,
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse);

		m_IsHovered = ImGui::IsWindowHovered();

		if (ImGui::IsWindowHovered())
		{
			ImVec2 mousePos = ImGui::GetMousePos();
			ImVec2 windowPos = ImGui::GetWindowPos();

			m_LocalMousePos = ImVec2(
				mousePos.x - windowPos.x,
				mousePos.y - windowPos.y
			);
		}
		
		ImVec2 avail = ImGui::GetContentRegionAvail();

		u32 newW = (u32)glm::max(1.0f, avail.x);
		u32 newH = (u32)glm::max(1.0f, avail.y);

		if (newW != m_Width || newH != m_Height)
		{
			m_PendingWidth = newW;
			m_PendingHeight = newH;
			m_ResizePending = true;
		}

		if (m_DescriptorSet != VK_NULL_HANDLE)
			ImGui::Image((ImTextureID)m_DescriptorSet, avail);
		else
			ImGui::Text("Scene Viewport not initialized");

		if (m_EditorCamera && m_EditorGuizmos)
		{
			m_EditorGuizmos->RenderGizmos(m_EditorCamera);
		}

		ImGui::End();

		ImGui::PopStyleVar();
	}

	void ViewportPanel::ProcessPendingChanges()
	{
		VT_PROFILE_FUNCTION();
		if (m_ResizePending)
		{
			Resize(m_PendingWidth, m_PendingHeight);
			m_ResizePending = false;
		}

		if (RenderTargetChangePending)
		{
			SetRenderTarget(PendingRenderTarget);
			RenderTargetChangePending = false;
		}
	}

}