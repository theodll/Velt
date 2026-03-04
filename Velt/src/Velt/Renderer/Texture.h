#pragma once
#include "Core/Core.h"
#include <filesystem>

namespace Velt 
{
	class Texture 
	{
	public:
		virtual u32 GetWidth() const = 0;
		virtual u32 GetHeight() const = 0;
		virtual u32 Bind() const = 0;
	};

	class Texture2D : public Texture 
	{
	public:
		static Ref<Texture2D> Create(const std::filesystem::path& path);
	};
}