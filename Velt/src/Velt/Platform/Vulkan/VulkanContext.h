#pragma once
#include "vtpch.h"
#include "Velt/Core/Core.h"
#include "Velt/Renderer/RenderContext.h"

namespace Velt::Renderer::Vulkan
{

	class VELT_API VulkanContext : public Renderer::RendererContext
	{
	public:
		VulkanContext() = default;
		virtual ~VulkanContext() = default;
		virtual void Init() override;
		static std::weak_ptr<Vulkan::VulkanDevice> getDevice() { return s_Device; }

	private:
		static VulkanDevice s_Device;
	};
}
