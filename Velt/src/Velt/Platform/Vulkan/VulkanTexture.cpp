#include "vtpch.h"
#include "VulkanContext.h"
#include "Core/Core.h"
#include "Core/Application.h"
#include "VulkanTexture.h"

#include "stb_image.h"

namespace Velt::RHI 
{

	static void TransitionImageLayout(
		VkCommandBuffer commandBuffer,
		VkImage image,
		VkFormat format,
		VkImageLayout oldLayout,
		VkImageLayout newLayout
	)
	{
		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;

		VkPipelineStageFlags sourceStage{};
		VkPipelineStageFlags destinationStage{};

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else {
			VT_CORE_ASSERT(false, "Unsupported layout transition");
		}
		
		vkCmdPipelineBarrier(
			commandBuffer,
			sourceStage,
			destinationStage,
			0,
			0, VT_NULL_HANDLE,
			0, VT_NULL_HANDLE,
			1, &barrier
		);
	}

	VulkanTexture2D::VulkanTexture2D(const std::filesystem::path& path) : m_Path(path)
	{
		int width{}, height{}, channels{};
		stbi_uc* pixels = stbi_load(path.string().c_str(), &width, &height, &channels, STBI_rgb_alpha);
		VT_CORE_ASSERT(pixels, "Failed to load image");

		m_Width = width;
		m_Height = height;

		CreateImage(pixels);
		CreateImageView();
		CreateImageSampler();
	}

	VulkanTexture2D::VulkanTexture2D(TextureCreateInfo* pInfo) 
	{
		m_Width = pInfo->Extent.width;
		m_Height = pInfo->Extent.height;

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.format = pInfo->Format;
		imageInfo.extent = pInfo->Extent;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.usage = pInfo->Usage;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.initialLayout = pInfo->ImageLayout;

		VulkanContext::GetDevice()->CreateImageWithInfo(imageInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Image, m_ImageMemory);

		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = m_Image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = pInfo->Format;
		viewInfo.subresourceRange.aspectMask = pInfo->AspectMask;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VT_VK_CHECK(vkCreateImageView(VulkanContext::GetDevice()->device(), &viewInfo, VT_NULL_HANDLE, &m_ImageView), "Failed to create Image View");

		CreateImageSampler();
	}

	VulkanTexture2D::VulkanTexture2D(i32 width, i32 height) : m_Width(width), m_Height(height)
	{

		CreateImageWithoutData();
		CreateImageView();
		CreateImageSampler();
	}

	VulkanTexture2D::~VulkanTexture2D() 
	{
		auto&& pDevice = VulkanContext::GetDevice();
		//vkDestroyImage(pDevice->device(), m_Image, VT_NULL_HANDLE);
		//vkFreeMemory(pDevice->device(), m_ImageMemory, VT_NULL_HANDLE);
		//vkDestroyImageView(pDevice->device(), m_ImageView, VT_NULL_HANDLE);
		//vkDestroySampler(pDevice->device(), m_Sampler, VT_NULL_HANDLE);
	}

	void VulkanTexture2D::CreateImage(stbi_uc* pPixelData) 
	{
		VkDeviceSize imageSize = m_Width * m_Height * 4;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		auto&& pDevice = VulkanContext::GetDevice();
		pDevice->CreateBuffer(pDevice->device(), imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer, stagingBufferMemory);

		void* pData;
		vkMapMemory(pDevice->device(), stagingBufferMemory, 0, imageSize, 0, &pData);
		memcpy(pData, pPixelData, (size_t)imageSize);
		vkUnmapMemory(pDevice->device(), stagingBufferMemory);

		stbi_image_free(pPixelData);

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = m_Width;
		imageInfo.extent.height = m_Height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.flags = 0;

		auto&& uploader = VulkanContext::GetResourceUploader();
		uploader->Begin();

		pDevice->CreateImageWithInfo(imageInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Image, m_ImageMemory);

		TransitionImageLayout(uploader->GetCommandBuffer(), m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

		pDevice->CopyBufferToImage(uploader->GetCommandBuffer(), stagingBuffer, m_Image, m_Width, m_Height, 1);

		TransitionImageLayout(uploader->GetCommandBuffer(), m_Image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		uploader->End();

		vkDestroyBuffer(pDevice->device(), stagingBuffer, VT_NULL_HANDLE);
		vkFreeMemory(pDevice->device(), stagingBufferMemory, VT_NULL_HANDLE);
	}

	void VulkanTexture2D::CreateImageWithoutData() 
	{
		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.format = VK_FORMAT_B8G8R8A8_SRGB;
		imageInfo.extent.width = m_Width;
		imageInfo.extent.height = m_Height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		VulkanContext::GetDevice()->CreateImageWithInfo(imageInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Image, m_ImageMemory);

	}

	void VulkanTexture2D::CreateImageView() 
	{
		auto&& pDevice = VulkanContext::GetDevice();

		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = m_Image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VT_VK_CHECK(vkCreateImageView(pDevice->device(), &viewInfo, VT_NULL_HANDLE, &m_ImageView), "Failed to create Image View");
	}

	void VulkanTexture2D::CreateImageSampler() 
	{
		auto&& pDevice = VulkanContext::GetDevice();

		// Todo [07.03.26, Theo]: Make this flexible and adjustable in settings
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;

		VkPhysicalDeviceProperties properties{};
		vkGetPhysicalDeviceProperties(pDevice->GetPhysicalDevice(), &properties);

		samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;

		VT_VK_CHECK(vkCreateSampler(pDevice->device(), &samplerInfo, VT_NULL_HANDLE, &m_Sampler), "Failed to create Texture Sampler")
	}


	void VulkanTexture2D::CreateStagingData()
	{
		VT_PROFILE_FUNCTION();

		const auto& device = VulkanContext::GetDevice();
		u64 size = sizeof(u32) * m_Width * m_Height;

		if (m_StagingBuffer == VK_NULL_HANDLE || m_StagingBufferSize < size) {
			if (m_StagingBuffer != VK_NULL_HANDLE) {

				vkDestroyBuffer(device->device(), m_StagingBuffer, nullptr);
				vkFreeMemory(device->device(), m_StagingBufferMemory, nullptr);

			}

			device->CreateBuffer(
				device->device(),
				size,
				VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				m_StagingBuffer,
				m_StagingBufferMemory
			);
			m_StagingBufferSize = size;
		}

		if (m_Fence == VK_NULL_HANDLE)
		{
			VkFenceCreateInfo fenceInfo{};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			vkCreateFence(device->device(), &fenceInfo, VT_NULL_HANDLE, &m_Fence);
		}

	}

	u32 VulkanTexture2D::ReadPixel(int x, int y)
	{
		VT_PROFILE_FUNCTION();
		auto&& device = VulkanContext::GetDevice();
		auto&& sc = Velt::Application::Get()->GetWindow()->GetSwapchain();
		auto&& uploader = VulkanContext::GetResourceUploader();

		u64 size = sizeof(u32) * m_Width * m_Height;

		if (m_Fence == VK_NULL_HANDLE || m_StagingBuffer == VK_NULL_HANDLE) 
		{
			CreateStagingData();
		}
		
		uploader->Begin();
		
		sc->TransitionImageLayout(
			uploader->GetCommandBuffer(),
			m_Image,
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 
			VK_PIPELINE_STAGE_TRANSFER_BIT                 
		);

		VkBufferImageCopy bufferRegion{};
		bufferRegion.bufferOffset = 0;
		bufferRegion.bufferRowLength = 0;
		bufferRegion.bufferImageHeight = 0; 
		bufferRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		bufferRegion.imageSubresource.mipLevel = 0;
		bufferRegion.imageSubresource.baseArrayLayer = 0; 
		bufferRegion.imageSubresource.layerCount = 1;
		bufferRegion.imageOffset = { 0, 0, 0 };
		bufferRegion.imageExtent = { m_Width, m_Height, 1 };

		vkCmdCopyImageToBuffer(uploader->GetCommandBuffer(), m_Image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_StagingBuffer, 1, &bufferRegion);

		sc->TransitionImageLayout(
			uploader->GetCommandBuffer(),
			m_Image,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
		);

		uploader->End();

		vkWaitForFences(device->device(), 1, &m_Fence, VK_TRUE, UINT64_MAX);
		vkResetFences(device->device(), 1, &m_Fence);

		void* data;
		vkMapMemory(device->device(), m_StagingBufferMemory, 0, size, 0, &data);

		u32* pIDs = reinterpret_cast<uint32_t*>(data);
		u32 objectID = pIDs[y * m_Width + x];

		vkUnmapMemory(device->device(), m_StagingBufferMemory);

		return objectID;
	}
}
