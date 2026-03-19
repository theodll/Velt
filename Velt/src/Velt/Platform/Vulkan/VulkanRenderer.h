#include "vtpch.h"
#include "Renderer/RenderAPI.h"
#include "Platform/Vulkan/VulkanPipeline.h"
#include "Velt/Renderer/Renderer.h"

namespace Velt::RHI
{
	class VELT_API VulkanRenderer : public RenderAPI
	{
	public:
		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void DrawQuad(VkCommandBuffer renderCommandBuffer, const Matrix& transform, const Material& material) override;
		virtual void DrawTexturedQuad(VkCommandBuffer renderCommandBuffer, const Ref<Texture2D> texture, const Matrix& transform) override;
		virtual void DrawStaticModel(VkCommandBuffer renderCommandBuffer, const Ref<Pipeline>& pipeline, const Ref<Model>& model, const Ref<Mesh>& meshSource, u32 submeshIndex, const Ref<MaterialTable>& materialTable) override;
		
		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual void BeginScenePass() override;
		virtual void EndScenePass() override;

		virtual void BeginGuiPass() override;
		virtual void EndGuiPass() override;

		virtual void ClearScreen(VkCommandBuffer& renderCommandBuffer) override;

	private:
		void RenderStaticMesh(
			VkCommandBuffer commandBuffer,
			const Ref<Pipeline>& pipeline,
			const Ref<Model>& model,
			const Ref<Mesh>& meshSource,
			u32 submeshIndex,
			const Ref<MaterialTable>& materialTable,
			const Ref<Material>& fallbackMaterial
		);
	};
}