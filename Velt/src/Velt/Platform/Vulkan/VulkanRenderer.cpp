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

		Vertex quadVerticesData[] = {
			{ {-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{ { 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{ { 0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},
			{ {-0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}}
		};
		std::vector<Vertex> quadVertices(quadVerticesData, quadVerticesData + 4);

		s_RenderData->QuadVertexBuffer = CreateRef<VulkanVertexBuffer>(quadVertices.data(), quadVertices.size(), sizeof(Vertex));
		std::vector<u32> quadIndices = { 0, 1, 2, 2, 3, 0 };
		s_RenderData->QuadIndexBuffer = CreateRef<VulkanIndexBuffer>(quadIndices.data(), quadIndices.size());

		// Upload quad buffers
		auto uploader = VulkanContext::GetResourceUploader();
		uploader.Begin();
		s_RenderData->QuadVertexBuffer->Upload(uploader.GetCommandBuffer());
		s_RenderData->QuadIndexBuffer->Upload(uploader.GetCommandBuffer());
		uploader.End();
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