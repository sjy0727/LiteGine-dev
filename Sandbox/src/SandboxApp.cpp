#include <Hazel.h>
#include "ImGui/imgui.h"
#include "Hazel/Editor/Editor.h"

//#include "Hazel/Profiler.h"

class ExampleLayer : public Hazel::Layer
{
public:
    ExampleLayer() : Layer("Example") {}

    void OnUpdate() override
    {
        //        HZ_INFO("ExampleLayer::Update");
        if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
        {
            HZ_TRACE("Tab key is pressed (epoll)!");
        }
    }

    void OnImGuiRender() override
    {
        static bool show = true;
        // demowindow 得在前面 否则 下面的window 无法 dock
        Hazel::g_profiler.Begin(Hazel::Profiler::Stage::DemoWindow);
        ImGui::ShowDemoWindow(&show);
        Hazel::g_profiler.End(Hazel::Profiler::Stage::DemoWindow);

        Hazel::g_profiler.Begin(Hazel::Profiler::Stage::ImGuizmo);
        Hazel::Editor::Get()->ShowImGuizmo();
        Hazel::g_profiler.End(Hazel::Profiler::Stage::ImGuizmo);

        Hazel::g_profiler.Begin(Hazel::Profiler::Stage::FileDialog);
        Hazel::Editor::Get()->ShowFileDialog();
        Hazel::g_profiler.End(Hazel::Profiler::Stage::FileDialog);

        // FIXME: FlameGraph只能在OnImGuiRender中 性能分析点可以在循环中任意点
        ImGui::Begin("Profiler Window", &show);
        auto& entry = Hazel::g_profiler._entries[Hazel::g_profiler.GetCurrentEntryIndex()];
        ImGuiWidgetFlameGraph::PlotFlame("CPU", &Hazel::ProfilerValueGetter, &entry, Hazel::g_profiler._StageCount, 0, "Main Thread", FLT_MAX, FLT_MAX, ImVec2(400, 0));
        ImGui::End();
    }

    void OnEvent(Hazel::Event& event) override
    {
        if (event.GetEventType() == Hazel::EventType::KeyPressed)
        {
            auto& e = dynamic_cast<Hazel::KeyPressedEvent&>(event);
            if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
            {
                HZ_TRACE("Tab key is pressed (event)!");
            }
            HZ_TRACE("{0}", (char)e.GetKeyCode());
        }
    }
};

class Sandbox : public Hazel::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());

        //在 Application中已经实例化过ImGuiLayer了
//        PushLayer(new Hazel::ImGuiLayer());
    }
    ~Sandbox() override = default;
};

Hazel::Application* Hazel::CreateApplication() { return new Sandbox(); }