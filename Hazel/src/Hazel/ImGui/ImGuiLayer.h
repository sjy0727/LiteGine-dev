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