#include "../Core.h"
#include <GLFW/glfw3.h>
#include <string>

namespace Lavendel {

	class LAVENDEL_API Window
	{

    public:
        Window(int width, int height, const std::string& title);
        ~Window();

        void PollEvents() const { glfwPollEvents(); };
        bool ShouldClose() const { return glfwWindowShouldClose(m_Window); };
        // void* GetNativeHandle() const { return m_Window; };

    private:
        void Init(int width, int height, const std::string& title);
        void Shutdown();
        GLFWwindow* m_Window = nullptr;
        int m_Width, m_Height;
        std::string m_Title;

	};

}
