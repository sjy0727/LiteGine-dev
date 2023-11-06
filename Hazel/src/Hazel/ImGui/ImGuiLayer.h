#pragma once

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Layer.h"

namespace Hazel
{
    class HAZEL_API ImGuiLayer : public Layer
    {
        //    private:
        //        float m_Time = 0.0f;

        //            private:
        //        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        //        bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
        //        bool OnMouseMovedEvent(MouseMovedEvent& e);
        //        bool OnMouseScrolledEvent(MouseScrolledEvent& e);
        //
        //        bool OnKeyPressedEvent(KeyPressedEvent& e);
        //        bool OnKeyTypedEvent(KeyTypedEvent& e);
        //        bool OnKeyReleasedEvent(KeyReleasedEvent& e);
        //
        //        bool OnWindowResizeEvent(WindowResizeEvent& e);

    public:
        ImGuiLayer();
        ~ImGuiLayer() override;

        void OnAttach() override;
        void OnDetach() override;
        //        void OnUpdate() override;
        void OnImGuiRender() override;

        void Begin();
        void End();
        //        void OnEvent(Event& event) override;
    };
} // namespace Hazel