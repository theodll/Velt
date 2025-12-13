#pragma once

#include "vtpch.h"
#include "Velt/Core/Core.h"
#include "Velt/Renderer/Window.h"
#include "Velt/Renderer/Device.h"
#include "VulkanWindow.h"

namespace Velt::Renderer::Vulkan {

        class VELT_API Window;

        struct SwapChainSupportDetails
        {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };

        struct QueueFamilyIndices
        {
            u32 graphicsFamily;
            u32 presentFamily;
            bool graphicsFamilyHasValue = false;
            bool presentFamilyHasValue = false;
            bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
        };

		class VELT_API VulkanDevice : public Device
        {
        public:
        #ifdef NDEBUG
            const bool enableValidationLayers = false;
        #else
            const bool enableValidationLayers = true;
        #endif

            VulkanDevice(RenderAPI::Window& window);
            ~VulkanDevice();

            VulkanDevice(const VulkanDevice&) = delete;
            void operator=(const VulkanDevice&) = delete;
            VulkanDevice(VulkanDevice&&) = delete;
            VulkanDevice& operator=(VulkanDevice&&) = delete;

            virtual void* GetNativeDevice() override { return device(); }
            virtual void* GetPhysicalDevice() override { return getPhysicalDevice(); }
            virtual void* GetCommandPool() override { return getCommandPool(); }
            virtual void* GetGraphicsQueue() override { return getGraphicsQueue(); }
            virtual void* GetPresentQueue() override { return presentQueue(); }
            virtual void* GetInstance() override { return getInstance(); }

            virtual void CreateBuffer(size_t size, u32 usage, u32 properties, void** buffer, void** bufferMemory) override;
            virtual void* BeginSingleTimeCommands() override { return beginSingleTimeCommands(); }
            virtual void EndSingleTimeCommands(void* commandBuffer) override { endSingleTimeCommands(static_cast<VkCommandBuffer>(commandBuffer)); }
            virtual void CopyBuffer(void* srcBuffer, void* dstBuffer, size_t size) override { copyBuffer(static_cast<VkBuffer>(srcBuffer), static_cast<VkBuffer>(dstBuffer), size); }
            virtual u32 FindMemoryType(u32 typeFilter, u32 properties) override { return findMemoryType(typeFilter, properties); }

            VkCommandPool getCommandPool() { return m_CommandPool; }
            VkDevice* device() { return m_Device; }
            VkSurfaceKHR surface() { return m_Surface; }
            VkQueue graphicsQueue() { return m_GraphicsQueue; }
            VkQueue presentQueue() { return m_PresentQueue; }
            VkInstance getInstance() { return m_Instance; }
            VkPhysicalDevice getPhysicalDevice() { return m_PhysicalDevice; }
            VkQueue getGraphicsQueue() { return m_GraphicsQueue; }
            u32 getQueueFamilyIndex() { return findPhysicalQueueFamilies().graphicsFamily; }
            RenderAPI::Window& getWindow() { return m_Window; }
            
            SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(m_PhysicalDevice); }
            u32 findMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties);
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
                VkBuffer buffer, VkImage image, u32 width, u32 height, u32 layerCount);

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
            Velt::RenderAPI::Window& m_Window;
            VulkanWindow m_VulkanWindow;
            VkCommandPool m_CommandPool;

            VkDevice* m_Device;
            VkSurfaceKHR m_Surface;
            VkQueue m_GraphicsQueue;
            VkQueue m_PresentQueue;

            const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

            #ifdef VT_PLATFORM_OSX
		            const std::vector<const char*> deviceExtensions = {
		                VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		                "VK_KHR_portability_subset"
                    };
            #else
		        const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
            #endif
        };
}  // namespace Velt