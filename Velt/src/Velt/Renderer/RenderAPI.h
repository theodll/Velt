#pragma once 

#include "VertexBuffer.h"
#include "Platform/Vulkan/VulkanPipeline.h"
#include "Velt/Core/Core.h"
#include "IndexBuffer.h"
#include <glm/glm.hpp>

namespace Velt::Renderer {

    class RenderAPI
    {
    public:

        enum class API
        {
            None = 0,
            Vulkan = 1,
            D3D12 = 2,
            METAL = 3
        };

		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual void DrawQuad(Ref<VkCommandBuffer> renderCommandBuffer, Ref<Vulkan::VulkanPipeline> pipeline, const glm::mat4& transform) = 0;

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		virtual void BeginRenderPass(Ref<VkCommandBuffer> renderCommandBuffer, Ref<VkRenderPass> renderpass, bool explicitClear = false) = 0;
		virtual void EndRenderPass(Ref<VkCommandBuffer> renderCommandBuffer) = 0;

		virtual void ClearScreen(Ref<VkCommandBuffer> renderCommandBuffer) = 0;
        
        inline static API GetAPI() { return s_API; }

    private:
        
        static API s_API;
    };

}