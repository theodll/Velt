#pragma once

#include "Velt/Core/Core.h"
#include "Velt/Renderer/VertexBuffer.h"

#include "../VulkanContext.h"
#include "../VulkanDevice.h"
#include "Renderer/Buffer.h"

#include <vulkan/vulkan.h>

namespace Velt::Renderer::RHI
{
	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(void* data, u64 size, u64 offset, bool autoupload = false);
		~VulkanVertexBuffer();

		virtual void SetData(void* data, u64 size, u64 offset = 0) override;
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; };

		virtual void Upload(VkCommandBuffer commandBuffer) override;

		VkBuffer GetVulkanBuffer() const { return m_VertexBuffer; }
		BufferLayout GetLayout() const override { return m_Layout; };

	private:
		VkBuffer       m_VertexBuffer = VK_NULL_HANDLE;
		VkDeviceMemory m_VertexBufferMemory = VK_NULL_HANDLE;

		VkBuffer       m_StagingBuffer = VK_NULL_HANDLE;
		VkDeviceMemory m_StagingBufferMemory = VK_NULL_HANDLE;

		BufferLayout m_Layout; 

		u64 m_Size = 0;  // GPU buffer capacity
		u64 m_UploadSize = 0;  // size of current upload
		u64 m_StagingBufferSize = 0;  // staging buffer capacity
		u64 m_Offset = 0;  // offset in GPU buffer
	};
}
