#include "vtpch.h"
#include "Renderer/RenderAPI.h"
#include "Platform/Vulkan/VulkanPipeline.h"
#include "Velt/Renderer/Renderer.h"

namespace Velt::Renderer::Vulkan
{
	class VELT_API VulkanRenderer : public RenderAPI
	{
	public:
		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void DrawQuad(Ref<VkCommandBuffer> renderCommandBuffer, Ref<VulkanPipeline> pipeline, const glm::mat4& transform) override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual void BeginRenderPass(Ref<VkCommandBuffer> renderCommandBuffer, Ref<VkRenderPass> renderpass, bool explicitClear = false) override;
		virtual void EndRenderPass(Ref<VkCommandBuffer> renderCommandBuffer) override;

		virtual void ClearScreen(Ref<VkCommandBuffer> renderCommandBuffer) override;

	private:

	};
}