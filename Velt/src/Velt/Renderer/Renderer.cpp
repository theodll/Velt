#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "vtpch.h"
#include "Velt/Core/Core.h"
#include "Renderer.h"
#include "Velt/Core/Application.h"
#include "Platform/Vulkan/VulkanRenderer.h"
#include "SceneRenderer.h"


namespace Velt::Renderer {

	Scope<RenderAPI> Renderer::s_RenderAPI = nullptr;
	Scope<SceneRenderer> Renderer::s_SceneRenderer = nullptr;

	Renderer::Renderer()
	{
		VT_PROFILE_FUNCTION();
	}

	Renderer::~Renderer()
	{
		VT_PROFILE_FUNCTION();
	}

	void Renderer::Init()
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Init Static Renderer");
		s_RenderAPI = CreateScope<RHI::VulkanRenderer>();
		s_RenderAPI->Init();

		s_SceneRenderer = CreateScope<SceneRenderer>();
		s_SceneRenderer->Init();
	}

	void Renderer::Shutdown()
	{
		VT_PROFILE_FUNCTION();
		VT_CORE_TRACE("Shutdown Static Renderer");
		s_SceneRenderer->Shutdown();
		s_RenderAPI->Shutdown();

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
		s_SceneRenderer->BeginScene();
	}

	void Renderer::EndScenePass()
	{
		VT_PROFILE_FUNCTION();
		s_SceneRenderer->EndScene();
		s_RenderAPI->EndScenePass();
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

	void Renderer::DrawQuad(VkCommandBuffer& renderCommandBuffer, const glm::mat4& transform)
	{
		VT_PROFILE_FUNCTION();
		s_RenderAPI->DrawQuad(renderCommandBuffer, transform);
	}

	void Renderer::DrawStaticModel(VkCommandBuffer& renderCommandBuffer, const Ref<Model>& model, const glm::mat4& transform)
	{
		VT_PROFILE_FUNCTION();
		s_RenderAPI->DrawStaticModel(renderCommandBuffer, model, transform);
	}

	void Renderer::RequestShutdown()
	{
		VT_PROFILE_FUNCTION();
	}



}