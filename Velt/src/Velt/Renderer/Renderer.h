#pragma once
#include "vtpch.h"
#include "RenderCommandBuffer.h"
#include "VertexBuffer.h"
#include "SceneRenderer.h"
#include "IndexBuffer.h"
#include "RenderAPI.h"

namespace Velt::Renderer
{
	static class Application;
	static class LayerStack;
	static class SceneRenderer;


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

		static void BeginRendering(VkCommandBuffer& renderCommandBuffer, Ref<VkRenderPass> renderPass, bool explicitClear = false);
		static void EndRendering(VkCommandBuffer& renderCommandBuffer);
		
		static void BeginFrame();
		static void EndFrame();

		static void BeginScenePass();
		static void EndScenePass();
		
		static void BeginGuiPass();
		static void EndGuiPass();

		static void DrawQuad(VkCommandBuffer& renderCommandBuffer, const glm::mat4& transform = glm::mat4(1.0f));

		
		inline static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); };

	private:
		static Scope<RenderAPI> s_RenderAPI; 
		static Scope<SceneRenderer> s_SceneRenderer;
		

	};
}
