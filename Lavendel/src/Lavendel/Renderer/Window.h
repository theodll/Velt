#pragma once

#include "../Core.h"
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>

namespace Lavendel {

    namespace RenderAPI {

        // Forward declarations to break circular dependency
        class GPUDevice;
        class Pipeline;

        class LAVENDEL_API Window
        {

        public:
            Window(int width, int height, const std::string& title, bool bResizable);
            ~Window();

            Window(const Window&) = delete;
            void operator=(const Window&) = delete;

            void PollEvents() const { glfwPollEvents(); };
            bool ShouldClose() const { return glfwWindowShouldClose(m_Window); };
            void* GetNativeHandle() const { return m_Window; };
            void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
			VkExtent2D getExtent() const { return { static_cast<uint32_t>(m_Width), static_cast<uint32_t>(m_Height) }; }

        private:
            void Init(int width, int height, const std::string& title, bool bResizable);
            void Shutdown();

            GLFWwindow* m_Window;
            int m_Width, m_Height;
            bool m_Resizable;
            std::string m_Title;

            // Use smart pointers for proper initialization order
            std::unique_ptr<GPUDevice> m_Device;
            std::unique_ptr<Pipeline> m_Pipeline;

        };

    }
}