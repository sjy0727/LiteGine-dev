-- xmake.lua
-- add_rules("mode.debug", "mode.release")
-- Define project GLFW
target("glfw")
    set_kind("static")
    set_languages("c11")
    set_warnings("off")

    -- Add source files
    add_files("src/context.c", "src/init.c", "src/input.c", "src/monitor.c")
    add_files("src/null_init.c", "src/null_joystick.c", "src/null_monitor.c", "src/null_window.c")
    add_files("src/platform.c", "src/vulkan.c", "src/window.c")

    -- Add platform-specific source files for Linux
    if is_plat("linux") then
        add_files("src/x11_init.c", "src/x11_monitor.c", "src/x11_window.c", "src/xkb_unicode.c")
        add_files("src/posix_module.c", "src/posix_time.c", "src/posix_thread.c", "src/glx_context.c")
        add_files("src/egl_context.c", "src/osmesa_context.c", "src/linux_joystick.c")
        add_defines("_GLFW_X11")
    end

    -- Add platform-specific source files for macOS
    if is_plat("macosx") then
        add_mflags("-fno-objc-arc", {target = "src/nsgl_context.m"})
        add_files("src/cocoa_init.m", "src/cocoa_monitor.m", "src/cocoa_window.m", "src/cocoa_joystick.m")
        add_files("src/cocoa_time.c", "src/nsgl_context.m", "src/posix_thread.c", "src/posix_module.c")
        add_files("src/osmesa_context.c", "src/egl_context.c")
        add_defines("_GLFW_COCOA")
    end

    -- Add platform-specific source files for Windows
    if is_plat("windows") then
        add_cxflags("-fno-objc-arc")
        add_files("src/win32_init.c", "src/win32_joystick.c", "src/win32_module.c", "src/win32_monitor.c")
        add_files("src/win32_time.c", "src/win32_thread.c", "src/win32_window.c", "src/wgl_context.c")
        add_files("src/egl_context.c", "src/osmesa_context.c")
        add_defines("_GLFW_WIN32", "_CRT_SECURE_NO_WARNINGS")
    end