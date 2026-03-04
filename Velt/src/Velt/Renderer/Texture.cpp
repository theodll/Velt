#include "vtpch.h"
#include "Texture.h"
#include <filesystem>
#include "Renderer.h"

namespace Velt 
{
	Ref<Texture2D> Texture2D::Create(const std::filesystem::path& path) 
	{
		VT_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:    return nullptr;
		//case RenderAPI::API::Vulkan:  return CreateRef<RHI::>(vertexData, bytes, 0, autoupload);
		}
		VT_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}