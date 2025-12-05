#include "Framebuffer.h"
#include "Velt/Plattform/Vulkan/VulkanFramebuffer.h"
#include "Velt/Renderer/Renderer.h"
#include "vtpch.h"

namespace Velt {
namespace RenderAPI {

Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification &spec) {
  return CreateRef<VulkanFramebuffer>(Renderer::getDevice(), spec);
}

} // namespace RenderAPI
} // namespace Velt
