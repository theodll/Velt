#pragma once

#include "Velt/Core/Core.h"
#include "Velt/Renderer/VertexBuffer.h"

#include "VulkanContext.h"
#include "VulkanDevice.h"
#include "Renderer/Buffer.h"

#include <vulkan/vulkan.h>

namespace Velt::Renderer::Vulkan
{
    class VulkanVertexBuffer : public VertexBuffer
    {
    public:
        VulkanVertexBuffer(
            const void* vertexData,
            u32 vertexCount,
            VkDeviceSize vertexStride
        );

        // virtual void setLayout(const BufferLayout& layout) override {m_Layout = layout;};
        //virtual BufferLayout getLayout() const override {return m_Layout;};

        virtual ~VulkanVertexBuffer() override;

        virtual void Bind() const override;
        virtual void Unbind() const override;


        u32 GetVertexCount() const { return m_VertexCount; }

    private:
        void CreateBuffer(const void* data);

    private:
        VulkanDevice* m_Device = VulkanContext::GetDevice();        

        VkBuffer m_VertexBuffer = VK_NULL_HANDLE;
        VkDeviceMemory m_VertexBufferMemory = VK_NULL_HANDLE;

        // BufferLayout m_Layout;

        u32 m_VertexCount = 0;
        VkDeviceSize m_VertexStride = 0;
    };
}
