#pragma once
#include "Velt/Core/Core.h"

namespace Velt
{
	class VELT_API Swapchain
	{
	public:
		virtual ~Swapchain() = default;

		virtual void* GetFramebuffer(int index) = 0;
		virtual void* GetRenderPass() = 0;
		virtual void* GetImageView(int index) = 0;
		virtual size_t GetImageCount() const = 0;
		virtual u32 GetWidth() const = 0;
		virtual u32 GetHeight() const = 0;
		virtual float GetAspectRatio() const = 0;

		virtual int AcquireNextImage(u32& imageIndex) = 0;
		virtual int SubmitCommandBuffers(const void* buffers, u32* imageIndex) = 0;

		static Ref<Swapchain> Create(void* device, void* windowExtent);
		static Ref<Swapchain> Create(void* device, void* windowExtent, Ref<Swapchain> oldSwapchain);
	};
}
