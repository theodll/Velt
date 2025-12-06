#pragma once
#include "Velt/Core/Core.h"

namespace Velt::Renderer
{

	struct FramebufferSpecification
	{
		u32 Width, Height;
		u32 Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual const FramebufferSpecification &getSpecification() const = 0;
		static Ref<Framebuffer> Create(const FramebufferSpecification &specs);
		virtual void Resize(u32 width, u32 height) = 0;
		virtual int ReadPixel(u32 attachmentIndex, int x, int y) = 0;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;
		virtual const FramebufferSpecification &GetSpecification() const = 0;
	};
	// namespace RenderAPI
} // namespace Velt
