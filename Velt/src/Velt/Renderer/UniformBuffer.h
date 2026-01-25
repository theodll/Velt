#pragma once
#include "Velt/Core/Core.h"
#include "Buffer.h"

namespace Velt::Renderer
{

    class UniformBuffer
    {
    public:
        virtual ~UniformBuffer() {};

		virtual void SetData(void* data, u64 size, u64 offset = 0) = 0;
        virtual void Upload(VkCommandBuffer commandBuffer) = 0;
		
        virtual VkBuffer GetVulkanBuffer() const = 0;
        
		static std::shared_ptr<UniformBuffer> Create(void* uniformData, u64 size, u64 offset);
    };
} 