#include "vtpch.h"
#include "VulkanRenderer.h"
#include "Velt/Platform/Vulkan/Buffer/VulkanVertexBuffer.h"
#include "Velt/Platform/Vulkan/Buffer/VulkanIndexBuffer.h"
#include "Velt/Platform/Vulkan/VulkanPipeline.h"
#include "vulkan/vulkan.h"
#include "Core/Application.h"
#include "Velt/Renderer/Model.h"

namespace Velt::RHI
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
			{ {-0.5f, -0.5f, 0.5f} },
			{ { 0.5f, -0.5f, 0.5f} },
			{ { 0.5f,  0.5f, 0.5f} },
			{ {-0.5f,  0.5f, 0.5f} }
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
		// Handle any pending viewport resize before starting command recording
		ImGuiLayer::ProcessPendingResize();
		
		auto& app = Velt::Application::Get();
		auto& window = app.GetWindow();
		auto& swapchain = window.GetSwapchain();
		auto&& currentCommandBuffer = swapchain.GetCurrentDrawCommandBuffer();

		swapchain.BeginFrame();

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		beginInfo.pInheritanceInfo = nullptr;

		vkBeginCommandBuffer(currentCommandBuffer, &beginInfo);
	}

	void VulkanRenderer::BeginScenePass() 
	{
		auto& app = Velt::Application::Get();
		auto& window = app.GetWindow();
		auto& sc = window.GetSwapchain();
		auto&& cmd = sc.GetCurrentDrawCommandBuffer();
		auto* viewport = ImGuiLayer::GetViewport();

		sc.TransitionImageLayout(
    	cmd,
    	viewport->GetImage(),
    	VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    	VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    	VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
    	VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
		);

		// Rendering Info Setup
		VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };

		VkRenderingAttachmentInfoKHR colorAttachmentInfo{};
		colorAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
		colorAttachmentInfo.imageView = viewport->GetImageView();
		colorAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		colorAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachmentInfo.clearValue = clearColor;

		VkRenderingInfoKHR renderInfo{};
		renderInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
		renderInfo.renderArea.extent = { viewport->GetWidth(), viewport->GetHeight() };
		renderInfo.renderArea.offset = { 0, 0 };
		renderInfo.layerCount = 1;
		renderInfo.colorAttachmentCount = 1;
		renderInfo.pColorAttachments = &colorAttachmentInfo;

		vkCmdBeginRendering(cmd, &renderInfo);

		u32 width = viewport->GetWidth();
		u32 height = viewport->GetHeight();

		VkRect2D scissor{};
		scissor.extent = { width, height };
		scissor.offset = { 0, 0 };
		vkCmdSetScissor(cmd, 0, 1, &scissor);

		VkViewport VulkanViewport{};
		VulkanViewport.height = (float)height;
		VulkanViewport.width = (float)width;
		VulkanViewport.x = 0;
		VulkanViewport.y = 0;
		VulkanViewport.maxDepth = 1.0f;
		VulkanViewport.minDepth = 0.0f;
		vkCmdSetViewport(cmd, 0, 1, &VulkanViewport);
	}

	void VulkanRenderer::EndScenePass() 
	{
		auto& app = Velt::Application::Get();
		auto& window = app.GetWindow();
		auto& sc = window.GetSwapchain();
		auto&& cmd = sc.GetCurrentDrawCommandBuffer();

		vkCmdEndRendering(cmd);


		auto* viewport = ImGuiLayer::GetViewport();
		VkImage sceneImg = viewport->GetImage();

		sc.TransitionImageLayout(
			cmd,
			sceneImg,
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT
		);
	}

	void VulkanRenderer::BeginGuiPass()
	{
		auto& app = Velt::Application::Get();
		auto& window = app.GetWindow();
		auto& sc = window.GetSwapchain();
		auto&& cmd = sc.GetCurrentDrawCommandBuffer();

		auto&& img = sc.GetCurrentSwapchainImage(); 

		// On first frame for this image, it's in UNDEFINED layout, not PRESENT_SRC_KHR
		VkImageLayout oldLayout = sc.IsFirstFrameForImage(sc.GetCurrentImageIndex()) 
			? VK_IMAGE_LAYOUT_UNDEFINED 
			: VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		sc.TransitionImageLayout(
			cmd,
			img.Image,
			oldLayout,
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,          
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
		);

		// Rendering Info Setup
		VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };

		VkRenderingAttachmentInfoKHR colorAttachmentInfo{};
		colorAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
		colorAttachmentInfo.imageView = sc.GetCurrentSwapchainImage().ImageView;
		colorAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		colorAttachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachmentInfo.clearValue = clearColor;

		VkRenderingInfoKHR renderInfo{};
		renderInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
		renderInfo.renderArea.extent = { sc.GetWidth(), sc.GetHeight() };
		renderInfo.renderArea.offset = { 0, 0 };
		renderInfo.layerCount = 1;
		renderInfo.colorAttachmentCount = 1;
		renderInfo.pColorAttachments = &colorAttachmentInfo;

		vkCmdBeginRendering(cmd, &renderInfo);

		u32 width = sc.GetWidth();
		u32 height = sc.GetHeight();

		VkRect2D scissor{};
		scissor.extent = { width, height };
		scissor.offset = { 0, 0 };
		vkCmdSetScissor(cmd, 0, 1, &scissor);

		VkViewport viewport{};
		viewport.height = (float)height;
		viewport.width = (float)width;
		viewport.x = 0;
		viewport.y = 0;
		viewport.maxDepth = 1.0f;
		viewport.minDepth = 0.0f;
		vkCmdSetViewport(cmd, 0, 1, &viewport);


	}

	void VulkanRenderer::EndGuiPass() 
	{
		auto& app = Velt::Application::Get();
		auto& window = app.GetWindow();
		auto& sc = window.GetSwapchain();
		auto&& cmd = sc.GetCurrentDrawCommandBuffer();

		vkCmdEndRendering(cmd);

		auto&& img = sc.GetCurrentSwapchainImage();
		sc.TransitionImageLayout(
			cmd,
			img.Image,
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT
		);
	}

	void VulkanRenderer::EndFrame()
	{
		auto& app = Velt::Application::Get();
		auto& window = app.GetWindow();
		auto& swapchain = window.GetSwapchain();
		auto&& currentCommandBuffer = swapchain.GetCurrentDrawCommandBuffer();

		vkEndCommandBuffer(currentCommandBuffer);
		swapchain.Present();
	}

	void VulkanRenderer::DrawQuad(VkCommandBuffer& renderCommandBuffer, const glm::mat4& transform)
	{
		auto pp = SceneRenderer::GetPipeline(); 
		VkPipelineLayout layout = pp->GetVulkanPipelineLayout();

		VkPipelineLayout pipelineLayout = pp->GetVulkanPipelineLayout();
		VkBuffer vertexBuffer = s_RenderData->QuadVertexBuffer->GetVulkanBuffer();
		VkCommandBuffer commandBuffer = renderCommandBuffer;

		VkDeviceSize offsets[1] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, offsets);

		VkBuffer indexBuffer = s_RenderData->QuadIndexBuffer->GetVulkanBuffer();
		vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

		uint32_t indexCount = s_RenderData->QuadIndexBuffer->GetCount();
		// VT_CORE_ERROR("{}", indexCount);

		vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &transform);
		vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
	}

	void VulkanRenderer::DrawStaticModel(VkCommandBuffer& renderCommandBuffer, const Ref<Model>& model)
	{
		auto pp = SceneRenderer::GetPipeline(); 
		VkPipelineLayout layout = pp->GetVulkanPipelineLayout();

		VkCommandBuffer commandBuffer = renderCommandBuffer;

		auto transform = model->GetTransform().mat4();
		vkCmdPushConstants(commandBuffer, layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &transform);

		auto& submeshes = model->GetSubmeshes();
		for (auto& submesh : submeshes)
		{
			VkBuffer vertexBuffer = submesh.Mesh->GetVertexBuffer()->GetVulkanBuffer();
			VkDeviceSize offsets[1] = { 0 };
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, offsets);

			VkBuffer indexBuffer = submesh.Mesh->GetIndexBuffer()->GetVulkanBuffer();
			vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

			uint32_t indexCount = (uint32_t)submesh.Mesh->GetIndexBuffer()->GetCount();

			vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
		}
	}

	void VulkanRenderer::ClearScreen(VkCommandBuffer& renderCommandBuffer)
	{
	}
}