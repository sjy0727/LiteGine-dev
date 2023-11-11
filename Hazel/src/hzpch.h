#pragma once
// 预编译头文件
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>

#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Hazel/Log.h"
#include "Hazel/Profiler.h"

#ifdef HZ_PLATFORM
#    if defined(_WIN32)
#        include <Windows.h>
#    endif
#endif