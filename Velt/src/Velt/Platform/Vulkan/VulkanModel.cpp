#if COMPILE_GARABAGE

#include "vtpch.h"
#include "VulkanModel.h"

namespace Velt::Renderer::Vulkan
{
		Model::Model(VulkanDevice& device, const std::vector<Model::Vertex>& vertices) : m_Device{ device }
        {
            VT_PROFILE_FUNCTION();
            VT_CORE_INFO("Creating Model...");
            createVertexBuffers(vertices);
        }
        Model::~Model()
        {
            VT_PROFILE_FUNCTION();
		    vkDeviceWaitIdle(m_Device.device());
            vkDestroyBuffer(m_Device.device(), m_VertexBuffer, nullptr);
            vkFreeMemory(m_Device.device(), m_VertexBufferMemory, nullptr);
		}

        void Model::createVertexBuffers(const std::vector<Model::Vertex>& vertecies)
        {
            VT_PROFILE_FUNCTION();
            m_VertexCount = static_cast<u32>(vertecies.size());

            if (m_VertexCount < 3)
            {
                VT_CORE_ERROR("Vertex Count must be at least 3");
            }
            VT_CORE_ASSERT(m_VertexCount >= 3, "Vertex count must be at least 3");

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

        void Model::draw(VkCommandBuffer commandBuffer)
        {
            VT_PROFILE_FUNCTION();
            vkCmdDraw(commandBuffer, m_VertexCount, 1, 0, 0);
		}
        
        void Model::bind(VkCommandBuffer commandBuffer)
        {
            VT_PROFILE_FUNCTION();
			VkBuffer buffers[] = { m_VertexBuffer };
            VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
        }

        std::vector<VkVertexInputBindingDescription> Model::Vertex::getBindingDescription()
        {
            VT_PROFILE_FUNCTION();
            std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
            bindingDescriptions[0].binding = 0;
            bindingDescriptions[0].stride = sizeof(Vertex);
            bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return bindingDescriptions;
		}

        std::vector<VkVertexInputAttributeDescription> Model::Vertex::getAttributeDescription()
        {
            VT_PROFILE_FUNCTION();
            std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, position);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);

            return attributeDescriptions;
        }
}

#endif