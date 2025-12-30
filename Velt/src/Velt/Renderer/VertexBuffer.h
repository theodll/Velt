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
        glm::vec3 color;
    };


    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() {};

        //virtual void setLayout(const BufferLayout& layout) = 0;
        //virtual BufferLayout getLayout() const = 0; 


        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        

		static std::shared_ptr<VertexBuffer> Create(const void* vertexData, u64 vertexCount, u64 vertexStride);
    };
} 