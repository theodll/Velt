#pragma once
#include "vtpch.h"
#include <vulkan/vulkan.h>
#include "Velt/Core/Core.h"
#include "Velt/Platform/Vulkan/VulkanDevice.h"
#include "Velt/Renderer/RenderContext.h"
#include "Velt/Platform/Vulkan/VulkanSwapchain.h"
#include "Velt/Platform/Vulkan/VulkanPipeline.h"


namespace Velt::Renderer::Vulkan
{

	class VELT_API VulkanContext : public RendererContext
	{
	public:
		VulkanContext();
		virtual ~VulkanContext() = default;
		virtual void Init() override;
		virtual void Shutdown() override;

		void CreateInstance();
		static VulkanDevice* GetDevice() { return s_Device; }
		static VkInstance GetInstance() { return s_Instance; }

	private:
		bool m_EnableValidationLayers; 
		void SDLRequiredInstanceExtensions();
		std::vector<const char*> GetRequiredExtensions();
		bool CheckValidationLayerSupport();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		const std::vector<const char*> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };

		VkInstance m_Instance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		inline static VkInstance s_Instance; 
		static VulkanDevice* s_Device;
		VulkanSwapchain* s_Swapchain;
		
	};
}
