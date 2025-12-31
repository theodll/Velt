#include "vtpch.h"
#include "VulkanContext.h"
#include <SDL3/SDL_vulkan.h>

namespace Velt::Renderer::Vulkan
{
	VulkanDevice* VulkanContext::s_Device = nullptr;
	
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		VT_PROFILE_FUNCTION();

		auto coreLogger = ::Velt::Log::GetCoreLogger();
		if (coreLogger && coreLogger.get() != nullptr)
		{
			VT_CORE_ERROR("validation layer: {}", pCallbackData->pMessage);
		}
		else
		{
			std::cerr << "[VULKAN VALIDATION]: " << pCallbackData->pMessage << std::endl;
		}

		return VK_FALSE;
	}

	VulkanContext::VulkanContext()
	{
		VT_PROFILE_FUNCTION();
		Init();
	}

	void VulkanContext::Init()
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Initializing Vulkan Context");
		s_Device = new VulkanDevice();
		s_Swapchain = new VulkanSwapchain();
	}

	void VulkanContext::Shutdown()
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Shutting down Vulkan Context");
		delete s_Swapchain;
		delete s_Device;
	}

	bool VulkanContext::CheckValidationLayerSupport()
	{
		VT_PROFILE_FUNCTION();
		u32 layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : m_ValidationLayers)
		{
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers)
			{
				if (strcmp(layerName, layerProperties.layerName) == 0)
				{
					layerFound = true;
					break;
				}
			}

			if (!layerFound)
			{
				return false;
			}
		}

		return true;
	}

	void VulkanContext::CreateInstance() 
	{
		VT_PROFILE_FUNCTION();
		if (m_EnableValidationLayers && !CheckValidationLayerSupport())
		{
			throw std::runtime_error("validation layers requested, but not available!");
			VT_CORE_ERROR("Validation layers requested, but not available!");
		}

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Velt App";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Velt";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;


#ifdef VT_PLATFORM_OSX
		createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif


		auto extensions = GetRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<u32>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
		if (m_EnableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<u32>(m_ValidationLayers.size());
			createInfo.ppEnabledLayerNames = m_ValidationLayers.data();

			PopulateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else
		{
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		if (vkCreateInstance(&createInfo, nullptr, &s_Instance) != VK_SUCCESS)
		{
			VT_CORE_ERROR("Failed to create Vulkan instance!");
		}

		SDLRequiredInstanceExtensions();
	}

	void PopulateDebugMessengerCreateInfo(
		VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		VT_PROFILE_FUNCTION();
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
		createInfo.pUserData = nullptr;  // Optional
	}

	bool VulkanDevice::isDeviceSuitable(VkPhysicalDevice device)
	{
		VT_PROFILE_FUNCTION();
		QueueFamilyIndices indices = findQueueFamilies(device);

		bool extensionsSupported = m_Device->CheckDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if (extensionsSupported)
		{
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

		return indices.isComplete() && extensionsSupported && swapChainAdequate &&
			supportedFeatures.samplerAnisotropy;
	}

	void VulkanDevice::setupDebugMessenger()
	{
		VT_PROFILE_FUNCTION();
		if (!enableValidationLayers) return;
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		populateDebugMessengerCreateInfo(createInfo);
		if (CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to set up debug messenger!");
			VT_CORE_ERROR("Failed to set up debug messenger!");
		}
	}

	void VulkanContext::SDLRequiredInstanceExtensions()
	{
		VT_PROFILE_FUNCTION();
		u32 extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		VT_CORE_INFO("Available Vulkan extensions: ");
		std::unordered_set<std::string> available;
		for (const auto& extension : extensions)
		{
			VT_CORE_INFO("\t {}", extension.extensionName);
			available.insert(extension.extensionName);
		}

		VT_CORE_INFO("Required Vulkan extensions: ");
		auto requiredExtensions = GetRequiredExtensions();
		for (const auto& required : requiredExtensions)
		{
			VT_CORE_INFO("\t {}", required);
			if (available.find(required) == available.end())
			{
				VT_CORE_ERROR("Missing required SDL extension: {}", required);
			}
		}
	}

	std::vector<const char*> VulkanContext::GetRequiredExtensions()
	{
		VT_PROFILE_FUNCTION();
		// Get SDL3 required Vulkan instance extensions
		u32 sdlExtensionCount = 0;
		const char* const* sdlExtensions = SDL_Vulkan_GetInstanceExtensions(&sdlExtensionCount);

		std::vector<const char*> extensions;
		if (sdlExtensions != nullptr && sdlExtensionCount > 0)
		{
			extensions.assign(sdlExtensions, sdlExtensions + sdlExtensionCount);
		}

#ifdef VT_PLATFORM_OSX
		extensions.push_back("VK_KHR_portability_enumeration");
#endif

		extensions.push_back("VK_KHR_get_physical_device_properties2");

		if (m_EnableValidationLayers)
		{
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}
}