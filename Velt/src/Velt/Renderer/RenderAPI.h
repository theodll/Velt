#pragma once 
#include "Core/Math.h"
#include "Material.h"
#include "VertexBuffer.h"
#include "Platform/Vulkan/VulkanPipeline.h"
#include "Velt/Core/Core.h"
#include "IndexBuffer.h"
#include "Velt/Renderer/Model.h"
#include "Velt/Renderer/DefferedRenderer.h"
#include "Texture.h"
#include <glm/glm.hpp>

namespace Velt {

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
        virtual void WaitIdle() = 0;

		virtual void DrawQuad(VkCommandBuffer renderCommandBuffer, const Matrix& transform, const Material& material) = 0;
        virtual void DrawTexturedQuad(VkCommandBuffer renderCommandBuffer, const Ref<Texture2D> texture, const Matrix& transform) = 0;
		virtual void DrawStaticModel(VkCommandBuffer commandBuffer, const Ref<Pipeline>& pipeline, const Ref<Model>& model, const Ref<Mesh>& meshSource, u32 submeshIndex, const Ref<MaterialTable>& materialTable, const Matrix& transformModel, u32 entityID) = 0;
        
        virtual void SubmitFullscreenTriangle(VkCommandBuffer renderCommandBuffer, const Ref<Pipeline>& pipeline, const Ref<DefferedShaderInput>& input)  = 0;

		virtual void BeginFrame() = 0;
		virtual void EndFrame() = 0;

		// virtual void BeginRendering(VkCommandBuffer& renderCommandBuffer, bool explicitClear = false) = 0;
		// virtual void EndRendering(VkCommandBuffer& renderCommandBuffer) = 0;

        virtual void BeginScenePass() = 0;
        virtual void EndScenePass() = 0; 

        virtual void BeginDefferedPass() = 0;
        virtual void EndDefferedPass() = 0;

        virtual void BeginGuiPass() = 0;
        virtual void EndGuiPass() = 0;

        virtual i32 GetDrawCallCount() = 0;

		virtual void ClearScreen(VkCommandBuffer& renderCommandBuffer) = 0;
        
        inline static API GetAPI() { return s_API; }

    private:
        
        static API s_API;
    };

}