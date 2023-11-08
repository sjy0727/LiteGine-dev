#include "ImGuiLayer.h"
#include "Application.h"
#include "hzpch.h"

#include "ImGui/imgui.h"
//#include "ImGui/imgui_impl_glfw.h"
//#include "ImGui/imgui_impl_opengl3.h"
#include "Platform/ImGuiBackend/imgui_impl_glfw.h"
#include "Platform/ImGuiBackend/imgui_impl_opengl3.h"


#include "Editor/Editor.h"

// FIXME: 暂时存放
#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace Hazel
{

    ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

    ImGuiLayer::~ImGuiLayer() = default;

    void ImGuiLayer::OnAttach()
    {

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        // io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        // io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular
        // ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding              = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

//        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        Application& app = Application::Get();
        GLFWwindow* backup_current_context = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
        // TODO: install_callbacks
        ImGui_ImplGlfw_InitForOpenGL(backup_current_context, true);
        const char* glsl_version = "#version 410";
        ImGui_ImplOpenGL3_Init(glsl_version);
    }

    void ImGuiLayer::OnDetach()
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

//    void ImGuiLayer::OnUpdate()
//    {
//
//        ImGuiIO&     io  = ImGui::GetIO();
//        Application& app = Application::Get();
//        io.DisplaySize   = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
//
//        float time   = (float)glfwGetTime();
//        io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.f / 60.f);
//        m_Time       = time;
//
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplGlfw_NewFrame();
//        ImGui::NewFrame();
//
//        // TODO: ImGui Editor代码入口
////        static bool show = true;
//        //        ImGui::ShowDemoWindow(&show);
//        //        Editor::Get()->ShowFPSOverLay(show);
//        Editor::Get()->ShowImGuizmo();
//
//        ImGui::Render();
//        // BUG: test add
//        GLFWwindow* backup_current_context = glfwGetCurrentContext();
//        int         display_w, display_h;
//        glfwGetFramebufferSize(backup_current_context, &display_w, &display_h);
//        glViewport(0, 0, display_w, display_h);
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
//        {
//            //            GLFWwindow* backup_current_context = glfwGetCurrentContext();
//            ImGui::UpdatePlatformWindows();
//            ImGui::RenderPlatformWindowsDefault();
//            glfwMakeContextCurrent(backup_current_context);
//        }
//    }

    void ImGuiLayer::Begin()
    {
        g_profiler.Begin(Hazel::Profiler::Stage::NewFrame);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        g_profiler.End(Hazel::Profiler::Stage::NewFrame);
    }
    void ImGuiLayer::End()
    {
        ImGuiIO&     io  = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize   = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

        g_profiler.Begin(Profiler::Stage::Rendering);
        ImGui::Render();
        // FIXME: 修正
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        int         display_w, display_h;
        glfwGetFramebufferSize(backup_current_context, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        g_profiler.Begin(Profiler::Stage::ImGuiRenderOpenGL);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        g_profiler.End(Profiler::Stage::ImGuiRenderOpenGL);


        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            //            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        g_profiler.End(Profiler::Stage::Rendering);

    }

    void ImGuiLayer::OnImGuiRender()
    {
//        static bool show = true;
//        g_profiler.Begin(Hazel::Profiler::Stage::ImGuizmo);
//        Hazel::Editor::Get()->ShowImGuizmo();
//        g_profiler.End(Hazel::Profiler::Stage::ImGuizmo);
//
//        g_profiler.Begin(Hazel::Profiler::Stage::FileDialog);
//        Hazel::Editor::Get()->ShowFileDialog();
//        g_profiler.End(Hazel::Profiler::Stage::FileDialog);
//
//        g_profiler.Begin(Hazel::Profiler::Stage::DemoWindow);
//        ImGui::ShowDemoWindow(&show);
//        g_profiler.End(Hazel::Profiler::Stage::DemoWindow);
//
//        // FIXME: FlameGraph只能在OnImGuiRender中 性能分析点可以在循环中任意点
//        ImGui::Begin("Profiler Window", &show);
//        auto& entry = g_profiler._entries[g_profiler.GetCurrentEntryIndex()];
//        ImGuiWidgetFlameGraph::PlotFlame("CPU", &ProfilerValueGetter, &entry, g_profiler._StageCount, 0, "Main Thread", FLT_MAX, FLT_MAX, ImVec2(400, 0));
//        ImGui::End();
    }


} // namespace Hazel