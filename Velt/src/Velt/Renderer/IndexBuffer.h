#pragma once
#include "Velt/Core/Core.h"

namespace Velt::Renderer
{
    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() {};

        virtual void Bind() const = 0;
        virtual u64 GetSize() const = 0;
      
        static std::shared_ptr<IndexBuffer> Create(u64 size);
        static std::shared_ptr<IndexBuffer> Create(void* data, u64 size);
    };
} 