#include "vtpch.h"
#include "Renderer/RenderAPI.h"
#include "Velt/Renderer/Renderer.h"

namespace Velt::Renderer::Vulkan
{
	class VELT_API VulkanRenderer : public RenderAPI
	{
	public:
		void Init();
		void DrawQuad(Ref<VkCommandBuffer> renderCommandBuffer, Ref<VulkanPipeline>& pipeline, const glm::mat4& transform); 

	private:

	};
}