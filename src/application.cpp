#include <imgui.h>
#include <iostream>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GL/glew.h>
#include <OpenGUI/Application.hpp>


void OpenGUI::Application::ErrorCallback(const int error_code, const char* description)
{
    Error("OpenGUI::Application::ErrorCallback", "[error_code=0x{:08X}, description='{}']", error_code, description);
}

OpenGUI::Application::Application(std::vector<std::string>&& layouts)
{
    for (const auto& filename : layouts)
        m_Layouts.emplace_back().LoadFile(filename);

    glfwSetErrorCallback(ErrorCallback);
    glfwInit();
    m_Window = glfwCreateWindow(640, 480, "OpenGUI", nullptr, nullptr);
    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1);
    glewInit();

    ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        auto& style = ImGui::GetStyle();
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init();

    Start();
}

OpenGUI::Application::~Application()
{
    Stop();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_Window);
    glfwTerminate();
    glfwSetErrorCallback(nullptr);
}

void OpenGUI::Application::Start()
{
    auto& io = ImGui::GetIO();

    while (!glfwWindowShouldClose(m_Window))
    {
        glfwPollEvents();

        if (glfwGetWindowAttrib(m_Window, GLFW_ICONIFIED))
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

#pragma region GUI
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

        if (ImGui::Begin("Demo Window"))
            m_Layouts[0].Draw(m_Ctx);
        ImGui::End();
#pragma endregion

        ImGui::Render();

        int width, height;
        glfwGetFramebufferSize(m_Window, &width, &height);
        glViewport(0, 0, width, height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(m_Window);
        }

        glfwSwapBuffers(m_Window);
    }
}

void OpenGUI::Application::Stop()
{
    glfwSetWindowShouldClose(m_Window, true);
}
