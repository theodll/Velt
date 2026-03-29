#pragma once
#include "Core/Core.h"
#include "Renderer/Texture.h"
#include <stb_image.h>
#include <filesystem>

namespace Velt::RHI 
{
	class VulkanTexture2D : public Texture2D 
	{
	public: 
		VulkanTexture2D(const std::filesystem::path& path);
		VulkanTexture2D(i32 width, i32 height);
		VulkanTexture2D(TextureCreateInfo* pInfo);
		virtual ~VulkanTexture2D();
		
		virtual u32 GetWidth() const override { return m_Width; }
		virtual u32 GetHeight() const override { return m_Height; }

		virtual VkSampler GetSampler() const override { return m_Sampler; }
		virtual VkImage GetImage() const override { return m_Image; }
		virtual VkImageView GetImageView() const override { return m_ImageView; }

	private: 
		void CreateImage(stbi_uc* pPixelData);
		void CreateImageWithoutData();
		void CreateImageView();
		void CreateImageSampler();

	private:
		// Note [6.03.26, Theo]: The Texture in theory must not store the path, but 
		// rather some kind of asset manager should.

		// Todo [6.03.26, Theo]: Move to asset manager
		std::filesystem::path m_Path = "";
		u32 m_Width{}, m_Height{};

		VkImage m_Image;
		VkDeviceMemory m_ImageMemory;
		VkImageView m_ImageView;
		VkSampler m_Sampler;
	};
}