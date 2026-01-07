#pragma once

#include "Velt/Core/Core.h"
#include "Velt/Renderer/VertexBuffer.h"

#include "VulkanContext.h"
#include "VulkanDevice.h"
#include "Renderer/Buffer.h"

#include <vulkan/vulkan.h>

namespace Velt::Renderer::Vulkan
{
	class VulkanVertexBuffer
	{
	public:
		VulkanVertexBuffer(void* data, u64 size, u64 offset);
		~VulkanVertexBuffer();

		void SetData(void* data, u64 size, u64 offset = 0);
		void Upload(VkCommandBuffer commandBuffer);

		VkBuffer GetBuffer() const { return m_VertexBuffer; }

	private:
		VkBuffer       m_VertexBuffer = VK_NULL_HANDLE;
		VkDeviceMemory m_VertexBufferMemory = VK_NULL_HANDLE;

		VkBuffer       m_StagingBuffer = VK_NULL_HANDLE;
		VkDeviceMemory m_StagingBufferMemory = VK_NULL_HANDLE;

		u64 m_Size = 0;  // GPU buffer capacity
		u64 m_UploadSize = 0;  // size of current upload
		u64 m_StagingBufferSize = 0;  // staging buffer capacity
		u64 m_Offset = 0;  // offset in GPU buffer
	};
}
