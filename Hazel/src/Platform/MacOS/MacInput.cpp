#include "MacInput.h"
#include "hzpch.h"

#include "GLFW/glfw3.h"
#include "Hazel/Application.h"

namespace Hazel
{
    Input* Input::s_Instance = new MacInput();

    bool MacInput::IsKeyPressedImpl(int keycode)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state  = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool MacInput::IsMouseButtonPressedImpl(int button)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state  = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    std::pair<float, float> MacInput::GetMousePositionImpl()
    {
        auto   window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double x_pos, y_pos;
        glfwGetCursorPos(window, &x_pos, &y_pos);
        return std::pair<float, float>((float)x_pos, (float)y_pos);
    }

    float MacInput::GetMouseXImpl()
    {
        auto xy = GetMousePositionImpl();
        return xy.first;
    }

    float MacInput::GetMouseYImpl()
    {
        auto xy = GetMousePositionImpl();
        return xy.second;
    }
} // namespace Hazel