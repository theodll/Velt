#include "vtpch.h"
#include "ImGuiRenderer.h"
#include "Velt/Core/Log.h"
#include <vulkan/vulkan.h>
#include "Core/Application.h"
#include <SDL3/SDL.h>
#include "Platform/Vulkan/VulkanContext.h"

namespace Velt {

	ImGuiRenderer::ImGuiRenderer() {}

	void ImGuiRenderer::Init()
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Init ImGUI Renderer");

		auto&& device = RHI::VulkanContext::GetDevice();
		auto&& window = Application::Get()->GetWindow();

		if (!ImGui_ImplSDL3_InitForVulkan(static_cast<SDL_Window*>(window->GetNativeHandle())))
		{
			VT_CORE_ERROR("Failed to initialize ImGui SDL3 backend!");
		}

		VkDescriptorPoolSize pool_sizes[] = {
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};

		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000;
		pool_info.poolSizeCount = std::size(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;

		if (vkCreateDescriptorPool(device.device(), &pool_info, nullptr, &m_DescriptorPool) != VK_SUCCESS)
		{
			VT_CORE_ERROR("Failed to create ImGui descriptor pool");
		}

		// Setup dynamic rendering format
		VkFormat colorFormat = VK_FORMAT_B8G8R8A8_SRGB;
		VkPipelineRenderingCreateInfoKHR rendering_info = {};
		rendering_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO_KHR;
		rendering_info.colorAttachmentCount = 1;
		rendering_info.pColorAttachmentFormats = &colorFormat;

		// Initialize ImGui for Vulkan
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = RHI::VulkanContext::GetInstance();
		init_info.PhysicalDevice = device.GetPhysicalDevice();
		init_info.Device = device.device();
		init_info.QueueFamily = device.GetQueueFamilyIndex();
		init_info.Queue = device.GetGraphicsQueue();
		init_info.DescriptorPool = m_DescriptorPool;
		init_info.MinImageCount = 3;
		init_info.ImageCount = 3;
		init_info.UseDynamicRendering = true;
		init_info.PipelineInfoMain.PipelineRenderingCreateInfo = rendering_info;

		if (!ImGui_ImplVulkan_Init(&init_info))
		{
			VT_CORE_ERROR("Failed to initialize ImGui Vulkan backend");
		}

		VT_CORE_INFO("ImGuiRenderer initialized");
	}

	void ImGuiRenderer::Shutdown()
	{
		VT_PROFILE_FUNCTION();
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		
		if (m_DescriptorPool != nullptr)
		{
			m_DescriptorPool = nullptr;
		}

		VT_CORE_INFO("ImGuiRenderer shutdown");
	}

	void ImGuiRenderer::Begin()
	{
		ImGui_ImplSDL3_NewFrame();
		ImGui_ImplVulkan_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiRenderer::End()
	{
		ImGui::Render();
	}

	/*
	void ImGuiRenderer::Render(VkCommandBuffer commandBuffer)
	{
		VT_PROFILE_FUNCTION();
		ImDrawData* draw_data = ImGui::GetDrawData();
		if (draw_data != nullptr && draw_data->TotalVtxCount > 0)
		{
			ImGui_ImplVulkan_RenderDrawData(draw_data, commandBuffer, VK_NULL_HANDLE);
		}
	}*/


	void ImGuiRenderer::RenderDrawData(VkCommandBuffer cmd)
	{
		ImDrawData* dd = ImGui::GetDrawData();
		if (dd && dd->TotalVtxCount > 0)
			ImGui_ImplVulkan_RenderDrawData(dd, cmd, VK_NULL_HANDLE);
	}
}