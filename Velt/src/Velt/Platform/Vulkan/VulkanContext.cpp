#include "vtpch.h"
#include "VulkanContext.h"

namespace Velt::Renderer::Vulkan
{
	VulkanDevice* VulkanContext::s_Device = nullptr;
	VulkanSwapchain* VulkanContext::s_Swapchain = nullptr;
	VulkanPipeline* VulkanContext::s_Pipeline = nullptr;
	
	VulkanContext::VulkanContext()
	{
		Init();
	}

	void VulkanContext::Init()
	{
		VT_CORE_TRACE("Initializing Vulkan Context");
		s_Device = new VulkanDevice();
		s_Swapchain = new VulkanSwapchain(s_Device);
		s_Pipeline = new VulkanPipeline(s_Device, s_Swapchain);
	}

	void VulkanContext::Shutdown()
	{
		VT_CORE_TRACE("Shutting down Vulkan Context");
		delete s_Pipeline;
		delete s_Swapchain;
		delete s_Device;
	}
}