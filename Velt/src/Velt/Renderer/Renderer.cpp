#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "vtpch.h"
#include "Velt/Core/Core.h"
#include "Renderer.h"
#include "Velt/Core/Application.h"


namespace Velt::Renderer {

	RendererAPI Renderer::s_API = RendererAPI::Vulkan;

	struct SimplePushConstantData
	{
		glm::mat2 transform{1.0f};
		glm::vec2 offset;
		alignas(16) glm::vec4 color;
	};


	void Renderer::requestShutdown()
	{
		VT_PROFILE_FUNCTION();
		Velt::Application::s_ShutdownRequested = true;
	};

	Renderer::Renderer()
	{

	}

	Renderer::~Renderer()
	{

	}

	void Renderer::drawFrame()
	{

	}

	void Renderer::setLayerStack(LayerStack* layerStack)
	{

	}

	void Renderer::Shutdown()
	{

	}

	void Renderer::loadModels()
	{

	}

	void Renderer::createPipelineLayout()
	{

	}

	void Renderer::createPipeline()
	{

	}


}