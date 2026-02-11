#pragma once
#include "Core/Core.h"
#include "Pipeline.h"
#include "Platform/Vulkan/VulkanPipeline.h"
#include "Renderer/UniformBuffer.h"
#include "Renderer/OrthographicCamera.h"

namespace Velt {


	class VELT_API SceneRenderer
	{
	public:
		void Init();
		void Shutdown();

		void BeginScene();
		void EndScene();

		static Ref<Pipeline> GetPipeline() { return s_Pipeline; };
		static OrthographicCamera* GetCamera() { return m_Camera.get(); };

	private:
		struct alignas(16) CameraUBO
		{
			glm::mat4 viewProj;
		};

		static Ref<OrthographicCamera> m_Camera;

		std::vector<Ref<UniformBuffer>> m_CameraUBOs;

		i32 m_Rotation{};
		glm::vec3 m_CameraPosition{};

		static Ref<Pipeline> s_Pipeline;
		

	};
}
