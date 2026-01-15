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

		virtual void DrawQuad(VkCommandBuffer& renderCommandBuffer, Ref<VulkanPipeline> pipeline, const glm::mat4& transform) override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual void BeginRendering(VkCommandBuffer& renderCommandBuffer, bool explicitClear = false) override;
		virtual void EndRendering(VkCommandBuffer& renderCommandBuffer) override;

		virtual void ClearScreen(VkCommandBuffer& renderCommandBuffer) override;

	private:

	};
}