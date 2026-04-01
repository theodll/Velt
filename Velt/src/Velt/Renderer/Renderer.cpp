#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "vtpch.h"
#include "Velt/Core/Core.h"
#include "Renderer.h"
#include "Velt/Core/Application.h"
#include "Platform/Vulkan/VulkanRenderer.h"
#include "SceneRenderer.h"
#include "Material.h"


namespace Velt {

	Scope<RenderAPI> Renderer::s_RenderAPI = VT_NULL_HANDLE;
	std::unordered_map<u32, Ref<Texture2D>> Renderer::s_RenderTargets;

	Renderer::Renderer()
	{
		VT_PROFILE_FUNCTION();
	}

	Renderer::~Renderer()
	{
		VT_PROFILE_FUNCTION();
	}

	void Renderer::RecreateRenderTargets(u32 width, u32 height)
	{
		VT_PROFILE_FUNCTION();
		TextureCreateInfo albedoAO;
		albedoAO.AspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		albedoAO.Extent = {width, height, 1};
		albedoAO.Format = VK_FORMAT_B8G8R8A8_SRGB;
		albedoAO.ImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		albedoAO.Usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		s_RenderTargets[VT_RENDER_TARGET_ALBEDO_AO] = Texture2D::Create(&albedoAO);

		TextureCreateInfo normalRough;
		normalRough.AspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		normalRough.Extent = {width, height, 1};
		normalRough.Format = VK_FORMAT_B8G8R8A8_SRGB;
		normalRough.ImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		normalRough.Usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		s_RenderTargets[VT_RENDER_TARGET_NORMAL_ROUGH] = Texture2D::Create(&normalRough);

		TextureCreateInfo metalEmit;
		metalEmit.AspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		metalEmit.Extent = {width, height, 1};
		metalEmit.Format = VK_FORMAT_B8G8R8A8_SRGB;
		metalEmit.ImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		metalEmit.Usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		s_RenderTargets[VT_RENDER_TARGET_METAL_EMIT] = Texture2D::Create(&metalEmit);

		TextureCreateInfo depth;
		depth.AspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		depth.Extent = {width, height, 1};
		depth.Format = VK_FORMAT_D32_SFLOAT;
		depth.ImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depth.Usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		s_RenderTargets[VT_RENDER_TARGET_DEPTH] = Texture2D::Create(&depth);

		TextureCreateInfo composite;
		composite.AspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		composite.Extent = {width, height, 1};
		composite.Format = VK_FORMAT_B8G8R8A8_SRGB;
		composite.ImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		composite.Usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		s_RenderTargets[VT_RENDER_TARGET_COMPOSITE] = Texture2D::Create(&composite);

		TextureCreateInfo sampler;
		sampler.AspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		sampler.Extent = { width, height, 1 };
		sampler.Format = VK_FORMAT_B8G8R8A8_SRGB;
		sampler.ImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		sampler.Usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		s_RenderTargets[VT_RENDER_TARGET_SAMPLER] = Texture2D::Create(&sampler);
	}

	void Renderer::Init()
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Init Static Renderer");
		s_RenderAPI = CreateScope<RHI::VulkanRenderer>();
		s_RenderAPI->Init();

		i32 w{ 1920 }, h{ 1080 };

		auto&& app = Application::Get();
		w = app->GetRenderableWidth(), h = app->GetRenderableHeight();

		RecreateRenderTargets(w, h);
	}

	void Renderer::Shutdown()
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_INFO("Shutdown Static Renderer");
		s_RenderTargets.clear();
		Material::Shutdown();


		s_RenderAPI->Shutdown();
		s_RenderAPI.reset();
	}

	void Renderer::BeginScene()
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Begin Scene");
	}

	void Renderer::EndScene()
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("End Scene");
	}

	void Renderer::BeginRendering(VkCommandBuffer& renderCommandBuffer, Ref<VkRenderPass> renderPass, bool explicitClear /*= false*/)
	{
		VT_PROFILE_FUNCTION();
		// s_RenderAPI->BeginRendering(renderCommandBuffer, explicitClear);
	}

	void Renderer::EndRendering(VkCommandBuffer& renderCommandBuffer)
	{
		VT_PROFILE_FUNCTION();
		// s_RenderAPI->EndRendering(renderCommandBuffer);
	}

	void Renderer::BeginFrame()
	{
		VT_PROFILE_FUNCTION();
		s_RenderAPI->BeginFrame();
	}

	void Renderer::BeginScenePass()
	{
		VT_PROFILE_FUNCTION();
		s_RenderAPI->BeginScenePass();
	}

	void Renderer::EndScenePass()
	{
		VT_PROFILE_FUNCTION();
		s_RenderAPI->EndScenePass();
	}

	void Renderer::BeginDefferedPass()
	{
		VT_PROFILE_FUNCTION();
		s_RenderAPI->BeginDefferedPass();
	}
	
	
	void Renderer::EndDefferedPass() 
	{
		VT_PROFILE_FUNCTION();
		s_RenderAPI->EndDefferedPass();
	}

	void Renderer::BeginGuiPass()
	{
		VT_PROFILE_FUNCTION();
		s_RenderAPI->BeginGuiPass();
		ImGuiLayer::Begin();
	}

	void Renderer::EndGuiPass()
	{
		VT_PROFILE_FUNCTION();
		ImGuiLayer::End();
		ImGuiLayer::Render();

		s_RenderAPI->EndGuiPass();
	}

	void Renderer::EndFrame()
	{
		VT_PROFILE_FUNCTION();
		s_RenderAPI->EndFrame();
	}

	void Renderer::DrawQuad(VkCommandBuffer& renderCommandBuffer, const Matrix& transform, const Material& material)
	{
		VT_PROFILE_FUNCTION();
		s_RenderAPI->DrawQuad(renderCommandBuffer, transform, material);
	}

	void Renderer::DrawStaticModel(VkCommandBuffer commandBuffer, const Ref<Pipeline>& pipeline, const Ref<Model>& model, const Ref<Mesh>& meshSource, u32 submeshIndex, const Ref<MaterialTable>& materialTable, const Matrix& transformModel)
	{
		VT_PROFILE_FUNCTION();
		s_RenderAPI->DrawStaticModel(commandBuffer, pipeline, model, meshSource, submeshIndex, materialTable, transformModel);
	}

	void Renderer::SubmitFullscreenTriangle(VkCommandBuffer renderCommandBuffer, const Ref<Pipeline>& pipeline, const Ref<DefferedShaderInput>& input)
	{
		VT_PROFILE_FUNCTION();
		s_RenderAPI->SubmitFullscreenTriangle(renderCommandBuffer, pipeline, input);
	}

	i32 Renderer::GetDrawCallCount()
	{
		return s_RenderAPI->GetDrawCallCount();
	}

	void Renderer::QueueShutdown()
	{
		VT_PROFILE_FUNCTION();
		s_RenderAPI->WaitIdle();
		Shutdown();
	}



}