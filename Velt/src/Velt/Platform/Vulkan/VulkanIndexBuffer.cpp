#include "Core/Core.h"
#include "VulkanIndexBuffer.h"
#include "VulkanContext.h"
#include "Core/Log.h"

namespace Velt::Renderer::Vulkan
{
	VulkanIndexBuffer::VulkanIndexBuffer(u64 size)
		: m_Size(size)
	{
		VT_PROFILE_FUNCTION();

		auto device = VulkanContext::GetDevice();

		// Erstelle nur den GPU-Buffer (device-local)
		device.createBuffer(
			size,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_IndexBuffer,
			m_IndexBufferMemory
		);
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		VT_PROFILE_FUNCTION();

		auto device = VulkanContext::GetDevice();

		// Staging-Buffer aufräumen
		if (m_StagingBuffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(device.device(), m_StagingBuffer, nullptr);
			vkFreeMemory(device.device(), m_StagingBufferMemory, nullptr);
		}

		// GPU-Buffer aufräumen
		if (m_IndexBuffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(device.device(), m_IndexBuffer, nullptr);
			vkFreeMemory(device.device(), m_IndexBufferMemory, nullptr);
		}
	}

	void VulkanIndexBuffer::SetData(void* data, u64 size, u64 offset)
	{
		VT_PROFILE_FUNCTION();

		VT_CORE_ASSERT(data != nullptr, "Index buffer data is null!");
		VT_CORE_ASSERT(offset + size <= m_Size, "Upload exceeds index buffer size!");

		auto device = VulkanContext::GetDevice();

		m_UploadSize = size;
		m_Offset = offset;

		// Bestehenden Staging-Buffer löschen, falls vorhanden
		if (m_StagingBuffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(device.device(), m_StagingBuffer, nullptr);
			vkFreeMemory(device.device(), m_StagingBufferMemory, nullptr);
		}

		// Staging-Buffer erstellen
		device.createBuffer(
			size,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			m_StagingBuffer,
			m_StagingBufferMemory
		);

		// Daten kopieren
		void* mapped = nullptr;
		vkMapMemory(device.device(), m_StagingBufferMemory, 0, size, 0, &mapped);
		memcpy(mapped, data, static_cast<size_t>(size));
		vkUnmapMemory(device.device(), m_StagingBufferMemory);
	}

	void VulkanIndexBuffer::Upload(VkCommandBuffer commandBuffer)
	{
		VT_PROFILE_FUNCTION();

		VT_CORE_ASSERT(m_StagingBuffer != VK_NULL_HANDLE, "No staging buffer set!");

		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0;
		copyRegion.dstOffset = m_Offset;
		copyRegion.size = m_UploadSize;

		vkCmdCopyBuffer(
			commandBuffer,
			m_StagingBuffer,
			m_IndexBuffer,
			1,
			&copyRegion
		);
	}
}
