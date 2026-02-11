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

		virtual void DrawQuad(VkCommandBuffer& renderCommandBuffer, const glm::mat4& transform) override;
		virtual void DrawStaticModel(VkCommandBuffer& renderCommandBuffer, const Ref<Model>& model) override;
		
		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual void BeginScenePass() override;
		virtual void EndScenePass() override;

		virtual void BeginGuiPass() override;
		virtual void EndGuiPass() override;

		virtual void ClearScreen(VkCommandBuffer& renderCommandBuffer) override;

	private:

	};
}