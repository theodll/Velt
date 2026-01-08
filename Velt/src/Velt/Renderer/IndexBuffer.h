#pragma once
#include "Velt/Core/Core.h"

namespace Velt::Renderer
{
    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() {};

		virtual void SetData(void* data, u64 size, u64 offset = 0) = 0;
		virtual void Upload(VkCommandBuffer commandBuffer) = 0;

        virtual VkBuffer GetVulkanBuffer() const = 0;
        virtual u32 GetCount() const = 0;

        static std::shared_ptr<IndexBuffer> Create(u64 size);
        static std::shared_ptr<IndexBuffer> Create(void* data, u64 size, u64 offset = 0);
    };
} 