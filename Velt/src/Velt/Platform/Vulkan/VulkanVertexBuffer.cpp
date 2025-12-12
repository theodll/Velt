#include "vtpch.h"
#include "VulkanVertexBuffer.h"
#include "VulkanContext.h"
#include "VulkanDevice.h"

namespace Velt::Renderer::Vulkan
{
	

	VulkanVertexBuffer::VulkanVertexBuffer(VertexBufferElement& elements) : m_Elements(elements)
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Creating Vulkan Vertex Buffer");
		m_Device = VulkanContext::getDevice();
	
	}


	void VulkanVertexBuffer::CreateVertexBuffers()
	{
		VT_PROFILE_FUNCTION();
		m_VertexCount = static_cast<u32>(m_Elements.vertecies.size());

		if (m_VertexCount < 3)
		{
			VT_CORE_ERROR("Vertex Count must be at least 3");
		}
		assert(m_VertexCount >= 3 && "Vertex count must be at least 3");

		VkDeviceSize bufferSize = sizeof(vertecies[0]) * m_VertexCount;
		m_Device.createBuffer(
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			m_VertexBuffer,
			m_VertexBufferMemory
		);

		void* data;
		vkMapMemory(m_Device.device(), m_VertexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertecies.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(m_Device.device(), m_VertexBufferMemory);
	};


	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Destroying Vulkan Vertex Buffer");

	}
	void VulkanVertexBuffer::Bind() const
	{
		VT_PROFILE_FUNCTION();

	}
	void VulkanVertexBuffer::Unbind() const
	{
		VT_PROFILE_FUNCTION();

	}
}