#pragma once
#include "vtpch.h"

#include "Velt/Renderer/RenderCommandBuffer.h"
#include "vulkan/vulkan.h"


namespace Velt::Renderer::Vulkan {

	class VulkanCommandBuffer : public RenderCommandBuffer
	{
	public:
		VulkanCommandBuffer(u32 count = 0, std::string debugName = "");
		VulkanCommandBuffer(std::string debugName, bool swapchain);
		~VulkanCommandBuffer() override;

		virtual void Begin() override;
		virtual void End() override;
		virtual void Submit() override;

		virtual float GetExecutionGPUTime(u32 frameIndex, u32 queryIndex = 0) const override
		{
			if (queryIndex == UINT32_MAX || queryIndex / 2 >= m_TimestampNextAvailableQuery / 2)
				return 0.0f;

			return m_ExecutionGPUTimes[frameIndex][queryIndex / 2];
		}

		virtual u32 BeginTimestampQuery() override;
		virtual void EndTimestampQuery(u32 queryID) override;

		VkCommandBuffer GetActiveCommandBuffer() const { return m_ActiveCommandBuffer; }

		VkCommandBuffer GetCommandBuffer(u32 frameIndex) const
		{
			return m_CommandBuffers[frameIndex];
		}
	private:
		std::string m_DebugName;
		VkCommandPool m_CommandPool = nullptr;
		std::vector<VkCommandBuffer> m_CommandBuffers;
		VkCommandBuffer m_ActiveCommandBuffer = nullptr;
		std::vector<VkFence> m_WaitFences;

		bool m_OwnedBySwapChain = false;

		u32 m_TimestampQueryCount = 0;
		u32 m_TimestampNextAvailableQuery = 2;
		std::vector<VkQueryPool> m_TimestampQueryPools;
		std::vector<VkQueryPool> m_PipelineStatisticsQueryPools;
		std::vector<std::vector<uint64_t>> m_TimestampQueryResults;
		std::vector<std::vector<float>> m_ExecutionGPUTimes;

		u32 m_PipelineQueryCount = 0;
		// std::vector<PipelineStatistics> m_PipelineStatisticsQueryResults;
	};

}