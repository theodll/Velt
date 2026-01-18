#pragma once
#include "Core/Core.h"
#include "Renderer/IndexBuffer.h"
#include <vulkan/vulkan.h>

namespace Velt::Renderer::Vulkan
{
	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(u64 size);
		VulkanIndexBuffer(void* data, u64 size, u64 offset);
		~VulkanIndexBuffer();

		virtual void SetData(void* data, u64 size, u64 offset = 0) override;
		virtual void Upload(VkCommandBuffer commandBuffer) override;

		virtual u32 GetCount() const override { return m_Count; }

		virtual VkBuffer GetVulkanBuffer() const override { return m_IndexBuffer; }

	private:
		VkBuffer       m_IndexBuffer = VK_NULL_HANDLE;
		VkDeviceMemory m_IndexBufferMemory = VK_NULL_HANDLE;

		VkBuffer       m_StagingBuffer = VK_NULL_HANDLE;
		VkDeviceMemory m_StagingBufferMemory = VK_NULL_HANDLE;

		u32 m_Count = 0; 
		u64 m_Size = 0;  // GPU buffer capacity
		u64 m_UploadBytes = 0;  // bytes of current upload
		u64 m_SizeBytes = 0;
		u64 m_StagingBufferSize = 0;  // staging buffer capacity
		u64 m_Offset = 0;  // offset in GPU buffer
	};
}
