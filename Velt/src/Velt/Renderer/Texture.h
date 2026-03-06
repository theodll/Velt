#pragma once
#include "Core/Core.h"
#include <filesystem>

namespace Velt 
{
	class VELT_API Texture
	{
	public:
		virtual ~Texture() = default;
		virtual u32 GetWidth() const = 0;
		virtual u32 GetHeight() const = 0;
	};

	class VELT_API Texture2D : public Texture 
	{
	public:
		virtual ~Texture2D() = default;
		static Ref<Texture2D> Create(const std::filesystem::path& path);
	};
}