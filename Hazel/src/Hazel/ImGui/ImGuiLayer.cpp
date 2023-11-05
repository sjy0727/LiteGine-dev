#include "ImGuiLayer.h"
#include "Application.h"
#include "hzpch.h"

#include "ImGui/imgui.h"
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

        // BUG:addtest

        // TODO: 最终使用Hazel 键码 包含glfw3.h
        // io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        // io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        // io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        // io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        // io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        // io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        // io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        // io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        // io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        // io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        // io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        // io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        // io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        // io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        // io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        // io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        // io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        // io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        // io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        // io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        // io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

        // io.KeyMap[ImGuiButtonFlags_MouseButtonLeft] = GLFW_MOUSE_BUTTON_LEFT;
        // io.KeyMap[ImGuiButtonFlags_MouseButtonRight] = GLFW_MOUSE_BUTTON_RIGHT;
        // io.KeyMap[ImGuiButtonFlags_MouseButtonMiddle] = GLFW_MOUSE_BUTTON_MIDDLE;

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular
        // ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding              = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        // TODO: install_callbacks
        ImGui_ImplGlfw_InitForOpenGL(backup_current_context, true);
        const char* glsl_version = "#version 410";
        ImGui_ImplOpenGL3_Init(glsl_version);
    }

    void ImGuiLayer::OnDetach() {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::OnUpdate()
    {

        ImGuiIO&     io  = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize   = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

        float time   = (float)glfwGetTime();
        io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.f / 60.f);
        m_Time       = time;

        ImGui_ImplOpenGL3_NewFrame();
        // BUG: add test
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // TODO: ImGui Editor代码入口
        static bool show = true;
//        ImGui::ShowDemoWindow(&show);
//        Editor::Get()->ShowFPSOverLay(show);
        Editor::Get()->ShowImGuizmo();



        ImGui::Render();
        // BUG: test add
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        int         display_w, display_h;
        glfwGetFramebufferSize(backup_current_context, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
//            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::OnEvent(Event& event)
    {
//        HZ_CORE_TRACE(event);

        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
        dispatcher.Dispatch<MouseMovedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
        dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));

        dispatcher.Dispatch<KeyPressedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
        dispatcher.Dispatch<KeyTypedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
        dispatcher.Dispatch<KeyReleasedEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));

        dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
    }

    bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
    {
        ImGuiIO& io                      = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = true;

        return false;
    }

    bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
    {
        ImGuiIO& io                      = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = false;

        return false;
    }

    bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(e.GetX(), e.GetY());

        return false;
    }

    bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += e.GetXOffset();
        io.MouseWheel += e.GetYOffset();

        return false;
    }

    bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
    {
        // extern ImGuiKey ImGui_ImplGlfw_KeyToImGuiKey(int key);
        // ImGuiKey keycode = e.GetKeyCode();
        // io.AddKeyEvent(ImGui_ImplGlfw_KeyToImGuiKey(e.GetKeyCode()), true);

        // io.AddKeyEvent(ImGuiKey key, bool down);
        // io.AddKeyEvent(ImGuiKey key, bool down);
        // io.AddKeyEvent(ImGuiKey key, bool down);
        // io.AddKeyEvent(ImGuiKey key, bool down);

        // HZ_TRACE("{0}", e);
        // ImGuiIO& io                 = ImGui::GetIO();
        // io.KeysDown[e.GetKeyCode()] = true;

        // io.KeyCtrl  = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        // io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        // io.KeyAlt   = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        // io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];

        return false;
    }

    bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e){
        ImGuiIO& io = ImGui::GetIO();
        int keycode = e.GetKeyCode();
        io.AddInputCharacter(keycode);

        return false;
    }

    bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
    {
        ImGuiIO& io                 = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = false;

        return false;
    }

    bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
    {
        ImGuiIO& io                = ImGui::GetIO();
        io.DisplaySize             = ImVec2(e.GetWidth(), e.GetHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, e.GetWidth(), e.GetHeight());

        return false;
    }

} // namespace Hazel