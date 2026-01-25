#pragma once
#include "Platform/Vulkan/VulkanDevice.h"

namespace Velt
{

	class VELT_API SceneViewport
	{
	public:
		SceneViewport();
		virtual ~SceneViewport() = default; 

		void Init(u32 width, u32 height);
		void Shutdown();

		void Resize(u32 width, u32 height);

		VkImage GetImage() const { return m_Image; }
		VkImageView GetImageView() const { return m_ImageView; }
		VkDescriptorSet GetDescriptorSet() const { return m_DescriptorSet; }

		u32 GetWidth() const { return m_Width; }
		u32 GetHeight() const { return m_Height; }

		void TransitionForRendering(VkCommandBuffer commandBuffer);
		void TransitionForSampling(VkCommandBuffer commandBuffer);

	private:
		void CreateResources();
		void CleanupResources();
		void CreateDescriptorSet();

		u32 m_Width;
		u32 m_Height;

		VkImage m_Image = VK_NULL_HANDLE;
		VkDeviceMemory m_ImageMemory = VK_NULL_HANDLE;
		VkImageView m_ImageView = VK_NULL_HANDLE;
		VkSampler m_Sampler = VK_NULL_HANDLE;
		VkDescriptorSet m_DescriptorSet = VK_NULL_HANDLE;

		Renderer::RHI::VulkanDevice& m_Device;
	};

}
