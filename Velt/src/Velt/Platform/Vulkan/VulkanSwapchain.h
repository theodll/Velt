#pragma once
#include "vtpch.h"
#include "VulkanDevice.h"

namespace Velt::Renderer::Vulkan
{

    class VELT_API VulkanSwapchain
    {
    public:
        static constexpr int MAX_FRAMES_IN_FLIGHT = 3;

        VulkanSwapchain(VulkanDevice &deviceRef, VkExtent2D windowExtent);
        VulkanSwapchain(VulkanDevice &deviceRef, VkExtent2D windowExtent, VulkanSwapchain *previous);
        ~VulkanSwapchain();

        VulkanSwapchain(const VulkanSwapchain &) = delete;
        void operator=(const VulkanSwapchain &) = delete;

        VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
        VkRenderPass getRenderPass() { return renderPass; }
        VkImageView getImageView(int index) { return swapChainImageViews[index]; }
        size_t imageCount() { return swapChainImages.size(); }
        VkFormat getVulkanSwapchainImageFormat() { return swapChainImageFormat; }
        VkExtent2D getVulkanSwapchainExtent() { return swapChainExtent; }
        uint32_t width() { return swapChainExtent.width; }
        uint32_t height() { return swapChainExtent.height; }

        float extentAspectRatio()
        {
            return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height);
        }
        VkFormat findDepthFormat();

        VkResult acquireNextImage(uint32_t &imageIndex);
        VkResult submitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);

    private:
        void init();
        void createVulkanSwapchain();
        void createImageViews();
        void createDepthResources();
        void createRenderPass();
        void createFramebuffers();
        void createSyncObjects();

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(
            const std::vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(
            const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;

        std::vector<VkFramebuffer> swapChainFramebuffers;
        VkRenderPass renderPass;

        std::vector<VkImage> depthImages;
        std::vector<VkDeviceMemory> depthImageMemorys;
        std::vector<VkImageView> depthImageViews;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews;

        VulkanDevice &m_Device;
        VkExtent2D windowExtent;

        VkSwapchainKHR m_Swapchain;
        VulkanSwapchain *m_OldSwapchain;

        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> inFlightFences;
        std::vector<VkFence> imagesInFlight;
        size_t currentFrame = 0;
    };

}