#pragma once
#include "vtpch.h"
#include "Core/Core.h"
#include "VulkanDevice.h"
#include "Velt/Renderer/Swapchain.h"
#include <SDL3/SDL.h>

namespace Velt::RHI {


    class VulkanSwapchain : Swapchain
    {
    public:
        static constexpr int MAX_FRAMES_IN_FLIGHT = 3;

        VulkanSwapchain();

        VulkanSwapchain(const VulkanSwapchain&) = delete;
        void operator=(const VulkanSwapchain&) = delete;

        void Init(SwapchainCreateInfo* pCreateInfo) override;
        void InitSurface(SDL_Window* pWindowHandle);
        void Create(SwapchainCreateInfo* pCreateInfo) override;
        void Destroy() override;

        void OnResize(SwapchainExtent* pExtend) override;

        void BeginFrame() override;
        void Present() override;

        inline u32 GetWidth() const override { return m_WindowExtent.width; }
		inline u32 GetHeight() const override { return m_WindowExtent.height; }
        inline u32 GetImageCount() const override  { return (u32)m_SwapchainImages.size(); }
        inline u32 GetMaxFrameInFlight() const override  { return MAX_FRAMES_IN_FLIGHT; }
        inline float GetAspectRatio() const override  { return (float)m_WindowExtent.width / (float)m_WindowExtent.height; }

        inline u32 GetCurrentFrameIndex() const override  { return m_CurrentFrameIndex; }
        inline u32 GetCurrentImageIndex() const override  { return m_CurrentImageIndex; }
        inline bool IsFirstFrameForImage(u32 imageIndex) const override  { return !m_ImagePresentedOnce[imageIndex]; }
		
        inline SwapchainImage GetSwapchainImage(int index) override  { return m_SwapchainImages[index]; }
        inline SwapchainImage GetCurrentSwapchainImage() override  { return GetSwapchainImage(m_CurrentImageIndex); }
        inline VkCommandBuffer GetCurrentDrawCommandBuffer() override {  return GetDrawCommandBuffer(m_CurrentFrameIndex); }

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
        

        VkFormat FindDepthFormat();
        
    private:
		VkResult AcquireNextImage();
        VkResult SubmitCommandBuffers(const VkCommandBuffer* buffers, u32* imageIndex);

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

        SDL_Window* m_WindowHandle = nullptr;

        bool m_VSync = false;
        u32 m_QueueNodeIndex = UINT32_MAX; 
        
        u32 m_CurrentImageIndex = 0; // Frame currently displayed
        u32 m_CurrentFrameIndex = 0; // Frame that is being worked on
        
        std::vector<u32> m_ImagePresentedOnce; // Track if each image has been presented at least once
    };
    
}
