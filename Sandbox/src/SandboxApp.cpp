#include <Hazel.h>

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
        PushLayer(new Hazel::ImGuiLayer);
    }
    ~Sandbox() override = default;
};

Hazel::Application* Hazel::CreateApplication() { return new Sandbox(); }