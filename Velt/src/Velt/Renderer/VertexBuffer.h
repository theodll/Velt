#pragma once
#include "Velt/Core/Core.h"
#include "Buffer.h"

namespace Velt::Renderer
{

    struct VertexBufferElement
    {
        void* data;
        u32 Size;
    };

    struct Vertex
    {
        glm::vec2 position;
    };


    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() {};

		virtual void SetData(void* data, u64 size, u64 offset = 0) = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
        
        virtual void Upload(VkCommandBuffer commandBuffer) = 0;
		
        virtual BufferLayout GetLayout() const = 0;
        virtual VkBuffer GetVulkanBuffer() const = 0;
        
        static std::shared_ptr<VertexBuffer> Create(void* vertexData, u64 vertexCount, u64 vertexStride, bool autoupload = false); 
    };
} 