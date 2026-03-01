#pragma once

#include "vtpch.h"
#include "Velt/Core/Core.h"

namespace Velt::RHI {

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

	class VELT_API VulkanDevice
	{
	public:
#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

		VulkanDevice();
		~VulkanDevice();


		VkCommandPool GetCommandPool() { return m_CommandPool; }
		VkDevice& device() { return m_Device; }
		VkSurfaceKHR surface() { return m_Surface; }
		VkQueue graphicsQueue() { return m_GraphicsQueue; }
		VkQueue presentQueue() { return m_PresentQueue; }

		VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }
		VkQueue GetGraphicsQueue() { return m_GraphicsQueue; }
		u32 GetQueueFamilyIndex() { return FindPhysicalQueueFamilies().graphicsFamily; }
		
		SwapChainSupportDetails GetSwapChainSupport() { return QuerySwapChainSupport(m_PhysicalDevice); }
		u32 FindMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties);
		QueueFamilyIndices FindPhysicalQueueFamilies() { return FindQueueFamilies(m_PhysicalDevice); }
		VkFormat FindSupportedFormat(
			const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

		// Buffer Helper Functions
		void CreateBuffer(
			VkDevice& device,
			VkDeviceSize size,
			VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties,
			VkBuffer& buffer,
			VkDeviceMemory& bufferMemory);

		VkCommandBuffer BeginSingleTimeCommands();
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void CopyBufferToImage(
			VkBuffer buffer, VkImage image, u32 width, u32 height, u32 layerCount);

		void CreateImageWithInfo(
			const VkImageCreateInfo& imageInfo,
			VkMemoryPropertyFlags properties,
			VkImage& image,
			VkDeviceMemory& imageMemory);

		VkPhysicalDeviceProperties properties;

		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
       

	private:
		void PickPhysicalDevice();
		void CreateLogicalDevice();
		void CreateCommandPool();

		// helper functions
		bool IsDeviceSuitable(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		
		VkInstance& m_Instance;

		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkCommandPool m_CommandPool;

		VkDevice m_Device;
		VkSurfaceKHR& m_Surface;
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;

		
#ifdef VT_PLATFORM_OSX
		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			"VK_KHR_portability_subset"
		};
#else
		const std::vector<const char*> deviceExtensions = { 
			VK_KHR_SWAPCHAIN_EXTENSION_NAME, 
		 };
#endif
	};
}  // namespace Velt