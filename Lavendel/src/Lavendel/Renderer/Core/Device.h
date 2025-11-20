#pragma once

#include "lvpch.h"
#include "../../Core.h"
#include "../Window.h"

namespace Lavendel {
    namespace RenderAPI {

        // Forward declaration to break circular dependency
        class LAVENDEL_API Window;

        struct SwapChainSupportDetails
        {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

        struct QueueFamilyIndices
        {
            uint32_t graphicsFamily;
            uint32_t presentFamily;
            bool graphicsFamilyHasValue = false;
            bool presentFamilyHasValue = false;
            bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
        };

		class LAVENDEL_API GPUDevice
        {
        public:
        #ifdef NDEBUG
            const bool enableValidationLayers = false;
        #else
            const bool enableValidationLayers = true;
        #endif

            GPUDevice(Window& window);
            ~GPUDevice();

            // Not copyable or movable
            GPUDevice(const GPUDevice&) = delete;
            void operator=(const GPUDevice&) = delete;
            GPUDevice(GPUDevice&&) = delete;
            GPUDevice& operator=(GPUDevice&&) = delete;

            VkCommandPool getCommandPool() { return m_CommandPool; }
            VkDevice device() { return m_Device; }
            VkSurfaceKHR surface() { return m_Surface; }
            VkQueue graphicsQueue() { return m_GraphicsQueue; }
            VkQueue presentQueue() { return m_PresentQueue; }
            VkInstance getInstance() { return m_Instance; }
            VkPhysicalDevice getPhysicalDevice() { return m_PhysicalDevice; }
            VkQueue getGraphicsQueue() { return m_GraphicsQueue; }
            uint32_t getQueueFamilyIndex() { return findPhysicalQueueFamilies().graphicsFamily; }
            // Access underlying window
            RenderAPI::Window& getWindow() { return m_Window; }
            
            SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(m_PhysicalDevice); }
            uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
            QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(m_PhysicalDevice); }
            VkFormat findSupportedFormat(
                const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

            // Buffer Helper Functions
            void createBuffer(
                VkDeviceSize size,
                VkBufferUsageFlags usage,
                VkMemoryPropertyFlags properties,
                VkBuffer& buffer,
                VkDeviceMemory& bufferMemory);
            VkCommandBuffer beginSingleTimeCommands();
            void endSingleTimeCommands(VkCommandBuffer commandBuffer);
            void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
            void copyBufferToImage(
                VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

            void createImageWithInfo(
                const VkImageCreateInfo& imageInfo,
                VkMemoryPropertyFlags properties,
                VkImage& image,
                VkDeviceMemory& imageMemory);

            VkPhysicalDeviceProperties properties;

        private:
            void createInstance();
            void setupDebugMessenger();
            void createSurface();
            void pickPhysicalDevice();
            void createLogicalDevice();
            void createCommandPool();

            // helper functions
            bool isDeviceSuitable(VkPhysicalDevice device);
            std::vector<const char*> getRequiredExtensions();
            bool checkValidationLayerSupport();
            QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
            void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
            void hasSDLRequiredInstanceExtensions();
            bool checkDeviceExtensionSupport(VkPhysicalDevice device);
            SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

            VkInstance m_Instance;
            VkDebugUtilsMessengerEXT m_DebugMessenger;
            VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
            Lavendel::RenderAPI::Window& m_Window;
            VkCommandPool m_CommandPool;

            VkDevice m_Device;
            VkSurfaceKHR m_Surface;
            VkQueue m_GraphicsQueue;
            VkQueue m_PresentQueue;

            const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

            #ifdef LV_PLATFORM_OSX
		            const std::vector<const char*> deviceExtensions = {
		                VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		                "VK_KHR_portability_subset"
                    };
            #else
		        const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
            #endif
        };
	}   // namespace RendererAPI

}  // namespace Lavendel