#pragma once
#include "Core/Window.h"
#include "Core/Core.h"
#include "Core/Application.h"
#include "Renderer/RenderContext.h"
#include "Platform/Vulkan/VulkanSwapchain.h"
#include <SDL3/SDL.h>

namespace Velt::Windows
{
	class VELT_API WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline u32 GetWidth() const override { return m_Data.Width; };
		inline u32 GetHeight() const override { return m_Data.Height; };
		bool IsVsync() const override { return m_Data.bVsync; };

		// void SetEventCallback(const EventCallbackFn& callback) override;
		void SetVsync(bool enable) override;
		void SetResizable(bool enable) override;
		void* GetNativeHandle() const override;
		VkExtent2D GetExtent() const { return { Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight() }; }

		void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
		void CreateSwapchain() override;


		Renderer::Vulkan::VulkanSwapchain& GetSwapchain() override;
	private:
		SDL_Window* m_Window = nullptr;
		std::unique_ptr<Renderer::Vulkan::VulkanSwapchain> m_Swapchain;

		virtual void Init();
		virtual void Shutdown();

		struct WindowData
		{
			std::string Title;
			u32 Width, Height;
			bool bVsync;
			bool bResizable = false;

			// EventCallbackFn EventCallback;
		};

		WindowProps m_WindowProps;
		WindowData m_Data;

	};

}