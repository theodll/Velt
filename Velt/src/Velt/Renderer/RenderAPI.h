#pragma once 

#include "VertexBuffer.h"
#include "Platform/Vulkan/VulkanPipeline.h"
#include "Velt/Core/Core.h"
#include "IndexBuffer.h"
#include "Velt/Renderer/Model.h"
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

		virtual void DrawQuad(VkCommandBuffer& renderCommandBuffer, const glm::mat4& transform) = 0;
        virtual void DrawStaticModel(VkCommandBuffer& renderCommandBuffer, const Ref<Model>& model, const glm::mat4& transform) = 0;

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		// virtual void BeginRendering(VkCommandBuffer& renderCommandBuffer, bool explicitClear = false) = 0;
		// virtual void EndRendering(VkCommandBuffer& renderCommandBuffer) = 0;

        virtual void BeginScenePass() = 0;
        virtual void EndScenePass() = 0;

        virtual void BeginGuiPass() = 0;
        virtual void EndGuiPass() = 0;

		virtual void ClearScreen(VkCommandBuffer& renderCommandBuffer) = 0;
        
        inline static API GetAPI() { return s_API; }

    private:
        
        static API s_API;
    };

}