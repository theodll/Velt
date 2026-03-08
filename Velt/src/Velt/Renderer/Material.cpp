#include "Material.h"
#include "UniformBuffer.h"
#include "SceneRenderer.h"
#include "Platform/Vulkan/VulkanContext.h"
#include "Platform/Vulkan/DescriptorSetManager.h"
#include "Core/Application.h"

namespace Velt
{

		Material::Material(const HVector& pColor)
		{
			m_Data.Color = pColor;
			for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			{
				m_UBOs[i] = UniformBuffer::Create(sizeof(MaterialUBO));

				auto& layouts = SceneRenderer::GetPipeline()->GetSetLayouts();

				// Todo [27.02, Theo] Maybe change this that this isn't a magic number but still, I couldn't 
				// care less

				// Note [27.02, Theo] The one stands for the Material Set Layout (set 0 = global, set 1 = material)
				m_Sets[i] = RHI::VulkanContext::GetSetManager()->Allocate(layouts[1]);

				RHI::VulkanContext::GetSetManager()->WriteBuffer(
					m_Sets[i],
					0,
					m_UBOs[i]->GetVulkanBuffer(),
					sizeof(MaterialUBO)
				);
			}
		};

		void Material::SetTexture(u32 binding, Ref<Texture2D> pTexture)
		{
			m_Textures[binding] = pTexture;

			for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			{
				VkDescriptorImageInfo info{};
				info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				info.imageView = pTexture->GetImageView();
				info.sampler = pTexture->GetSampler();

				RHI::VulkanContext::GetSetManager()->WriteImage(
					m_Sets[i],
					binding,
					info
				);
			}
		}

		const VkDescriptorSet& Material::GetSet() const { return m_Sets[Application::Get()->GetWindow()->GetSwapchain()->GetCurrentFrameIndex()]; }

		void Material::SetColor(const HVector& pColor)
		{
			m_Data.Color = pColor;
			for (int i{ 0 }; i < MAX_FRAMES_IN_FLIGHT; i++)
			{
				m_UBOs[i]->SetData(&m_Data, sizeof(MaterialUBO), 0);
			}

		}
}