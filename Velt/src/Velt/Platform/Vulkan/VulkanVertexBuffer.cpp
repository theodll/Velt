#include "vtpch.h"
#include "VulkanVertexBuffer.h"
#include "Velt/Platform/Vulkan/VulkanContext.h"

namespace Velt::Renderer::Vulkan
{
    VulkanVertexBuffer::VulkanVertexBuffer(
        const void* vertexData, // data
        u32 vertexCount, // count 
        VkDeviceSize vertexStride // size 
    ) :   m_VertexCount(vertexCount),
          m_VertexStride(vertexStride)
    {
        VT_PROFILE_FUNCTION();
        VT_CORE_TRACE("Creating Vulkan Vertex Buffer");

        VT_CORE_ASSERT(vertexData, "Vertex data must not be null");
        VT_CORE_ASSERT(vertexCount > 0, "Vertex count must be > 0");
        VT_CORE_ASSERT(vertexStride > 0, "Vertex stride must be > 0");

        CreateBuffer(vertexData);
    }

    VulkanVertexBuffer::~VulkanVertexBuffer()
    {
        VT_PROFILE_FUNCTION();
        VT_CORE_TRACE("Destroying Vulkan Vertex Buffer");

        if (m_VertexBuffer != VK_NULL_HANDLE)
        {
            vkDestroyBuffer(*m_Device->device(), m_VertexBuffer, nullptr);
            m_VertexBuffer = VK_NULL_HANDLE;
        }

        if (m_VertexBufferMemory != VK_NULL_HANDLE)
        {
            vkFreeMemory(*m_Device->device(), m_VertexBufferMemory, nullptr);
            m_VertexBufferMemory = VK_NULL_HANDLE;
        }
    }

    void VulkanVertexBuffer::CreateBuffer(const void* data)
    {
        VkDeviceSize bufferSize = m_VertexStride * m_VertexCount;

        

        m_Device->createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            m_VertexBuffer,
            m_VertexBufferMemory
        );

        void* mappedData = nullptr;
        vkMapMemory(
            *m_Device->device(),
            m_VertexBufferMemory,
            0,
            bufferSize,
            0,
            &mappedData
        );

        memcpy(mappedData, data, static_cast<size_t>(bufferSize));
        vkUnmapMemory(*m_Device->device(), m_VertexBufferMemory);
    }

    void VulkanVertexBuffer::Bind() const
    {

    }

    void VulkanVertexBuffer::Unbind() const
    {
 
    }
}
