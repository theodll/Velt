#pragma once

#include "vtpch.h"
#include "Velt/Core/Core.h"
#include "Velt/Platform/Vulkan/VulkanDevice.h"
#include <vulkan/vulkan.h>
#include "imgui.h"

namespace Velt {

	class VELT_API SceneViewport
	{
	public:
		SceneViewport(uint32_t width, uint32_t height);
		~SceneViewport();

		void Resize(uint32_t width, uint32_t height);
		
		VkImage GetImage() const { return m_Image; }
		VkImageView GetImageView() const { return m_ImageView; }
		VkDescriptorSet GetDescriptorSet() const { return m_DescriptorSet; }
		
		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

		void TransitionForRendering(VkCommandBuffer commandBuffer);
		void TransitionForSampling(VkCommandBuffer commandBuffer);

	private:
		void CreateResources();
		void CleanupResources();
		void CreateDescriptorSet();

		uint32_t m_Width;
		uint32_t m_Height;

		VkImage m_Image = VK_NULL_HANDLE;
		VkDeviceMemory m_ImageMemory = VK_NULL_HANDLE;
		VkImageView m_ImageView = VK_NULL_HANDLE;
		VkSampler m_Sampler = VK_NULL_HANDLE;
		VkDescriptorSet m_DescriptorSet = VK_NULL_HANDLE;

		Renderer::Vulkan::VulkanDevice& m_Device;
	};

}
