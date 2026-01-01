#pragma once

#include "vtpch.h"
#include "Velt/Renderer/Renderer.h"
#include "VulkanSwapchain.h"

namespace Velt::Renderer::Vulkan
{
	class VulkanDevice;
	class VulkanPipeline;

	class VELT_API VulkanRenderer
	{
	public:
		VulkanRenderer();
		~VulkanRenderer();

		VulkanRenderer(const VulkanRenderer&) = delete;
		VulkanRenderer& operator=(const VulkanRenderer&) = delete;

		void Init(VulkanSwapchain& swapchain);
		void DrawFrame(VulkanSwapchain& swapchain);

	private:
		void CreatePipelineLayout();
		void CreatePipeline();
		void CreateVertexBuffer();
		void CreateCommandBuffers();
		void FreeCommandBuffers();
		void RecordCommandBuffer(u32 imageIndex, VulkanSwapchain& swapchain);

		VulkanDevice& m_Device;
		VulkanSwapchain* m_Swapchain = nullptr;
		std::unique_ptr<VulkanPipeline> m_Pipeline;
		VkPipelineLayout m_PipelineLayout = VK_NULL_HANDLE;
		std::vector<VkCommandBuffer> m_CommandBuffers;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
	};
}