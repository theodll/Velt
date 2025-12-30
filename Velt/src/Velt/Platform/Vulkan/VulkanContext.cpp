#include "vtpch.h"
#include "VulkanContext.h"

namespace Velt::Renderer::Vulkan
{
	VulkanDevice* VulkanContext::s_Device = nullptr;
	
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

}