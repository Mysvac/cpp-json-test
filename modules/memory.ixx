module;

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#endif

#ifdef __APPLE__
#include <mach/mach.h> // for task_info, mach_task_self, etc.
#endif


export module test_memory;

import std;

export namespace jtu {


    std::size_t get_memory_usage_KB() {
#if defined(_WIN32)
        PROCESS_MEMORY_COUNTERS pmc;
        if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
            return pmc.WorkingSetSize / 1024;
        }
#elif defined(__linux__)
        std::ifstream status("/proc/self/status");
        std::string line;
        while (std::getline(status, line)) {
            std::string_view sv(line);
            if (sv.starts_with("VmRSS:")) {
                std::string_view num = sv.substr(6);
                size_t pos = num.find_first_of("0123456789");
                if (pos != std::string_view::npos) {
                    return std::stoul(std::string(num.substr(pos)));
                }
            }
        }
#elif defined(__APPLE__)
        mach_task_basic_info info;
        mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;
        if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &count) == KERN_SUCCESS) {
            return info.resident_size / 1024;
        }
#endif
        return static_cast<std::size_t>(-1);
    }


}

