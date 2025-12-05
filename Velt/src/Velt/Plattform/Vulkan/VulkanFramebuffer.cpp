#include "VulkanFramebuffer.h"
#include <stdexcept>

namespace Velt::RenderAPI {

VkFormat VulkanFramebuffer::FramebufferTextureFormatToVulkan(
    FramebufferTextureFormat format) {
  switch (format) {
  case FramebufferTextureFormat::RGBA8:
    return VK_FORMAT_R8G8B8A8_UNORM;
  case FramebufferTextureFormat::RGBA16F:
    return VK_FORMAT_R16G16B16A16_SFLOAT;
  case FramebufferTextureFormat::RED_INTEGER:
    return VK_FORMAT_R32_SINT;
  case FramebufferTextureFormat::DEPTH24STENCIL8:
    return VK_FORMAT_D24_UNORM_S8_UINT;
  case FramebufferTextureFormat::DEPTH32F:
    return VK_FORMAT_D32_SFLOAT;
  case FramebufferTextureFormat::None:
  default:
    return VK_FORMAT_UNDEFINED;
  }
}

bool VulkanFramebuffer::IsDepthFormat(FramebufferTextureFormat format) {
  switch (format) {
  case FramebufferTextureFormat::DEPTH24STENCIL8:
  case FramebufferTextureFormat::DEPTH32F:
    return true;
  default:
    return false;
  }
}

VulkanFramebuffer::VulkanFramebuffer(GPUDevice *device,
                                     const FramebufferSpecification &spec)
    : m_Device(device), m_Specification(spec),
      m_DepthAttachmentSpecification(FramebufferTextureFormat::None) {

  m_ColorAttachmentSpecifications.push_back(
      FramebufferTextureSpecification(FramebufferTextureFormat::RGBA8));

  m_DepthAttachmentSpecification = FramebufferTextureSpecification(
      FramebufferTextureFormat::DEPTH24STENCIL8);

  Invalidate();
}

VulkanFramebuffer::~VulkanFramebuffer() { Cleanup(); }

void VulkanFramebuffer::Cleanup() {
  if (m_Device == VK_NULL_HANDLE)
    return;

  vkDeviceWaitIdle(m_Device->device());
  if (m_Framebuffer != VK_NULL_HANDLE) {
    vkDestroyFramebuffer(m_Device->device(), m_Framebuffer, nullptr);
    m_Framebuffer = VK_NULL_HANDLE;
  }

  if (m_RenderPass != VK_NULL_HANDLE) {
    vkDestroyRenderPass(m_Device->device(), m_RenderPass, nullptr);
    m_RenderPass = VK_NULL_HANDLE;
  }

  for (size_t i = 0; i < m_ColorImages.size(); i++) {
    if (m_ColorImageViews[i] != VK_NULL_HANDLE)
      vkDestroyImageView(m_Device->device(), m_ColorImageViews[i], nullptr);
    if (m_ColorImages[i] != VK_NULL_HANDLE)
      vkDestroyImage(m_Device->device(), m_ColorImages[i], nullptr);
    if (m_ColorMemories[i] != VK_NULL_HANDLE)
      vkFreeMemory(m_Device->device(), m_ColorMemories[i], nullptr);
  }
  m_ColorImages.clear();
  m_ColorMemories.clear();
  m_ColorImageViews.clear();

  if (m_DepthImageView != VK_NULL_HANDLE) {
    vkDestroyImageView(m_Device->device(), m_DepthImageView, nullptr);
    m_DepthImageView = VK_NULL_HANDLE;
  }
  if (m_DepthImage != VK_NULL_HANDLE) {
    vkDestroyImage(m_Device->device(), m_DepthImage, nullptr);
    m_DepthImage = VK_NULL_HANDLE;
  }
  if (m_DepthMemory != VK_NULL_HANDLE) {
    vkFreeMemory(m_Device->device(), m_DepthMemory, nullptr);
    m_DepthMemory = VK_NULL_HANDLE;
  }

  if (m_StagingBuffer != VK_NULL_HANDLE) {
    vkDestroyBuffer(m_Device->device(), m_StagingBuffer, nullptr);
    m_StagingBuffer = VK_NULL_HANDLE;
  }
  if (m_StagingMemory != VK_NULL_HANDLE) {
    vkFreeMemory(m_Device->device(), m_StagingMemory, nullptr);
    m_StagingMemory = VK_NULL_HANDLE;
  }
}

void VulkanFramebuffer::Invalidate() {
  Cleanup();

  CreateRenderPass();
  CreateImages();
  CreateImageViews();
  CreateFramebuffer();
}

void VulkanFramebuffer::CreateRenderPass() {
  std::vector<VkAttachmentDescription> attachments;
  std::vector<VkAttachmentReference> colorAttachmentRefs;

  VkSampleCountFlagBits sampleCount =
      static_cast<VkSampleCountFlagBits>(m_Specification.Samples);

  for (size_t i = 0; i < m_ColorAttachmentSpecifications.size(); i++) {
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = FramebufferTextureFormatToVulkan(
        m_ColorAttachmentSpecifications[i].TextureFormat);
    colorAttachment.samples = sampleCount;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    attachments.push_back(colorAttachment);

    VkAttachmentReference colorRef{};
    colorRef.attachment = static_cast<uint32_t>(i);
    colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorAttachmentRefs.push_back(colorRef);
  }

  VkAttachmentReference depthAttachmentRef{};
  bool hasDepth = m_DepthAttachmentSpecification.TextureFormat !=
                  FramebufferTextureFormat::None;

  if (hasDepth) {
    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = FramebufferTextureFormatToVulkan(
        m_DepthAttachmentSpecification.TextureFormat);
    depthAttachment.samples = sampleCount;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    depthAttachmentRef.attachment = static_cast<uint32_t>(attachments.size());
    depthAttachmentRef.layout =
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    attachments.push_back(depthAttachment);
  }

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount =
      static_cast<uint32_t>(colorAttachmentRefs.size());
  subpass.pColorAttachments = colorAttachmentRefs.data();
  subpass.pDepthStencilAttachment = hasDepth ? &depthAttachmentRef : nullptr;

  VkSubpassDependency dependency{};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                             VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = static_cast<u32>(attachments.size());
  renderPassInfo.pAttachments = attachments.data();
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &dependency;

  if (vkCreateRenderPass(m_Device->device(), &renderPassInfo, nullptr,
                         &m_RenderPass) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create render pass!");
  }
}

void VulkanFramebuffer::CreateImages() {
  VkSampleCountFlagBits sampleCount =
      static_cast<VkSampleCountFlagBits>(m_Specification.Samples);

  for (const auto &spec : m_ColorAttachmentSpecifications) {
    VkImage image;
    VkDeviceMemory memory;

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = FramebufferTextureFormatToVulkan(spec.TextureFormat);
    imageInfo.extent.width = m_Specification.Width;
    imageInfo.extent.height = m_Specification.Height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.samples = sampleCount;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
                      VK_IMAGE_USAGE_SAMPLED_BIT |
                      VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    if (vkCreateImage(m_Device->device(), &imageInfo, nullptr, &image) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to create color image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(m_Device->device(), image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(
        memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(m_Device->device(), &allocInfo, nullptr, &memory) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to allocate image memory!");
    }

    vkBindImageMemory(m_Device->device(), image, memory, 0);

    m_ColorImages.push_back(image);
    m_ColorMemories.push_back(memory);
  }

  if (m_DepthAttachmentSpecification.TextureFormat !=
      FramebufferTextureFormat::None) {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = FramebufferTextureFormatToVulkan(
        m_DepthAttachmentSpecification.TextureFormat);
    imageInfo.extent = {m_Specification.Width, m_Specification.Height, 1};
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.samples = sampleCount;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    if (vkCreateImage(m_Device->device(), &imageInfo, nullptr, &m_DepthImage) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to create depth image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(m_Device->device(), m_DepthImage,
                                 &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(
        memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(m_Device->device(), &allocInfo, nullptr,
                         &m_DepthMemory) != VK_SUCCESS) {
      throw std::runtime_error("Failed to allocate depth image memory!");
    }
    vkBindImageMemory(m_Device->device(), m_DepthImage, m_DepthMemory, 0);
  }
}

void VulkanFramebuffer::CreateImageViews() {
  for (size_t i = 0; i < m_ColorImages.size(); i++) {
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = m_ColorImages[i];
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = FramebufferTextureFormatToVulkan(
        m_ColorAttachmentSpecifications[i].TextureFormat);
    viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(m_Device->device(), &viewInfo, nullptr, &imageView) !=
        VK_SUCCESS) {
      throw std::runtime_error("Failed to create color image view!");
    }
    m_ColorImageViews.push_back(imageView);
  }

  if (m_DepthImage != VK_NULL_HANDLE) {
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = m_DepthImage;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = FramebufferTextureFormatToVulkan(
        m_DepthAttachmentSpecification.TextureFormat);
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

    if (m_DepthAttachmentSpecification.TextureFormat ==
        FramebufferTextureFormat::DEPTH24STENCIL8) {
      viewInfo.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }

    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(m_Device->device(), &viewInfo, nullptr,
                          &m_DepthImageView) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create depth image view!");
    }
  }
}

void VulkanFramebuffer::CreateFramebuffer() {
  std::vector<VkImageView> attachments;

  for (const auto &colorView : m_ColorImageViews) {
    attachments.push_back(colorView);
  }

  if (m_DepthImageView != VK_NULL_HANDLE) {
    attachments.push_back(m_DepthImageView);
  }

  VkFramebufferCreateInfo framebufferInfo{};
  framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  framebufferInfo.renderPass = m_RenderPass;
  framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
  framebufferInfo.pAttachments = attachments.data();
  framebufferInfo.width = m_Specification.Width;
  framebufferInfo.height = m_Specification.Height;
  framebufferInfo.layers = 1;

  if (vkCreateFramebuffer(m_Device->device(), &framebufferInfo, nullptr,
                          &m_Framebuffer) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create framebuffer!");
  }
}

void VulkanFramebuffer::BeginRenderPass(VkCommandBuffer commandBuffer) {
  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = m_RenderPass;
  renderPassInfo.framebuffer = m_Framebuffer;
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = {m_Specification.Width,
                                      m_Specification.Height};

  std::vector<VkClearValue> clearValues;

  for (size_t i = 0; i < m_ColorImageViews.size(); i++) {
    VkClearValue clearValue{};
    clearValue.color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues.push_back(clearValue);
  }

  if (m_DepthImageView != VK_NULL_HANDLE) {
    VkClearValue depthClear{};
    depthClear.depthStencil = {1.0f, 0};
    clearValues.push_back(depthClear);
  }

  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(commandBuffer, &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(m_Specification.Width);
  viewport.height = static_cast<float>(m_Specification.Height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = {m_Specification.Width, m_Specification.Height};
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void VulkanFramebuffer::EndRenderPass(VkCommandBuffer commandBuffer) {
  vkCmdEndRenderPass(commandBuffer);
}

void VulkanFramebuffer::Resize(u32 width, u32 height) {
  if (width == 0 || height == 0 || width > s_MaxFramebufferSize ||
      height > s_MaxFramebufferSize) {
    return;
  }

  m_Specification.Width = width;
  m_Specification.Height = height;

  Invalidate();
}

int VulkanFramebuffer::ReadPixel(u32 attachmentIndex, int x, int y) {
  // TODO: full implemntation:
  // 1. Staging Buffer
  // 2. Image Layout Transition
  // 3.  vkCmdCopyImageToBuffer
  // 4. read out vkMapMemory and Pixel

  assert(attachmentIndex < m_ColorImages.size());
  return 0; // Placeholder
}

void VulkanFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value) {
  // TODO: Full impkementation:
  // outside RenderPass: vkCmdClearColorImage
  // inside RenderPass: vkCmdClearAttachments

  assert(attachmentIndex < m_ColorImages.size());
}

uint32_t VulkanFramebuffer::FindMemoryType(uint32_t typeFilter,
                                           VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(m_Device->getPhysicalDevice(),
                                      &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags &
                                    properties) == properties) {
      return i;
    }
  }

  throw std::runtime_error("Failed to find suitable memory type!");
}
} // namespace Velt::RenderAPI
