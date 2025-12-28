#include "vtpch.h"
#include "ImGuiRenderer.h"
#include "Velt/Core/Log.h"
#include <vulkan/vulkan.h>
#include <SDL3/SDL.h>

namespace Velt {

	ImGuiRenderer::ImGuiRenderer(Renderer::Vulkan::VulkanSwapchain* swapchain, Renderer::Vulkan::VulkanDevice* device, SDL_Window* window)
		: m_Swapchain(swapchain), m_Device(device), m_Window(window)
	{
		VT_PROFILE_FUNCTION();
	}

	void ImGuiRenderer::Init()
	{
		VT_PROFILE_FUNCTION();

		// Initialize ImGui SDL3 backend first
		if (!ImGui_ImplSDL3_InitForVulkan(m_Window))
		{
			VT_CORE_ERROR("Failed to initialize ImGui SDL3 backend!");
			throw std::runtime_error("Failed to initialize ImGui SDL3 backend!");
		}

		// Create descriptor pool for ImGui
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

		if (vkCreateDescriptorPool(m_Device->device(), &pool_info, nullptr, &m_DescriptorPool) != VK_SUCCESS)
		{
			VT_CORE_ERROR("Failed to create ImGui descriptor pool!");
			throw std::runtime_error("Failed to create ImGui descriptor pool!");
		}

		// Initialize ImGui for Vulkan
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = m_Device->getInstance();
		init_info.PhysicalDevice = m_Device->getPhysicalDevice();
		init_info.Device = m_Device->device();
		init_info.QueueFamily = m_Device->getQueueFamilyIndex();
		init_info.Queue = m_Device->getGraphicsQueue();
		init_info.DescriptorPool = m_DescriptorPool;
		init_info.MinImageCount = 2;
		init_info.ImageCount = m_Swapchain->GetImageCount();
		init_info.PipelineInfoMain.RenderPass = m_Swapchain->GetRenderPass();

		if (!ImGui_ImplVulkan_Init(&init_info))
		{
			VT_CORE_ERROR("Failed to initialize ImGui Vulkan backend!");
			throw std::runtime_error("Failed to initialize ImGui Vulkan backend!");
		}

		// The backend will create / upload fonts automatically on the first NewFrame
		// call when needed (the imgui_impl_vulkan backend handles font atlas upload),
		// so there's no need to manually create font textures here.
		VT_CORE_INFO("ImGuiRenderer initialized");
	}

	void ImGuiRenderer::Shutdown()
	{
		VT_PROFILE_FUNCTION();
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		
		if (m_DescriptorPool != nullptr)
		{
			// Get device handle from ImGui Vulkan backend or store it
			// For now, we'll let Vulkan validation layers handle cleanup
			m_DescriptorPool = nullptr;
		}

		VT_CORE_INFO("ImGuiRenderer shutdown");
	}

	void ImGuiRenderer::Begin()
	{
		VT_PROFILE_FUNCTION();
		ImGui_ImplSDL3_NewFrame();
		ImGui_ImplVulkan_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiRenderer::End()
	{
		VT_PROFILE_FUNCTION();
		ImGui::Render();
	}

	void ImGuiRenderer::Render(VkCommandBuffer& commandBuffer)
	{
		VT_PROFILE_FUNCTION();
		ImDrawData* draw_data = ImGui::GetDrawData();
		if (draw_data != nullptr)
		{
			// Log draw data stats for debugging: number of command lists and total vertices
			int total_vtx = draw_data->TotalVtxCount;
			int total_idx = draw_data->TotalIdxCount;
			if (total_vtx == 0)
			{
				//VT_CORE_INFO("ImGui draw data empty: vtx=0 idx=0");
			}
			else
			{
				//VT_CORE_INFO("ImGui draw data: lists=%d verts=%d idx=%d", draw_data->CmdListsCount, total_vtx, total_idx);
			}
			ImGui_ImplVulkan_RenderDrawData(draw_data, commandBuffer);
		}
	}
}
