#pragma once
#include "vtpch.h"
#include "Core/Core.h"
#include "VulkanDevice.h"
#include "Velt/Renderer/Swapchain.h"
#include "Velt/Renderer/Framebuffer.h"
#include <SDL3/SDL.h>

namespace Velt::Renderer::RHI {

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

    class VELT_API VulkanSwapchain
    {
    public:
        static constexpr int MAX_FRAMES_IN_FLIGHT = 3;

        VulkanSwapchain();

        VulkanSwapchain(const VulkanSwapchain&) = delete;
        void operator=(const VulkanSwapchain&) = delete;

        void Init(SwapchainCreateInfo& createInfo);
        void InitSurface(SDL_Window* windowHandle);
        void Create(SwapchainCreateInfo& createInfo);
        void Destroy();

        void OnResize(SwapchainExtent& extend);

        void BeginFrame();
        void Present(); 

        inline u32 GetWidth() const { return m_WindowExtent.width; }
		inline u32 GetHeight() const { return m_WindowExtent.height; }
        inline u32 GetImageCount() const { return (u32)m_SwapchainImages.size(); } 
        inline u32 GetMaxFrameInFlight() const { return MAX_FRAMES_IN_FLIGHT; }
        inline float GetAspectRatio() const { return (float)m_WindowExtent.width / (float)m_WindowExtent.height; }

        inline u32 GetCurrentFrameIndex() const { return m_CurrentFrameIndex; }
        inline u32 GetCurrentImageIndex() const { return m_CurrentImageIndex; }
		
        inline SwapchainImage GetSwapchainImage(int index) { return m_SwapchainImages[index]; }
        inline SwapchainImage GetCurrentSwapchainImage() { return GetSwapchainImage(m_CurrentImageIndex); }
        inline VkCommandBuffer GetCurrentDrawCommandBuffer() {  return GetDrawCommandBuffer(m_CurrentFrameIndex); }

        static void TransitionImageLayout(
            VkCommandBuffer commandBuffer,
            VkImage image,
            VkImageLayout oldLayout,
            VkImageLayout newLayout,
            VkPipelineStageFlags srcStageMask,
            VkPipelineStageFlags dstStageMask);

        inline VkCommandBuffer GetDrawCommandBuffer(u32 index) 
        { 
            VT_CORE_ASSERT(index < m_Commandbuffers.size(), "Command buffer index out of bounds"); 
            return m_Commandbuffers.at(index).CommandBuffer; 
        }
        

        VkFormat findDepthFormat();
        
    private:
		u32 AcquireNextImage();
        u32 SubmitCommandBuffers(const VkCommandBuffer* buffers, u32* imageIndex);

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        void FindImageFormatAndColorSpace();
        VkFormat m_SwapChainImageFormat;

        VkInstance& m_Instance;
        VulkanDevice& m_Device;
		VkSurfaceKHR& m_Surface;
        VkSwapchainKHR m_Swapchain;

        VkFormat m_ColorFormat;
        VkColorSpaceKHR m_ColorSpace;


		std::vector<SwapchainCommandBuffer> m_Commandbuffers;

        std::vector<DepthStencilImage> m_DepthStencilImages;
        std::vector<SwapchainImage> m_SwapchainImages; 

        std::vector<VkSemaphore> m_ImageAvailableSemaphores;
        std::vector<VkSemaphore> m_RenderFinishedSemaphores;
        std::vector<VkFence> m_InFlightFences;
        std::vector<VkFence> m_ImagesInFlight;

        VkExtent2D m_WindowExtent;

        bool m_VSync = false;
        u32 m_QueueNodeIndex = UINT32_MAX; 
        
        u32 m_CurrentImageIndex = 0; // Frame currently displayed
        u32 m_CurrentFrameIndex = 0; // Frame that is being worked on
    };
    
}
