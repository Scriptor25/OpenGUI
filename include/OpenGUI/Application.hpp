#pragma once

#define GLFW_INCLUDE_NONE

#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include <OpenGUI/Context.hpp>
#include <OpenGUI/Layout.hpp>
#include <OpenGUI/OpenGUI.hpp>

namespace OpenGUI
{
    class Application
    {
        static void ErrorCallback(int error_code, const char* description);

    public:
        explicit Application(std::vector<std::string>&& layouts);
        virtual ~Application();

        void Start();
        void Stop();

    protected:
        unsigned CreateWindow(unsigned layout);

    private:
        std::vector<Layout> m_Layouts;
        GLFWwindow* m_Window;
        Context m_Ctx;
    };
}
