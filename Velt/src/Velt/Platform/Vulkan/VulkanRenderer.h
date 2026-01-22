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

		virtual void DrawQuad(VkCommandBuffer& renderCommandBuffer) override;

		virtual void BeginFrame() override;
		virtual void EndFrame() override;

		virtual void BeginRendering(VkCommandBuffer& renderCommandBuffer, bool explicitClear = false) override;
		virtual void EndRendering(VkCommandBuffer& renderCommandBuffer) override;

		virtual void BeginScenePass() override;
		virtual void EndScenePass() override;

		virtual void BeginGuiPass() override;
		virtual void EndGuiPass() override;

		virtual void ClearScreen(VkCommandBuffer& renderCommandBuffer) override;

	private:

	};
}