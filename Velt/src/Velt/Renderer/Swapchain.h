#pragma once
#include "Velt/Core/Core.h"

namespace Velt
{

    struct SwapchainExtent
    {
        u32 Height;
        u32 Width;
    };

    struct SwapchainCreateInfo
    {
        u32 Width;
        u32 Height;
        bool VSync;
    };

    struct SwapchainCommandBuffer
    {
        VkCommandBuffer CommandBuffer = nullptr;
        VkCommandPool CommandPool = nullptr;
    };

    struct SwapchainImage
    {
        VkImage Image = nullptr;
        VkImageView ImageView = nullptr;
    };

    struct DepthStencilImage
    {
        VkImage DepthImage = nullptr;
        VkDeviceMemory DepthImageMemory = nullptr;
        VkImageView DepthImageView = nullptr;
    };

	class VELT_API Swapchain
	{
	public:
        virtual void Init(SwapchainCreateInfo& createInfo);
        virtual void InitSurface(SDL_Window* windowHandle);
        virtual void Create(SwapchainCreateInfo& createInfo);
        virtual void Destroy();

        virtual void OnResize(SwapchainExtent& extend);

        virtual void BeginFrame();
        virtual void Present();

        inline virtual u32 GetWidth() const = 0;
        inline virtual u32 GetHeight() const = 0;
        inline virtual u32 GetImageCount() const = 0;
        inline virtual u32 GetMaxFrameInFlight() const = 0;
        inline virtual float GetAspectRatio() const = 0;

        inline virtual u32 GetCurrentFrameIndex() const = 0;
        inline virtual u32 GetCurrentImageIndex() const = 0;
        inline virtual bool IsFirstFrameForImage(u32 imageIndex) const = 0;

        inline virtual SwapchainImage GetSwapchainImage(int index) = 0;
        inline virtual SwapchainImage GetCurrentSwapchainImage() = 0;
        inline virtual VkCommandBuffer GetCurrentDrawCommandBuffer() = 0;

	};
}
