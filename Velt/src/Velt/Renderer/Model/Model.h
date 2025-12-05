#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE


#include "vtpch.h"
#include "../Core/Device.h"




namespace  Velt
{
    namespace RenderAPI
    {
        class Velt_API Model
        {


        public:

            struct Veltex
            {
                glm::vec2 position;
				glm::vec3 color;

                static std::vector<VkVeltexInputBindingDescription> getBindingDescription();
                static std::vector<VkVeltexInputAttributeDescription> getAttributeDescription();
            };

            Model(GPUDevice &device, const std::vector<Model::Veltex> &vertices);
            ~Model();

            Model(const Model &) = delete;
            Model &operator=(const Model &) = delete;

            void bind(VkCommandBuffer commandBuffer);
            void draw(VkCommandBuffer commandBuffer);

        private:
            void createVeltexBuffers(const std::vector<Veltex> &vertices);


            GPUDevice &m_Device;
            VkBuffer m_VeltexBuffer;
            VkDeviceMemory m_VeltexBufferMemory;
            uint32_t m_VeltexCount;
        };
    }
}

