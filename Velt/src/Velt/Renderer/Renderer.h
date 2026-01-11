#pragma once
#include "vtpch.h"
#include "RenderCommandBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "RenderAPI.h"

namespace Velt::Renderer
{
	class Application;
	class LayerStack;



	class VELT_API Renderer
	{
	public:
		Renderer();
		~Renderer();

		static void Init();
		static void Shutdown();
		static void RequestShutdown();

		static void BeginScene();
		static void EndScene();

		static void BeginRenderPass(Ref<VkCommandBuffer> renderCommandBuffer, Ref<VkRenderPass> renderPass, bool explicitClear = false);
		static void EndRenderPass(Ref<VkCommandBuffer> renderCommandBuffer);
		
		static void BeginFrame();
		static void EndFrame();

		static void DrawQuad(Ref<VkCommandBuffer> renderCommandBuffer, Ref<Vulkan::VulkanPipeline> pipeline, const glm::mat4& transform);

		
		inline static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); };

	private:
		static Scope<RenderAPI> s_RenderAPI; 
		

	};
}
