#include "Core/Core.h"
#include "VulkanIndexBuffer.h"
#include "VulkanContext.h"
#include <vulkan/vulkan.h>
#include "Core/Log.h"

namespace Velt::Renderer::Vulkan
{
	VulkanIndexBuffer::VulkanIndexBuffer(u64 size)
		: m_Size(size)
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("VulkanIndexBuffer constructed (uninitialized, size: {0})", size);
		// CreateBuffer(nullptr, size);
	}

	VulkanIndexBuffer::VulkanIndexBuffer(void* data, u64 size)
		: m_Size(size)
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("VulkanIndexBuffer constructed (with data, size: {0})", size);
		VT_CORE_ASSERT(data != nullptr, "Index buffer data is null!");
		CreateBuffer(data, size);
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("VulkanIndexBuffer destructed");

		auto device = VulkanContext::GetDevice();

		if (m_IndexBuffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(device->device(), m_IndexBuffer, nullptr);
			m_IndexBuffer = VK_NULL_HANDLE;
		}

		if (m_IndexBufferMemory != VK_NULL_HANDLE)
		{
			vkFreeMemory(device->device(), m_IndexBufferMemory, nullptr);
			m_IndexBufferMemory = VK_NULL_HANDLE;
		}
	}

	void VulkanIndexBuffer::Bind() const
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Bind VulkanIndexBuffer");

		VT_CORE_WARN("VulkanIndexBuffer:: Bind() called - binding should be done via vkCmdBindIndexBuffer in render pass");
	}

	void VulkanIndexBuffer::SetData(void* data, u64 size, u64 offset)
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("VulkanIndexBuffer::SetData (size: {0}, offset: {1})", size, offset);
		VT_CORE_ASSERT(data != nullptr, "Index buffer data is null!");
		VT_CORE_ASSERT(offset + size <= m_Size, "Data size exceeds buffer size!");

		auto device = VulkanContext::GetDevice();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		device->createBuffer(
			size,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer,
			stagingBufferMemory
		);

		void* mappedData = nullptr;
		vkMapMemory(device->device(), stagingBufferMemory, 0, size, 0, &mappedData);
		memcpy(mappedData, data, static_cast<size_t>(size));
		vkUnmapMemory(device->device(), stagingBufferMemory);

		VkCommandBuffer commandBuffer = device->beginSingleTimeCommands();

		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0;
		copyRegion.dstOffset = offset;
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, stagingBuffer, m_IndexBuffer, 1, &copyRegion);

		device->endSingleTimeCommands(commandBuffer);

		vkDestroyBuffer(device->device(), stagingBuffer, nullptr);
		vkFreeMemory(device->device(), stagingBufferMemory, nullptr);
	}

	void VulkanIndexBuffer::CreateBuffer(void* data, u64 size)
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("VulkanIndexBuffer::CreateBuffer (size: {0})", size);

		auto device = VulkanContext::GetDevice();

		device->createBuffer(
			size,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_IndexBuffer,
			m_IndexBufferMemory
		);

		if (data != nullptr)
		{
			SetData(data, size, 0);
		}
	}

	void VulkanIndexBuffer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, u64 size)
	{
		VT_PROFILE_FUNCTION();

		auto device = VulkanContext::GetDevice();
		VkCommandBuffer commandBuffer = device->beginSingleTimeCommands();


		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0;
		copyRegion.dstOffset = 0;
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		device->endSingleTimeCommands(commandBuffer);
	}
}