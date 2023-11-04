#include "Application.h"
#include "hzpch.h"

#include <glad/glad.h>

#include "Input.h"

namespace Hazel
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        HZ_CORE_ASSERT(s_Instance == nullptr, "Application already exists!");
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        // unsigned int id;
        // glGenVertexArrays(1, &id);
    }

    Application::~Application() {}

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        // FIXME:trace
        // HZ_CORE_TRACE("{0}", e);

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
            {
                break;
            }
        }
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(0.2, 0.2, 0.2, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            for (auto layer : m_LayerStack)
            {
                layer->OnUpdate();
            }

            // 全局监听
//            auto xy = Input::GetMousePosition();
//            HZ_CORE_TRACE("{0}, {1}", xy.first, xy.second);

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

} // namespace Hazel