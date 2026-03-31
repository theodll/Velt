#include "vtpch.h"
#include "SceneViewport.h"
#include "Renderer/Renderer.h"
#include "Velt/Platform/Vulkan/VulkanContext.h"
#include "Velt/Platform/Vulkan/VulkanDevice.h"
#include "imgui_impl_vulkan.h"

namespace Velt {

	SceneViewport::SceneViewport() : m_Width(0), m_Height(0), m_Device(*RHI::VulkanContext::GetDevice())
	{

	}

	void SceneViewport::Init(u32 width, u32 height) 
	{
		VT_PROFILE_FUNCTION();

		m_Width = width;
		m_Height = height;

		CreateResources();
	}

	void SceneViewport::Shutdown()
	{
		VT_PROFILE_FUNCTION();
		CleanupResources();
	}

	void SceneViewport::Resize(u32 width, u32 height)
	{
		VT_PROFILE_FUNCTION();
		if (width == m_Width && height == m_Height)
			return;

		m_Width = width;
		m_Height = height;

		CleanupResources();
		CreateResources();
	}

	void SceneViewport::CreateResources()
	{
		VT_PROFILE_FUNCTION();

		Renderer::RecreateRenderTargets(m_Width, m_Height); 

		m_Image = Renderer::GetRenderTarget(m_RenderTarget)->GetImage();
		m_ImageView = Renderer::GetRenderTarget(m_RenderTarget)->GetImageView();
		m_Sampler = Renderer::GetRenderTarget(m_RenderTarget)->GetSampler();

		m_DepthImage = Renderer::GetRenderTarget(VT_RENDER_TARGET_DEPTH)->GetImage();
		m_DepthImageView = Renderer::GetRenderTarget(VT_RENDER_TARGET_DEPTH)->GetImageView();

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


		VkImageMemoryBarrier depthBarrier{};
		depthBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		depthBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthBarrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		depthBarrier.srcAccessMask = 0;
		depthBarrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		depthBarrier.image = m_DepthImage;
		depthBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		depthBarrier.subresourceRange.baseMipLevel = 0;
		depthBarrier.subresourceRange.levelCount = 1;
		depthBarrier.subresourceRange.baseArrayLayer = 0;
		depthBarrier.subresourceRange.layerCount = 1;

		vkCmdPipelineBarrier(
			resourceUploader->GetCommandBuffer(),
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
			0,
			0, nullptr,
			0, nullptr,
			1, &depthBarrier
		);
		resourceUploader->End();
		CreateDescriptorSet();
	}

	void SceneViewport::CleanupResources()
	{
		VT_PROFILE_FUNCTION();
	}

	void SceneViewport::CreateDescriptorSet()
	{
		VT_PROFILE_FUNCTION();

		m_DescriptorSet = ImGui_ImplVulkan_AddTexture(m_Sampler, m_ImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		if (m_DescriptorSet == VK_NULL_HANDLE)
		{
			VT_CORE_ERROR("Failed to create ImGui descriptor set for scene viewport!");
		}
	}

	void SceneViewport::TransitionForRendering(VkCommandBuffer commandBuffer)
	{
		VT_PROFILE_FUNCTION();

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = m_Image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		vkCmdPipelineBarrier(
			commandBuffer,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);
	}

	void SceneViewport::TransitionForSampling(VkCommandBuffer commandBuffer)
	{
		VT_PROFILE_FUNCTION();

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = m_Image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(
			commandBuffer,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier
		);
	}

	void SceneViewport::SetRenderTarget(RenderTarget target)
	{
		if (target == m_RenderTarget)
			return;

		m_RenderTarget = target;

		CleanupResources();
		CreateResources();
	}

}
