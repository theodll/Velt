#pragma once
#include "Core/Core.h"
#include "Renderer/Texture.h"
#include <filesystem>

namespace Velt::RHI 
{
	class VulkanTexture2D : public Texture2D 
	{
	public: 
		VulkanTexture2D(const std::filesystem::path& path);
		virtual ~VulkanTexture2D() = default;
		
		virtual u32 GetWidth() const override { return m_Width; }
		virtual u32 GetHeight() const override { return m_Height; }


	private:
		// Note [6.03.26, Theo]: The Texture in theory must not store the path, but 
		// rather some kind of asset manager should.

		// Todo [6.03.26, Theo]: Move to asset manager
		const std::filesystem::path& m_Path;
		u32 m_Width{}, m_Height{};

		VkImage m_Image;
		VkImageView m_ImageView;
		VkSampler m_Sampler;
	};
}