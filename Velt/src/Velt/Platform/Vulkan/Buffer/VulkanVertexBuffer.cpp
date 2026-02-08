#include "Core/Core.h"
#include "VulkanVertexBuffer.h"
#include "../VulkanContext.h"
#include "Core/Log.h"

namespace Velt::RHI
{
	VulkanVertexBuffer::VulkanVertexBuffer(void* data, u64 size, u64 offset, bool autoupload)
		: m_UploadSize(size), m_Size(size)
	{
		VT_PROFILE_FUNCTION();

		auto& device = VulkanContext::GetDevice();

		device.createBuffer(
			device.device(),
			size,
			VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_VertexBuffer,
			m_VertexBufferMemory
		);

		SetData(data, size, offset);


		// this probably is bad practice because I build the resource loader to batch uploads 
		// eg. 1 cmd buffer 2++ uploads
		if (autoupload)
		{
			auto& uploader = VulkanContext::GetResourceUploader();

			uploader.Begin();
			Upload(uploader.GetCommandBuffer());
			uploader.End();
		}
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		VT_PROFILE_FUNCTION();

		auto device = VulkanContext::GetDevice();

		if (m_StagingBuffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(device.device(), m_StagingBuffer, nullptr);
			vkFreeMemory(device.device(), m_StagingBufferMemory, nullptr);
		}

		if (m_VertexBuffer != VK_NULL_HANDLE)
		{
			vkDestroyBuffer(device.device(), m_VertexBuffer, nullptr);
			vkFreeMemory(device.device(), m_VertexBufferMemory, nullptr);
		}
	}

	void VulkanVertexBuffer::SetData(void* data, u64 size, u64 offset)
	{
		auto& device = VulkanContext::GetDevice();

		if (m_StagingBuffer == VK_NULL_HANDLE || m_StagingBufferSize < size) {
			if (m_StagingBuffer != VK_NULL_HANDLE) {

				vkDestroyBuffer(device.device(), m_StagingBuffer, nullptr);
				vkFreeMemory(device.device(), m_StagingBufferMemory, nullptr);

			}

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

		void* mapped = nullptr;

		// If I understood correctly, vkMapMemory, well, maps a memory region in the HOST_VISIBLE Vulkan cpu address-space 
		// the CPU can access and use for a staging buffer, then it writes the source data (data) into the staging buffer
		// I hope I understood correctly
		vkMapMemory(device.device(), m_StagingBufferMemory, 0, size, 0, &mapped);
		memcpy(mapped, data, (size_t)size);
		vkUnmapMemory(device.device(), m_StagingBufferMemory);
	}

	void VulkanVertexBuffer::Upload(VkCommandBuffer commandBuffer)
	{
		VT_PROFILE_FUNCTION();

		VT_CORE_TRACE("Upload Vertexbuffer to GPU");
		VT_CORE_ASSERT(m_StagingBuffer != VK_NULL_HANDLE, "No staging buffer set!");

		VkBufferCopy copyRegion{};
		copyRegion.srcOffset = 0;
		copyRegion.dstOffset = m_Offset;
		copyRegion.size = m_UploadSize;

		vkCmdCopyBuffer(
			commandBuffer,
			m_StagingBuffer,
			m_VertexBuffer,
			1,
			&copyRegion
		);
	}


}
