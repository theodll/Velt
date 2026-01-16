#include "vtpch.h"
#include "VulkanRenderer.h"
#include "Velt/Platform/Vulkan/Buffer/VulkanVertexBuffer.h"
#include "Velt/Platform/Vulkan/Buffer/VulkanIndexBuffer.h"
#include "Velt/Platform/Vulkan/VulkanPipeline.h"
#include "vulkan/vulkan.h"
#include "Core/Application.h"

namespace Velt::Renderer::Vulkan
{
	struct RenderData
	{
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<IndexBuffer> QuadIndexBuffer;
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
		s_RenderData->QuadVertexBuffer = VertexBuffer::Create(quadVertices.data(), quadVertices.size(), sizeof(Vertex));

		std::vector<u32> quadIndices = { 0, 1, 2, 2, 3, 0 };
		s_RenderData->QuadIndexBuffer = IndexBuffer::Create(quadIndices.data(), quadIndices.size());

		// Upload quad buffers
		auto& uploader = VulkanContext::GetResourceUploader();
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
		auto& app = Velt::Application::Get();
		auto& window = app.GetWindow();
		auto& swapchain = window.GetSwapchain();
		auto&& currentCommandBuffer = swapchain.GetCurrentDrawCommandBuffer();

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		beginInfo.pInheritanceInfo = nullptr;

		vkBeginCommandBuffer(currentCommandBuffer, &beginInfo);

		BeginRendering(currentCommandBuffer, false);
	}

	void VulkanRenderer::EndFrame()
	{
		auto& app = Velt::Application::Get();
		auto& window = app.GetWindow();
		auto& swapchain = window.GetSwapchain();
		auto&& currentCommandBuffer = swapchain.GetCurrentDrawCommandBuffer();

		EndRendering(currentCommandBuffer);
		vkEndCommandBuffer(currentCommandBuffer);
		swapchain.Present();
	}

	void VulkanRenderer::BeginRendering(VkCommandBuffer& renderCommandBuffer, bool explicitClear /*= false*/)
	{
		auto& app = Velt::Application::Get();
		auto& window = app.GetWindow();
		auto& sc = window.GetSwapchain();

		/*
		VkRenderPassBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		beginInfo.renderPass = renderpass;
		beginInfo.framebuffer = swapchain.GetCurrentFramebuffer();
		beginInfo.renderArea.extent = { swapchain.GetWidth(), swapchain.GetHeight() };
		beginInfo.renderArea.offset = { 0, 0 };

		std::array<VkClearValue, 2> clearValues;
		clearValues[0].color = { 0.0f, 1.0f, 0.0f, 0.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		beginInfo.clearValueCount = (u32)clearValues.size();
		beginInfo.pClearValues = clearValues.data();*/

		// vkCmdBeginRenderPass(renderCommandBuffer, &beginInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkImageMemoryBarrier2 barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
		barrier.srcStageMask = VK_PIPELINE_STAGE_2_NONE;
		barrier.srcAccessMask = 0;
		barrier.dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
		barrier.dstAccessMask = VK_ACCESS_2_COLOR_ATTACHMENT_WRITE_BIT;
		barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED; 
		barrier.newLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
		barrier.image = sc.GetCurrentSwapchainImage().Image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkDependencyInfo dep{};
		dep.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
		dep.imageMemoryBarrierCount = 1;
		dep.pImageMemoryBarriers = &barrier;

		vkCmdPipelineBarrier2(renderCommandBuffer, &dep);


		VkRenderingAttachmentInfoKHR colorAttachmentInfo{};
		colorAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
		colorAttachmentInfo.imageView = sc.GetCurrentSwapchainImage().ImageView;
		colorAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;
		colorAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

		VkRenderingInfoKHR renderInfo{};
		renderInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
		renderInfo.renderArea.extent = { sc.GetWidth(), sc.GetHeight() };
		renderInfo.renderArea.offset = { 0, 0 };
		renderInfo.layerCount = 1;
		renderInfo.colorAttachmentCount = 1;
		renderInfo.pColorAttachments = &colorAttachmentInfo;

		vkCmdBeginRendering(renderCommandBuffer, &renderInfo);
	}

	void VulkanRenderer::EndRendering(VkCommandBuffer& renderCommandBuffer)
	{
		vkCmdEndRendering(renderCommandBuffer);
	}

	void VulkanRenderer::DrawQuad(VkCommandBuffer& renderCommandBuffer, Ref<VulkanPipeline> pipeline, const glm::mat4& transform)
	{
		VT_PROFILE_FUNCTION();

		VkPipelineLayout pipelineLayout = pipeline->GetVulkanPipelineLayout();
		VkBuffer vertexBuffer = s_RenderData->QuadVertexBuffer->GetVulkanBuffer();
		VkCommandBuffer commandBuffer = renderCommandBuffer;

		VkDeviceSize offsets[1] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, offsets);

		VkBuffer indexBuffer = s_RenderData->QuadIndexBuffer->GetVulkanBuffer();
		vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

		vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &transform);

		uint32_t indexCount = s_RenderData->QuadIndexBuffer->GetCount();
		vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
	}

	void VulkanRenderer::ClearScreen(VkCommandBuffer& renderCommandBuffer)
	{
	}
}