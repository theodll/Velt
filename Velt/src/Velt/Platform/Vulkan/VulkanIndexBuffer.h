#pragma once
#include "Core/Core.h"
#include <vulkan/vulkan.h>

namespace Velt::Renderer::Vulkan
{
	class VulkanIndexBuffer
	{
	public:
		VulkanIndexBuffer(u64 size);
		~VulkanIndexBuffer();

		void SetData(void* data, u64 size, u64 offset = 0);
		void Upload(VkCommandBuffer commandBuffer);

		VkBuffer GetBuffer() const { return m_IndexBuffer; }

	private:
		VkBuffer       m_IndexBuffer = VK_NULL_HANDLE;
		VkDeviceMemory m_IndexBufferMemory = VK_NULL_HANDLE;

		VkBuffer       m_StagingBuffer = VK_NULL_HANDLE;
		VkDeviceMemory m_StagingBufferMemory = VK_NULL_HANDLE;

		u64 m_Size = 0;  // GPU buffer capacity
		u64 m_UploadSize = 0;  // size of current upload
		u64 m_StagingBufferSize = 0;  // staging buffer capacity
		u64 m_Offset = 0;  // offset in GPU buffer
	};
}
