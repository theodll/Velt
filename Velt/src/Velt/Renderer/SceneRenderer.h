#pragma once
#include "Core/Core.h"
#include "Pipeline.h"
#include "Platform/Vulkan/VulkanPipeline.h"
#include "Renderer/UniformBuffer.h"

namespace Velt::Renderer {


	class SceneRenderer
	{
	public:
		void Init();
		void Shutdown();

		void BeginScene();
		void EndScene();

		static Ref<Pipeline> GetPipeline() { return s_Pipeline; };
		 
	private:
		struct alignas(16)
		{
			glm::mat4 view;
			glm::mat4 proj;
			glm::mat4 viewProj;
		} m_Camera;	

		std::vector<Ref<UniformBuffer>> m_CameraUBOs;

		static Ref<Pipeline> s_Pipeline;
		

	};
}
