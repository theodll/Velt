#pragma once
#include "Core/Core.h"
#include "Pipeline.h"
#include "Platform/Vulkan/VulkanPipeline.h"

namespace Velt::Renderer {

	class SceneRenderer
	{
	public:
		void Init();
		void Shutdown();

		void BeginScene();
		void EndScene();

//		VkCommandBuffer& GetVulkanCommandBuffer() { return m_Commandbuffer; }
		static Ref<Pipeline> GetPipeline() { return s_Pipeline; };
		VkRenderPass GetFinalRenderpass();
		 
	private:
		
		static Ref<Pipeline> s_Pipeline;

		// VkCommandBuffer m_Commandbuffer;

	};
}


//my plan is as follows just to get it to work:
// in application: 
// DrawQuad(commandbuffer, pipeline, {1, 1, 1, 1}) 
// commandbuffer and pipeline are fetched through this class (eg. m_SceneRenderer->GetVulkanCommandBuffer() / GetPipeline())
// Scene Renderer must also be created in Application::Init or somewhere like that. Then for every frame this takes care of it