#pragma once
#include "vtpch.h"
#include <vulkan/vulkan.h>
#include "Velt/Core/Core.h"
#include "Velt/Platform/Vulkan/VulkanDevice.h"
#include "Velt/Renderer/RenderContext.h"
#include "Velt/Platform/Vulkan/VulkanSwapchain.h"
#include "Velt/Platform/Vulkan/VulkanPipeline.h"
#include "Velt/Platform/Vulkan/VulkanRenderer.h"


namespace Velt::Renderer::Vulkan
{

	class VELT_API VulkanContext : public Context
	{
	public:
		VulkanContext();
		virtual ~VulkanContext() = default;
		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void DrawFrame() override;

		void CreateInstance();
		
		static VulkanDevice& GetDevice() { return *m_Device; }
		static VkInstance& GetInstance() { return m_Instance; }
		static VkSurfaceKHR& GetSurface() { return m_Surface; }
		
		VulkanSwapchain* GetSwapchain() { return m_Swapchain; }


	private:
		bool m_EnableValidationLayers; 
		void SDLRequiredInstanceExtensions();
		std::vector<const char*> GetRequiredExtensions();
		bool CheckValidationLayerSupport();
		void CreateSurface();


		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void SetupDebugMessenger();
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);
		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

		VkDebugUtilsMessengerEXT m_DebugMessenger;
		static VkInstance m_Instance; 
		static VulkanDevice* m_Device;
		static VkSurfaceKHR m_Surface;
		VulkanSwapchain* m_Swapchain;
		VulkanRenderer* m_Renderer = nullptr;

		const std::vector<const char*> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };
		


		
	};
}
