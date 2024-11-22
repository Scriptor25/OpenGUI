#include <iostream>
#include <OpenGUI/Application.hpp>

void OpenGUI::Application::ErrorCallback(const int error_code, const char* description)
{
    Error("OpenGUI::Application::ErrorCallback", "[error_code=0x{:08X}, description='{}']", error_code, description);
}

OpenGUI::Application::Application(std::vector<std::string>&& layouts)
    : m_Layouts(std::move(layouts))
{
    glfwSetErrorCallback(ErrorCallback);
    glfwInit();
    m_Window = glfwCreateWindow(640, 480, "OpenGUI", nullptr, nullptr);

    Start();
}

OpenGUI::Application::~Application()
{
    Stop();

    glfwDestroyWindow(m_Window);
    glfwTerminate();
    glfwSetErrorCallback(nullptr);
}

void OpenGUI::Application::Start()
{
    while (!glfwWindowShouldClose(m_Window))
    {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}

void OpenGUI::Application::Stop()
{
    glfwSetWindowShouldClose(m_Window, true);
}
