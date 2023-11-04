#pragma once

#include "Core.h"

#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
#include "LayerStack.h"
#include "Window.h"

namespace Hazel
{
    class HAZEL_API Application
    {
    private:
        std::unique_ptr<Window> m_Window;
        bool                    m_Running = true;
        LayerStack              m_LayerStack;

    private:
        bool OnWindowClose(WindowCloseEvent& e);

    private:
        static Application* s_Instance;

    public:
        Application();
        virtual ~Application();

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
