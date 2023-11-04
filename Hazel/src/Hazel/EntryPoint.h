#pragma once
#include "Hazel.h"
#ifdef HZ_PLATFORM
extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char* argv[])
{
    Hazel::Log::Init();
    // Hazel::Log::GetCoreLogger()->warn("Initialized log!");
    // Hazel::Log::GetClientLogger()->info("Hello!");
    HZ_CORE_WARN("Initialized Log!");
    int a = 5;
    HZ_INFO("hello! Var={0}", 5);

    auto app = Hazel::CreateApplication();
    app->Run();
    delete app;
    return 0;
}
#endif