#include "hzpch.h"
#include "Window.h"

#ifdef HZ_PLATFORM
    #include "Platform/MacOS/MacWindow.h"
#endif

namespace Hazel
{
    Window* Window::Create(const WindowProps& props)
    {
#ifdef HZ_PLATFORM
        return new MacWindow(props);
#else
        return nullptr;
#endif
    }

    // FIXME: 必须在这里实现否则链接时无法找到函数实现
    // Window::~Window() {}
    // void Window::SetEventCallback(const Window::EventCallbackFn& callback) {}
    // void Window::SetVSync(bool enabled) {}
    // bool Window::IsVSync() const { return true; }

} // namespace Hazel
