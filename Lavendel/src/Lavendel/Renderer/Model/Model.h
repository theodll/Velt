#pragma once
#include "lvpch.h"
#include "../Core/Device.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE



namespace  Lavendel
{
    namespace RenderAPI
    {
        class LAVENDEL_API Model
        {


        public:

            struct Vertex
            {
                glm::vec2 position;
				glm::vec3 color;

                static std::vector<VkVertexInputBindingDescription> getBindingDescription();
                static std::vector<VkVertexInputAttributeDescription> getAttributeDescription();
            };

            Model(GPUDevice &device, const std::vector<Model::Vertex> &vertices);
            ~Model();

            Model(const Model &) = delete;
            Model &operator=(const Model &) = delete;

            void bind(VkCommandBuffer commandBuffer);
            void draw(VkCommandBuffer commandBuffer);

        private:
            void createVertexBuffers(const std::vector<Vertex> &vertices);


            GPUDevice &m_Device;
            VkBuffer m_VertexBuffer;
            VkDeviceMemory m_VertexBufferMemory;
            uint32_t m_VertexCount;
        };
    }
}

