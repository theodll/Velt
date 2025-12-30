#pragma once
#include "vtpch.h"
#include "RenderCommandBuffer.h"


namespace Velt::Renderer
{
	class Application;
	class LayerStack;

	enum class RendererAPI
	{
		None = 0,
		Vulkan = 1,
		D3D12 = 2,
		METAL = 3
	};


	class VELT_API Renderer
	{
	public:
		Renderer();
		~Renderer();

		virtual void drawFrame();

		// virtual void setImGuiLayer(ImGuiLayer* layer) = 0;
		virtual void setLayerStack(LayerStack* layerStack) = 0;
		
		static void requestShutdown();
		static void Shutdown();

		// static void BeginRenderPass(Ref<RenderCommandBuffer> renderCommandbuffer, Ref<RenderPass> renderPass, bool explicitClear = false);
		// static void EndRenderPass(Ref<RenderCommandBuffer> renderCommandbuffer);

		inline static RendererAPI GetAPI() { return s_API; };

	private:
		virtual void loadModels() = 0;
		virtual void createPipelineLayout() = 0;
		virtual void createPipeline() = 0;
		static RendererAPI s_API;


	};
}
