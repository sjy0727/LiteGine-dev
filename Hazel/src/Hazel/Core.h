#pragma once
// #ifdef HZ_PLATPORM_WINDOWS
// #ifdef HZ_BUILD_DLL
// #define HAZEL_API __declspec(dllexport)
// #else
// #define HAZEL_API __declspec(dllexport)
// #endif
// #endif

// 存放引擎条件编译宏

#ifdef HZ_PLATFORM
#    ifdef HZ_DYNAMIC_LINK
#        if defined(__APPLE__)
#            define HAZEL_API
#        elif defined(_WIN32)
#            ifdef HZ_BUILD_DLL
#                define HAZEL_API __declspec(dllexport)
#            else
#                define HAZEL_API __declspec(dllimport)
#            endif
#        endif
#    else
#        define HAZEL_API
#    endif
#endif

#ifdef HZ_ENABLE_ASSERTS
#    define HZ_ASSERT(x, ...) \
        { \
            if (!(x)) \
            { \
                HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
            } \
        }
#    define HZ_CORE_ASSERT(x, ...) \
        { \
            if (!(x)) \
            { \
                HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
            } \
        }
#else
#    define HZ_ASSERT(x, ...)
#    define HZ_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define HZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)