#pragma once
#include "Core/Core.h"
#include "Camera.h"
#include "Pipeline.h"
#include "Platform/Vulkan/VulkanPipeline.h"
#include "Renderer/UniformBuffer.h"
#include "Editor/EditorCamera.h"

#include <vulkan/vulkan.h>

namespace Velt {


	class VELT_API SceneRenderer
	{
	public:
		void Init();
		void Shutdown();

		void BeginScene(const Ref<Camera>& pCamera);
		void EndScene();

		static Ref<Pipeline> GetPipeline() { return s_GeometryPipeline; };

	private:
		struct alignas(16) CameraUBO
		{
			Matrix viewProj;
			Matrix invViewProj;
			Vector cameraPos;
		};

		std::vector<Ref<UniformBuffer>> m_CameraUBOs;
		std::vector<VkDescriptorSet> m_GlobalSets;
		u32 m_CameraUBOBinding{};


		i32 m_Rotation{};
		Vector m_CameraPosition{};

		static Ref<Pipeline> s_GeometryPipeline;
	};
}
