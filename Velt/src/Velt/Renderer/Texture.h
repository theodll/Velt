#pragma once
#include "Core/Core.h"

#include <vulkan/vulkan.h>
#include <filesystem>

namespace Velt 
{
	struct TextureCreateInfo 
	{
		VkFormat                 Format;
		VkImageUsageFlags 	 	 Usage;
		VkExtent3D               Extent;
		VkImageLayout			 ImageLayout;
		VkImageAspectFlags 		 AspectMask;
	};
	
	class VELT_API Texture
	{
	public:
		virtual ~Texture() = default;
		virtual u32 GetWidth() const = 0;
		virtual u32 GetHeight() const = 0;

		virtual VkSampler GetSampler() const = 0;
		virtual VkImage GetImage() const = 0;
		virtual VkImageView GetImageView() const = 0;
	};

	class VELT_API Texture2D : public Texture 
	{
	public:
		virtual ~Texture2D() = default;
		static Ref<Texture2D> Create(const std::filesystem::path& path);
		static Ref<Texture2D> Create(i32 width, i32 height);
		static Ref<Texture2D> Create(TextureCreateInfo* pInfo);
	};
}