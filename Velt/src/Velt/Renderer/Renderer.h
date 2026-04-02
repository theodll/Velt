#pragma once
#include "vtpch.h"
#include "Core/Core.h"
#include "RenderCommandBuffer.h"
#include "VertexBuffer.h"
#include "SceneRenderer.h"
#include "DefferedRenderer.h"
#include "IndexBuffer.h"
#include "RenderAPI.h"
#include "Material.h"
#include "Model.h"

namespace Velt
{

	enum RenderTarget
	{
		VT_RENDER_TARGET_ALBEDO_AO = 0,
		VT_RENDER_TARGET_NORMAL_ROUGH = 1,
		VT_RENDER_TARGET_METAL_EMIT = 2,
		VT_RENDER_TARGET_DEPTH = 3,
		VT_RENDER_TARGET_COMPOSITE = 4,
		VT_RENDER_TARGET_SAMPLER = 5,
		VT_RENDER_TARGET_MOUSE_PICKING = 10
	};

	class VELT_API Renderer
	{
	public:
		Renderer();
		~Renderer();

		static void Init();
		static void Shutdown();
		static void QueueShutdown();

		static void BeginScene();
		static void EndScene();

		static void BeginRendering(VkCommandBuffer& renderCommandBuffer, Ref<VkRenderPass> renderPass, bool explicitClear = false);
		static void EndRendering(VkCommandBuffer& renderCommandBuffer);

		static void BeginFrame();
		static void EndFrame();

		static void BeginScenePass();
		static void EndScenePass();

		static void BeginDefferedPass();
		static void EndDefferedPass();
		
		static void BeginGuiPass();
		static void EndGuiPass();

		static void DrawQuad(VkCommandBuffer& renderCommandBuffer, const Matrix& transform = glm::mat4(1.0f), const Material& material = Material());
		static void DrawStaticModel(VkCommandBuffer commandBuffer, const Ref<Pipeline>& pipeline, const Ref<Model>& model, const Ref<Mesh>& meshSource, u32 submeshIndex, const Ref<MaterialTable>& materialTable, const Matrix& transformModel);
		static void SubmitFullscreenTriangle(VkCommandBuffer renderCommandBuffer, const Ref<Pipeline>& pipeline, const Ref<DefferedShaderInput>& input);

		static i32 GetDrawCallCount();

		static void RecreateRenderTargets(u32 width, u32 height);

		static std::unordered_map<u32, Ref<Texture2D>> GetRenderTargets() { return s_RenderTargets; }
		static Ref<Texture2D> GetRenderTarget(u32 binding) { return s_RenderTargets[binding]; }

		inline static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); };

	private:
		static Scope<RenderAPI> s_RenderAPI; 
		static std::unordered_map<u32, Ref<Texture2D>> s_RenderTargets;

	};
}
