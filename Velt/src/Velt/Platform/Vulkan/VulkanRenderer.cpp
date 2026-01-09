#include "vtpch.h"
#include "VulkanRenderer.h"
#include "Velt/Platform/Vulkan/Buffer/VulkanVertexBuffer.h"
#include "Velt/Platform/Vulkan/Buffer/VulkanIndexBuffer.h"
#include "Velt/Platform/Vulkan/VulkanPipeline.h"

namespace Velt::Renderer::Vulkan 
{

	struct RenderData
	{
		Ref<VulkanVertexBuffer> QuadVertexBuffer;
		Ref<VulkanIndexBuffer> QuadIndexBuffer;
	};

	static Scope<RenderData> s_RenderData = nullptr;


	void VulkanRenderer::Init()
	{
		VT_PROFILE_FUNCTION();
		s_RenderData = CreateScope<RenderData>();
	}

	void VulkanRenderer::Shutdown()
	{

	}

	void VulkanRenderer::BeginFrame()
	{

	}

	void VulkanRenderer::EndFrame()
	{

	}

	void VulkanRenderer::BeginRenderPass(Ref<VkCommandBuffer> renderCommandBuffer, Ref<VkRenderPass> renderpass, bool explicitClear /*= false*/)
	{

	}

	void VulkanRenderer::EndRenderPass(Ref<VkCommandBuffer> renderCommandBuffer)
	{

	}

	void VulkanRenderer::DrawQuad(Ref<VkCommandBuffer> renderCommandBuffer, Ref<VulkanPipeline> pipeline, const glm::mat4& transform)
	{
		VT_PROFILE_FUNCTION();

		VkPipelineLayout pipelineLayout = pipeline->GetVulkanPipelineLayout();
		VkBuffer vertexBuffer = s_RenderData->QuadVertexBuffer->GetVulkanBuffer();
		VkCommandBuffer commandBuffer = *renderCommandBuffer.get();

		VkDeviceSize offsets[1] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, offsets);

		VkBuffer indexBuffer = s_RenderData->QuadIndexBuffer->GetVulkanBuffer();
		vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

		vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &transform);

		uint32_t indexCount = s_RenderData->QuadIndexBuffer->GetCount();
		vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
	}

	void VulkanRenderer::ClearScreen(Ref<VkCommandBuffer> renderCommandBuffer)
	{

	}

}