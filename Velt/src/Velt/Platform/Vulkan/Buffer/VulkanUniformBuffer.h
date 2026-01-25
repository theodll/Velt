#pragma once

#include "Velt/Core/Core.h"
#include "Velt/Renderer/UniformBuffer.h"   // dein Interface
#include "../VulkanContext.h"
#include "../VulkanDevice.h"

#include <vulkan/vulkan.h>

namespace Velt::Renderer::RHI
{
	class VulkanUniformBuffer : public UniformBuffer
	{
	public:
		VulkanUniformBuffer(u64 size, bool useStaging = false, u64 offset = 0);
		~VulkanUniformBuffer();

		virtual void SetData(const void* data, u64 size, u64 offset = 0) override;

		virtual void Upload(VkCommandBuffer commandBuffer) override;

		VkBuffer GetVulkanBuffer() const override { return m_UniformBuffer; }
		u64      GetSize() const override { return m_Size; }

	private:
		void CreateOrResizeStaging(u64 size);
		void DestroyStaging();

	private:
		VkBuffer       m_UniformBuffer = VK_NULL_HANDLE;
		VkDeviceMemory m_UniformBufferMemory = VK_NULL_HANDLE;

		VkBuffer       m_StagingBuffer = VK_NULL_HANDLE;
		VkDeviceMemory m_StagingBufferMemory = VK_NULL_HANDLE;

		void* m_Mapped = nullptr;

		bool m_UseStaging = false;

		u64 m_Size = 0;               // UBO capacity
		u64 m_UploadSize = 0;         // size of current update
		u64 m_StagingBufferSize = 0;  // staging capacity
		u64 m_Offset = 0;             // dst offset in UBO
	};
}
