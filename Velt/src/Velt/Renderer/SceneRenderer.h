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
		VkRenderPass GetFinalRenderpass();
		 
	private:
		VkCommandBuffer m_Commandbuffer;

	};
}


//my plan is as follows just to get it to work:
// in application: 
// DrawQuad(commandbuffer, pipeline, {1, 1, 1, 1}) 
// commandbuffer and pipeline are fetched through this class (eg. m_SceneRenderer->GetVulkanCommandBuffer() / GetPipeline())
// Scene Renderer must also be created in Application::Init or somewhere like that. Then for every frame this takes care of it