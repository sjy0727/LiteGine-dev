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

        // application是单例的所以不用担心 ImGuiLayer 内存泄漏
        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application() { delete m_ImGuiLayer; }

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
            g_profiler.Frame();
            g_profiler.Begin(Profiler::Stage::Plot);
            glClearColor(0.2, 0.2, 0.2, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            g_profiler.Begin(Profiler::Stage::LayersOnUpdate);
            for (auto layer : m_LayerStack)
            {
                layer->OnUpdate();
            }
            g_profiler.End(Profiler::Stage::LayersOnUpdate);

            // 全局监听
            //            auto xy = Input::GetMousePosition();
            //            HZ_CORE_TRACE("{0}, {1}", xy.first, xy.second);

            // 渲染ImGui部分
            g_profiler.Begin(Profiler::Stage::LayersOnImGui);
            m_ImGuiLayer->Begin();
            for (auto layer : m_LayerStack)
            {
                layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();
            g_profiler.End(Profiler::Stage::LayersOnImGui);

            g_profiler.Begin(Profiler::Stage::WindowOnUpdate);
            m_Window->OnUpdate();
            g_profiler.End(Profiler::Stage::WindowOnUpdate);

            g_profiler.End(Profiler::Stage::Plot);
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

} // namespace Hazel