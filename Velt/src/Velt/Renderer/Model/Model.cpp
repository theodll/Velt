#include "vtpch.h"
#include "Model.h"

namespace Velt
{
    namespace RenderAPI
    {
		Model::Model(GPUDevice& device, const std::vector<Model::Veltex>& vertices) : m_Device{ device }
        {
            VT_PROFILE_FUNCTION();
            VT_CORE_INFO("Creating Model...");
            createVeltexBuffers(vertices);
        }
        Model::~Model()
        {
            VT_PROFILE_FUNCTION();
		    vkDeviceWaitIdle(m_Device.device());
            vkDestroyBuffer(m_Device.device(), m_VeltexBuffer, nullptr);
            vkFreeMemory(m_Device.device(), m_VeltexBufferMemory, nullptr);
		}

        void Model::createVeltexBuffers(const std::vector<Model::Veltex>& vertecies)
        {
            VT_PROFILE_FUNCTION();
            m_VeltexCount = static_cast<uint32_t>(vertecies.size());

            if (m_VeltexCount < 3)
            {
                VT_CORE_ERROR("Veltex Count must be at least 3");
            }
            assert(m_VeltexCount >= 3 && "Veltex count must be at least 3");

            VkDeviceSize bufferSize = sizeof(vertecies[0]) * m_VeltexCount;
            m_Device.createBuffer(
                bufferSize,
                VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                m_VeltexBuffer,
                m_VeltexBufferMemory
            );

            void* data;
            vkMapMemory(m_Device.device(), m_VeltexBufferMemory, 0, bufferSize, 0, &data);
            memcpy(data, vertecies.data(), static_cast<size_t>(bufferSize));
            vkUnmapMemory(m_Device.device(), m_VeltexBufferMemory);
        };

        void Model::draw(VkCommandBuffer commandBuffer)
        {
            VT_PROFILE_FUNCTION();
            vkCmdDraw(commandBuffer, m_VeltexCount, 1, 0, 0);
		}
        
        void Model::bind(VkCommandBuffer commandBuffer)
        {
            VT_PROFILE_FUNCTION();
			VkBuffer buffers[] = { m_VeltexBuffer };
            VkDeviceSize offsets[] = { 0 };
			vkCmdBindVeltexBuffers(commandBuffer, 0, 1, buffers, offsets);
        }

        std::vector<VkVeltexInputBindingDescription> Model::Veltex::getBindingDescription()
        {
            VT_PROFILE_FUNCTION();
            std::vector<VkVeltexInputBindingDescription> bindingDescriptions(1);
            bindingDescriptions[0].binding = 0;
            bindingDescriptions[0].stride = sizeof(Veltex);
            bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return bindingDescriptions;
		}

        std::vector<VkVeltexInputAttributeDescription> Model::Veltex::getAttributeDescription()
        {
            VT_PROFILE_FUNCTION();
            std::vector<VkVeltexInputAttributeDescription> attributeDescriptions(2);

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Veltex, position);

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Veltex, color);

            return attributeDescriptions;
        }
        
    }
}