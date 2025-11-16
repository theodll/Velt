#include "lvpch.h"
#include "ImGuiRenderer.h"
#include "Log.h"

namespace Lavendel {
	void ImGuiRenderer::Init(VkInstance instance, VkPhysicalDevice physicalDevice, VkDevice device,
	                          VkQueue queue, uint32_t queueFamily, VkRenderPass renderPass)
	{
		// Setup ImGui context
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		// Setup ImGui style
		ImGui::StyleColorsDark();

		// Create descriptor pool for ImGui
		VkDescriptorPoolSize pool_sizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100 },
		};

		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 100;
		pool_info.poolSizeCount = (uint32_t)std::size(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;

		if (vkCreateDescriptorPool(device, &pool_info, nullptr, (VkDescriptorPool*)&m_DescriptorPool) != VK_SUCCESS)
		{
			LV_CORE_ERROR("Failed to create ImGui descriptor pool");
			return;
		}

		// Setup ImGui for Vulkan
		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = instance;
		init_info.PhysicalDevice = physicalDevice;
		init_info.Device = device;
		init_info.QueueFamily = queueFamily;
		init_info.Queue = queue;
		init_info.DescriptorPool = (VkDescriptorPool)m_DescriptorPool;
		init_info.MinImageCount = 2;
		init_info.ImageCount = 3;
		init_info.Allocator = nullptr;
		init_info.PipelineInfoMain.RenderPass = renderPass;
		init_info.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		init_info.CheckVkResultFn = nullptr;

		ImGui_ImplVulkan_Init(&init_info);

		LV_CORE_INFO("ImGui Renderer initialized with Vulkan");
	}

	void ImGuiRenderer::Begin()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiRenderer::End()
	{
		ImGui::Render();
	}

	void ImGuiRenderer::Render(VkCommandBuffer commandBuffer)
	{
		ImDrawData* draw_data = ImGui::GetDrawData();
		ImGui_ImplVulkan_RenderDrawData(draw_data, commandBuffer);
	}

	void ImGuiRenderer::Shutdown()
	{
		ImGui_ImplVulkan_Shutdown();
		ImGui::DestroyContext();

		LV_CORE_INFO("ImGui Renderer shutdown");
	}
}
