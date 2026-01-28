#pragma once
#include "Core/Core.h"
#include "Pipeline.h"
#include "Platform/Vulkan/VulkanPipeline.h"
#include "Renderer/UniformBuffer.h"
#include "Renderer/OrthographicCamera.h"

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
		struct alignas(16) CameraUBO
		{
			glm::mat4 viewProj;
		};

		Ref<OrthographicCamera> m_Camera;

		std::vector<Ref<UniformBuffer>> m_CameraUBOs;

		i32 m_Rotation{};

		static Ref<Pipeline> s_Pipeline;
		

	};
}
