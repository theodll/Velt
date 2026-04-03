#include "vtpch.h"
#include "Renderer/RenderAPI.h"
#include "Platform/Vulkan/VulkanPipeline.h"
#include "Velt/Renderer/Renderer.h"
#include "Renderer/DefferedRenderer.h"

namespace Velt::RHI
{

	struct RenderData
	{
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<IndexBuffer> QuadIndexBuffer;
		Ref<VertexBuffer> TexuredQuadVertexBuffer;
		Ref<IndexBuffer> TexturedQuadIndexBuffer;

		i32 DrawCallCount{};

		Ref<Material> FallBackMaterial;
	};

	struct alignas(16) PushConstantData
	{
		glm::mat4 Transform;
		u32 EntityID;
	};

	class VELT_API VulkanRenderer : public RenderAPI
	{
	public:
		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void WaitIdle() override;

		virtual void DrawQuad(VkCommandBuffer renderCommandBuffer, const Matrix& transform, const Material& material) override;
		virtual void DrawTexturedQuad(VkCommandBuffer renderCommandBuffer, const Ref<Texture2D> texture, const Matrix& transform) override;
		virtual void DrawStaticModel(VkCommandBuffer commandBuffer, const Ref<Pipeline>& pipeline, const Ref<Model>& model, const Ref<Mesh>& meshSource, u32 submeshIndex, const Ref<MaterialTable>& materialTable, const Matrix& transformModel, u32 entityID) override;
		

		virtual void SubmitFullscreenTriangle(VkCommandBuffer renderCommandBuffer, const Ref<Pipeline>& pipeline, const Ref<DefferedShaderInput>& input) override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual void BeginScenePass() override;
		virtual void EndScenePass() override;

		virtual void BeginDefferedPass() override;
		virtual void EndDefferedPass() override;

		virtual void BeginGuiPass() override;
		virtual void EndGuiPass() override;

		virtual void ClearScreen(VkCommandBuffer& renderCommandBuffer) override;

		virtual i32 GetDrawCallCount() override;
	private:
		static Scope<RenderData> s_RenderData;
	};
}