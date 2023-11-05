#pragma once
#include <iostream>
#include <sstream>
#ifdef _WIN32
#    include <intrin.h>
#    include <windows.h>
#elif __APPLE__
#    include <array>
#    include <memory>
#    include <sys/sysctl.h>
#elif __linux__
#    include <fstream>
#endif

std::string get_cpu_info()
{
    char               cpuid[0x40] = {0};
    std::ostringstream oss;
#ifdef _WIN32
    int cpuInfo[4] = {-1};
    __cpuid(cpuInfo, 0x80000002);
    memcpy(cpuid, cpuInfo, sizeof(cpuInfo));
    __cpuid(cpuInfo, 0x80000003);
    memcpy(cpuid + 16, cpuInfo, sizeof(cpuInfo));
    __cpuid(cpuInfo, 0x80000004);
    memcpy(cpuid + 32, cpuInfo, sizeof(cpuInfo));
#elif __APPLE__
    size_t size = sizeof(cpuid);
    sysctlbyname("machdep.cpu.brand_string", &cpuid, &size, nullptr, 0);
#elif __linux__
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string   line;
    while (std::getline(cpuinfo, line))
    {
        if (line.substr(0, 10) == "model name")
        {
            strcpy(cpuid, line.substr(line.find(":") + 2).c_str());
            break;
        }
    }
#endif
    oss << "CPU Model: " << cpuid;
    return oss.str();
}

std::string get_gpu_info()
{
    std::ostringstream oss;
#ifdef _WIN32
    DISPLAY_DEVICE dd;
    dd.cb = sizeof(dd);
    EnumDisplayDevices(NULL, 0, &dd, 0);
    oss << "GPU Model: " << dd.DeviceString;
#elif __APPLE__
    std::array<char, 128>                    buffer{};
    std::string                              result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(
        popen("system_profiler SPDisplaysDataType | awk -F': ' '/Chipset Model/ {print $2}'", "r"), pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    oss << "GPU Model: " << result;
#elif __linux__
    std::ifstream gpuinfo("/proc/driver/nvidia/gpus/0/information");
    std::string   line;
    while (std::getline(gpuinfo, line))
    {
        if (line.substr(0, 18) == "Model:              ")
        {
            oss << "GPU Model: " << line.substr(18);
            break;
        }
    }
#endif
    return oss.str();
}

int main()
{
    std::string cpu_info = get_cpu_info();
    std::string gpu_info = get_gpu_info();
    std::cout << cpu_info << std::endl;
    std::cout << gpu_info << std::endl;
    return 0;
}
