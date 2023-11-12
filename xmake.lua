-- add_rules("mode.debug", "mode.release")
add_rules("mode.debug")
-- 设置c++11标准
set_languages("cxx17") -- c++14支持的make_unique 以及 filedialog里c++17支持的filesystem

-- 设置预定义宏
add_defines("HZ_PLATFORM")
-- add_defines("HZ_BUILD_SHARED_LIB")
add_defines("HZ_ENABLE_ASSERTS")

-- 添加模块
-- glfw
includes("Hazel/vendor/glfw")
-- glad
includes("Hazel/vendor/Glad")
-- ImGui
includes("Hazel/vendor/ImGui")

target("Hazel", function()
    -- 预编译头文件
    set_pcxxheader("Hazel/src/hzpch.h")

    -- 编译为动态链接库
--     set_kind("shared")
    set_kind("static")

    add_includedirs("Hazel/src")
    add_includedirs("Hazel/src/Hazel")
    add_includedirs("Hazel/vendor/glm")
    add_includedirs("Hazel/vendor/Glad/include")
    add_includedirs("Hazel/vendor/glfw/include")
    add_includedirs("Hazel/vendor/spdlog/include")
    add_includedirs("Hazel/vendor/ImGui/include")
    add_includedirs("Hazel/vendor/ImGui/include/ImGui")

    -- 递归编译文件夹下面的所有cpp文件
    add_files("Hazel/src/**.cpp")

    -- 添加Glad依赖
    add_deps("Glad")
    -- 添加Imgui依赖
    add_deps("ImGui")
    -- 添加glfw依赖
    add_deps("glfw")

    -- 定义 _CRT_SECURE_NO_WARNINGS 宏以取消 strcpy 报错, 取消switch警告
    add_cxflags("-D_CRT_SECURE_NO_WARNINGS", "-Wno-switch")
    -- 设置不显示GLFW warning
    add_defines("GL_SILENCE_DEPRECATION")
    -- 设置包含GLFW时不再包含OpenGL头文件
    add_defines("GLFW_INCLUDE_NONE")

--     add_defines("_GLFW_COCOA")
--     add_frameworks("OpenGL", "Cocoa", "IOKit", "CoreVideo", "CoreFoundation")
end)


target("LiteGine", function()
    -- 编译为二进制可执行文件
    set_kind("binary")
    add_files("Sandbox/src/*.cpp")

    -- 链接动态链接库
    add_deps("Hazel")
    add_includedirs("Hazel/src")
    add_includedirs("Hazel/vendor/glm")
    add_includedirs("Hazel/vendor/glfw/include")
    add_includedirs("Hazel/vendor/spdlog/include")
    add_includedirs("Hazel/vendor/ImGui/include")
    add_includedirs("Hazel/vendor/ImGui/include/ImGui")
end)


--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro defination
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

