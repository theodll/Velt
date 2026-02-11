#pragma once
#include <memory>

#include "Velt/Core/Core.h"
#include "Buffer.h"

namespace Velt
{

    class UniformBuffer
    {
    public:
        virtual ~UniformBuffer() {};

		virtual void SetData(const void* data, u64 size, u64 offset = 0) = 0;
        virtual void Upload(VkCommandBuffer commandBuffer) = 0;
		
        virtual VkBuffer GetVulkanBuffer() const = 0;
        virtual u64 GetSize() const = 0;

		static std::shared_ptr<UniformBuffer> Create(u64 size, u64 offset = 0);
    };
} 