#pragma once
#include "Platform/Vulkan/VulkanDevice.h"
#include "Renderer/Renderer.h"

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

		VkImage GetDepthImage() const { return m_DepthImage; }
		VkImageView GetDepthImageView() const { return m_DepthImageView; }
	
		u32 GetWidth() const { return m_Width; }
		u32 GetHeight() const { return m_Height; }

		void TransitionForRendering(VkCommandBuffer commandBuffer);
		void TransitionForSampling(VkCommandBuffer commandBuffer);

		void SetRenderTarget(RenderTarget target);
		RenderTarget GetRenderTarget() { return m_RenderTarget; }
	private:
		void CreateResources();
		void CleanupResources();
		void CreateDescriptorSet();

		RenderTarget m_RenderTarget = VT_RENDER_TARGET_ALBEDO_AO;

		u32 m_Width;
		u32 m_Height;

		VkImage m_Image = VT_NULL_HANDLE;
		VkDeviceMemory m_ImageMemory = VT_NULL_HANDLE;
		VkImageView m_ImageView = VT_NULL_HANDLE;
		VkSampler m_Sampler = VT_NULL_HANDLE;
		VkDescriptorSet m_DescriptorSet = VT_NULL_HANDLE;

		VkImage m_DepthImage = VT_NULL_HANDLE;
		VkDeviceMemory m_DepthImageMemory = VT_NULL_HANDLE;
		VkImageView m_DepthImageView = VT_NULL_HANDLE;

		RHI::VulkanDevice& m_Device;
	};

}
