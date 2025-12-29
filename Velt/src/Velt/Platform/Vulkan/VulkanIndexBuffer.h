#pragma once
#include "Core/Core.h"
#include "Velt/Renderer/IndexBuffer.h"
#include <vulkan/vulkan.h>

namespace Velt::Renderer::Vulkan
{
	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(u64 size);
		VulkanIndexBuffer(void* data, u64 size);
		~VulkanIndexBuffer();

		void Bind() const;
		void SetData(void* data, u64 size, u64 offset = 0);

		VkBuffer GetBuffer() const { return m_IndexBuffer; }
		u64 GetSize() const { return m_Size; }

	private:
		void CreateBuffer(void* data, u64 size);
		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, u64 size);

		VkBuffer m_IndexBuffer = VK_NULL_HANDLE;
		VkDeviceMemory m_IndexBufferMemory = VK_NULL_HANDLE;
		u64 m_Size = 0;
	};
}