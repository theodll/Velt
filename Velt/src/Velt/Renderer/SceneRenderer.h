#pragma once
#include "Core/Core.h"
#include "Pipeline.h"

namespace Velt::Renderer {

	class SceneRenderer
	{
	public:
		void Init();
		void Shutdown();

		void BeginScene();
		void EndScene();

		VkCommandBuffer& GetVulkanCommandBuffer() { return m_Commandbuffer; }
		Ref<Pipeline> GetPipeline();
		VkRenderPass& GetFinalRenderpass();
		 
	private:
		VkCommandBuffer m_Commandbuffer;

	};
}