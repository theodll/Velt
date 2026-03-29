#pragma once
#include "Core/Core.h"
#include "Camera.h"
#include "Pipeline.h"
#include "Platform/Vulkan/VulkanPipeline.h"
#include "Renderer/UniformBuffer.h"
#include "Editor/EditorCamera.h"

namespace Velt {


	class VELT_API SceneRenderer
	{
	public:
		void Init();
		void Shutdown();

		void BeginScene();
		void EndScene();

		static Ref<Pipeline> GetPipeline() { return s_GeometryPipeline; };
		static Ref<EditorCamera> GetCamera() { return m_Camera; };

	private:
		struct alignas(16) CameraUBO
		{
			Matrix viewProj;
			Matrix invViewProj;
			Vector cameraPos;
		};

		static Ref<EditorCamera> m_Camera;

		std::vector<Ref<UniformBuffer>> m_CameraUBOs;
		std::vector<VkDescriptorSet> m_GlobalSets;
		u32 m_CameraUBOBinding{};

		
		i32 m_Rotation{};
		Vector m_CameraPosition{};

		static Ref<Pipeline> s_GeometryPipeline;
		

	};
}
