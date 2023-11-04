add_rules("mode.debug", "mode.release")
target("GLFW")
    set_kind("shared")
    set_languages("c17")
    set_runtimes("dynamic")

    -- 禁用警告
    add_defines("NDEBUG")

    add_files(
		-- 头文件不用编译
        -- "include/GLFW/glfw3.h",
		-- "include/GLFW/glfw3native.h",
		-- "src/glfw_config.h",
		"src/context.c",
		"src/init.c",
		"src/input.c",
		"src/monitor.c",

		"src/null_init.c",
		"src/null_joystick.c",
		"src/null_monitor.c",
		"src/null_window.c",

		"src/platform.c",
		"src/vulkan.c",
		"src/window.c"
    )
    if is_plat("macosx") then
        -- add_files(
        --     "src/cocoa_init.m",
		-- 	"src/cocoa_monitor.m",
		-- 	"src/cocoa_window.m",
		-- 	"src/cocoa_joystick.m",
		-- 	"src/cocoa_time.c",
			-- "src/nsgl_context.m",
			-- "src/posix_thread.c",
			-- "src/posix_module.c",
			-- "src/osmesa_context.c",
			-- "src/egl_context.c"
        -- )
		add_files(
			"src/cocoa_init.m",
			-- "src/cocoa_joystick.h",
			"src/cocoa_joystick.m",
			"src/cocoa_monitor.m",
			-- "src/cocoa_platform.h",
			"src/cocoa_time.c",
			"src/cocoa_window.m"
		)
    end

	-- add_files("src/**.c","src/**.m")

	add_defines("_GLFW_COCOA")
	-- add_defines("_GLFW_USE_CONFIG_H")
	add_includedirs("include")
	add_frameworks("Cocoa", "IOKit", "CoreVideo", "CoreFoundation")
    