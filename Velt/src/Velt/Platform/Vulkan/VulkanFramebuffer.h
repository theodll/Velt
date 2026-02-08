#pragma once
/*
#include "Velt/Platform/Vulkan/VulkanDevice.h"
#include "Velt/Renderer/Framebuffer.h"
#include "Velt/Core/Core.h"
#include <cassert>
#include <vector>
#include <vulkan/vulkan.h>

namespace Velt::Vulkan {

enum class FramebufferTextureFormat {
  None = 0,

  RGBA8,
  RGBA16F,
  RED_INTEGER,

  DEPTH24STENCIL8,
  DEPTH32F
};

struct FramebufferTextureSpecification {
  FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;

  FramebufferTextureSpecification() = default;
  FramebufferTextureSpecification(FramebufferTextureFormat format)
      : TextureFormat(format) {}
};

class VELT_API VulkanFramebuffer : public Renderer::Framebuffer {
public:
  VulkanFramebuffer(VulkanDevice *device, const FramebufferSpecification &spec);

  virtual ~VulkanFramebuffer();

  void Invalidate();

  virtual void BeginRenderPass(void* commandBuffer) override;
  virtual void EndRenderPass(void* commandBuffer) override;

  virtual void Resize(u32 width, u32 height) override;
  virtual int ReadPixel(u32 attachmentIndex, int x, int y) override;
  virtual void ClearAttachment(u32 attachmentIndex, int value) override;
  virtual void* GetNativeHandle() override { return m_Framebuffer; }

  virtual const FramebufferSpecification &GetSpecification() const override {
    return m_Specification;
  }

  VkImageView GetColorImageView(u32 index = 0) const {
    assert(index < m_ColorImageViews.size());
    return m_ColorImageViews[index];
  }

  VkImageView GetDepthImageView() const { return m_DepthImageView; }
  VkRenderPass GetRenderPass() const { return m_RenderPass; }
  VkFramebuffer GetVkFramebuffer() const { return m_Framebuffer; }

  VkImage GetColorImage(u32 index = 0) const {
    assert(index < m_ColorImages.size());
    return m_ColorImages[index];
  }

  size_t GetColorAttachmentCount() const { return m_ColorImageViews.size(); }

private:
  void CreateRenderPass();
  void CreateImages();
  void CreateImageViews();
  void CreateFramebuffer();
  void Cleanup();

  u32 FindMemoryType(u32 typeFilter,
                          VkMemoryPropertyFlags properties);

  static VkFormat
  FramebufferTextureFormatToVulkan(FramebufferTextureFormat format);
  static bool IsDepthFormat(FramebufferTextureFormat format);

private:
  VulkanDevice *m_Device = VK_NULL_HANDLE;

  FramebufferSpecification m_Specification;

  std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
  FramebufferTextureSpecification m_DepthAttachmentSpecification;

  VkRenderPass m_RenderPass = VK_NULL_HANDLE;
  VkFramebuffer m_Framebuffer = VK_NULL_HANDLE;

  std::vector<VkImage> m_ColorImages;
  std::vector<VkDeviceMemory> m_ColorMemories;
  std::vector<VkImageView> m_ColorImageViews;

  VkImage m_DepthImage = VK_NULL_HANDLE;
  VkDeviceMemory m_DepthMemory = VK_NULL_HANDLE;
  VkImageView m_DepthImageView = VK_NULL_HANDLE;

  VkBuffer m_StagingBuffer = VK_NULL_HANDLE;
  VkDeviceMemory m_StagingMemory = VK_NULL_HANDLE;

  VkCommandPool m_CommandPool = VK_NULL_HANDLE;
  VkQueue m_GraphicsQueue = VK_NULL_HANDLE;

  static constexpr u32 s_MaxFramebufferSize = 8192;
};

} // namespace Velt::RenderAPI
*/