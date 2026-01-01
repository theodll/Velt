#include "vtpch.h"
#include "VulkanRenderAPI.h"
#include "VulkanContext.h"

namespace Velt::Renderer::Vulkan {

    static glm::vec4 s_ClearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    void VulkanRenderAPI::Clear()
    {
        // Clear is handled by the render pass in Vulkan
    }

    void VulkanRenderAPI::SetClearColor(const glm::vec4& color)
    {
        s_ClearColor = color;
    }

    void VulkanRenderAPI::DrawIndexed(const std::shared_ptr<VertexBuffer>& vertexBuffer, std::shared_ptr<IndexBuffer>& indexBuffer)
    {
        // Drawing is handled through command buffers in Vulkan
        // This method is primarily for API compatibility
    }

}
