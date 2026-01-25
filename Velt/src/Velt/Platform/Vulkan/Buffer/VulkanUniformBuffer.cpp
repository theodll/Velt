#include "Core/Core.h"
#include "VulkanUniformBuffer.h"
#include "../VulkanContext.h"
#include "Core/Log.h"

#include <cstring>

namespace Velt::Renderer::RHI
{
	VulkanUniformBuffer::VulkanUniformBuffer(u64 size, bool useStaging, u64 offset)
		: m_UseStaging(useStaging), m_Size(size), m_Offset(offset)
	{
		VT_PROFILE_FUNCTION();

		auto& device = VulkanContext::GetDevice();

		if (m_UseStaging)
		{
			device.createBuffer(
				device.device(),
				size,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
				m_UniformBuffer,
				m_UniformBufferMemory
			);
		}
		else
		{
			device.createBuffer(
				device.device(),
				size,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				m_UniformBuffer,
				m_UniformBufferMemory
			);

			VkResult res = vkMapMemory(device.device(), m_UniformBufferMemory, 0, size, 0, &m_Mapped);
			VT_CORE_ASSERT(res == VK_SUCCESS && m_Mapped != nullptr, "Failed to map uniform buffer memory!");
		}
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
		VT_PROFILE_FUNCTION();

		auto device = VulkanContext::GetDevice();

		if (m_Mapped)
		{
			vkUnmapMemory(device.device(), m_UniformBufferMemory);
			m_Mapped = nullptr;
		}

		DestroyStaging();

		if (m_UniformBuffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(device.device(), m_UniformBuffer, nullptr);
			vkFreeMemory(device.device(), m_UniformBufferMemory, nullptr);
			m_UniformBuffer = VK_NULL_HANDLE;
			m_UniformBufferMemory = VK_NULL_HANDLE;
		}
	}

	void VulkanUniformBuffer::DestroyStaging()
	{
		auto device = VulkanContext::GetDevice();

		if (m_StagingBuffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(device.device(), m_StagingBuffer, nullptr);
			vkFreeMemory(device.device(), m_StagingBufferMemory, nullptr);
			m_StagingBuffer = VK_NULL_HANDLE;
			m_StagingBufferMemory = VK_NULL_HANDLE;
			m_StagingBufferSize = 0;
		}
	}

	void VulkanUniformBuffer::CreateOrResizeStaging(u64 size)
	{
		auto& device = VulkanContext::GetDevice();

		if (m_StagingBuffer == VK_NULL_HANDLE || m_StagingBufferSize < size)
		{
			DestroyStaging();

			device.createBuffer(
				device.device(),
				size,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				m_StagingBuffer,
				m_StagingBufferMemory
			);

			m_StagingBufferSize = size;
		}
	}

	void VulkanUniformBuffer::SetData(const void* data, u64 size, u64 offset)
	{
		VT_PROFILE_FUNCTION();

		VT_CORE_ASSERT(data != nullptr, "UniformBuffer::SetData - data is null");
		VT_CORE_ASSERT(size > 0, "UniformBuffer::SetData - size is 0");
		VT_CORE_ASSERT(offset + size <= m_Size, "UniformBuffer::SetData - write out of bounds");

		m_UploadSize = size;
		m_Offset = offset;

		auto& device = VulkanContext::GetDevice();

		if (!m_UseStaging)
		{
			memcpy(static_cast<std::byte*>(m_Mapped) + offset, data, static_cast<size_t>(size));

			return;
		}

		CreateOrResizeStaging(size);

		void* mapped = nullptr;
		VkResult res = vkMapMemory(device.device(), m_StagingBufferMemory, 0, size, 0, &mapped);
		VT_CORE_ASSERT(res == VK_SUCCESS && mapped != nullptr, "Failed to map staging buffer memory!");

		std::memcpy(mapped, data, static_cast<size_t>(size));
		vkUnmapMemory(device.device(), m_StagingBufferMemory);
	}

	void VulkanUniformBuffer::Upload(VkCommandBuffer commandBuffer)
	{
		VT_PROFILE_FUNCTION();

		if (!m_UseStaging)
			return; 

		VT_CORE_ASSERT(m_StagingBuffer != VK_NULL_HANDLE, "UniformBuffer Upload: No staging buffer set!");

		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0;
		copyRegion.dstOffset = m_Offset;
		copyRegion.size = m_UploadSize;

		vkCmdCopyBuffer(
			commandBuffer,
			m_StagingBuffer,
			m_UniformBuffer,
			1,
			&copyRegion
		);

	}
}
