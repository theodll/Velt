#pragma once
#include "Velt/Core/Core.h"
#include "Velt/Renderer/VertexBuffer.h"
#include "VulkanDevice.h"
#include <vulkan/vulkan.h>

namespace Velt::Renderer::Vulkan
 {


    class VulkanVertexBuffer : public VertexBuffer
    {
    public:

        VulkanVertexBuffer(VertexBufferElement& elements);

        virtual ~VulkanVertexBuffer() {}

        void CreateVertexBuffer();
        void CleanUp();

        virtual void Bind() const;
        virtual void Unbind() const;

    private:
        u32 m_RendererID{};
        const VulkanDevice* m_Device = nullptr;
        VertexBufferElement m_Elements;
    };
 }