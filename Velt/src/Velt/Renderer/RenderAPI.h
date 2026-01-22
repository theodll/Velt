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

		virtual void DrawQuad(VkCommandBuffer& renderCommandBuffer) = 0;

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		virtual void BeginRendering(VkCommandBuffer& renderCommandBuffer, bool explicitClear = false) = 0;
		virtual void EndRendering(VkCommandBuffer& renderCommandBuffer) = 0;

        virtual void BeginScenePass();
        virtual void EndScenePass();

        virtual void BeginGuiPass();
        virtual void EndGuiPass();

		virtual void ClearScreen(VkCommandBuffer& renderCommandBuffer) = 0;
        
        inline static API GetAPI() { return s_API; }

    private:
        
        static API s_API;
    };

}