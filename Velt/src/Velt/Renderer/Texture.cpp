#include "vtpch.h"
#include "Texture.h"
#include <filesystem>
#include "Renderer.h"
#include "Platform/Vulkan/VulkanTexture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Velt 
{
	Ref<Texture2D> Texture2D::Create(const std::filesystem::path& path) 
	{
		VT_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:   return nullptr;
		case RenderAPI::API::Vulkan:  return CreateRef<RHI::VulkanTexture2D>(path);
		case RenderAPI::API::D3D12:	VT_CORE_ASSERT(false, "D3D12 is currently unsupported");
		case RenderAPI::API::METAL: VT_CORE_ASSERT(false, "Metal is currently unsupported");
		}
		VT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}