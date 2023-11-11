#pragma once

#include "Core.h"

#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
#include "LayerStack.h"
#include "Window.h"

#include "ImGui/ImGuiLayer.h"
#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

namespace Hazel
{
    class HAZEL_API Application
    {
    private:
        std::unique_ptr<Window> m_Window;
        ImGuiLayer*             m_ImGuiLayer;
        bool                    m_Running = true;
        LayerStack              m_LayerStack;

        std::shared_ptr<Shader>      m_Shader;
        std::shared_ptr<VertexArray> m_VertexArray;

        std::shared_ptr<Shader>      m_BlueShader;
        std::shared_ptr<VertexArray> m_SquareVA;

    private:
        bool OnWindowClose(WindowCloseEvent& e);

    private:
        static Application* s_Instance;

    public:
        Application();
        virtual ~Application() = default;

        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        inline static Application& Get() { return *s_Instance; }
        inline Window&             GetWindow() { return *m_Window; }
    };

    // 在客户端(Sandbox)中定义
    Application* CreateApplication();

} // namespace Hazel
