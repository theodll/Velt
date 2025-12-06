#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "vtpch.h"
#include "VulkanDevice.h"

namespace Velt::Renderer::Vulkan
{
    class VELT_API Model
    {

    public:
        struct Vertex
        {
            glm::vec2 position;
            glm::vec3 color;

            static std::vector<VkVertexInputBindingDescription> getBindingDescription();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();
        };

        Model(VulkanDevice &device, const std::vector<Model::Vertex> &vertices);
        ~Model();

        Model(const Model &) = delete;
        Model &operator=(const Model &) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

    private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);

        VulkanDevice &m_Device;
        VkBuffer m_VertexBuffer;
        VkDeviceMemory m_VertexBufferMemory;
        uint32_t m_VertexCount;
    };
}
