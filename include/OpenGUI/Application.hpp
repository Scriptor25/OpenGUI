#pragma once

#define GLFW_INCLUDE_NONE

#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include <OpenGUI/OpenGUI.hpp>

namespace OpenGUI
{
    class Application
    {
    private:
        static void ErrorCallback(int error_code, const char* description);

    public:
        explicit Application(std::vector<std::string>&& layouts);
        virtual ~Application();

        void Start();
        void Stop();

    protected:

    private:
        std::vector<std::string> m_Layouts;
        GLFWwindow* m_Window;
    };
}
