#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <chrono>
#include <thread>
#include <filesystem>


// 统计分数用的对象
class TestScore{
public:
    // 解析完整性测试
    long long validity { -1 };

    // 数值类型支持性
    long long value_type { -1 };

    // 反序列化耗时
    long long unserialize_1 { -1 };  // 常规大型文本数据 big_normal.json 259KB
    long long  unserialize_2 { -1 };  // 超级多的浮点数内容 big_double.json 2199KB
    long long  unserialize_3 { -1 }; // 比较深的list和map嵌套 big_nesting.json 6KB

    // 序列化耗时
    long long  serialize_1 { -1 }; // 常规大型文本数据 big_normal.json 259KB
    long long  serialize_2 { -1 }; // 超级多的浮点数内容 big_double.json 2199KB
    long long  serialize_3 { -1 }; // 比较深的list和map嵌套 big_nesting.json 6KB

    // 序列化+美化
    long long  prettify_1 { -1 }; // 常规大型文本数据 big_normal.json 259KB
    long long  prettify_2 { -1 }; // 超级多的浮点数内容 big_double.json 2199KB
    long long  prettify_3 { -1 }; // 比较深的list和map嵌套 big_nesting.json 6KB

    long long memory { -1 };

    // 获取内部数据测试
    long long get_child { -1 };

    // 增加内容测试
    long long add_child { -1 };

    // 删除内容测试
    long long delete_child { -1 };

    // 浮点解析完备性
    long long double_validity { -1 };

    // 浮点精度测试性
    long long double_accuracy { -1 };

    // 字符串转义完备性
    long long string_validity { -1 };

    // 错误检测能力
    long long error_validity { -1 };
};

// 文件读取
std::string readFile(const std::string& path){
    std::stringstream buffer;
    std::ifstream ifs { path };
    // 其他解析逻辑...
    if (!ifs.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
        return "";
    }
    buffer << ifs.rdbuf();
    ifs.close();
    return buffer.str();
}

void my_write_csv(const std::map<std::string, TestScore>& mp){
    std::vector<std::string> test_project_name = { 
        "lib_name", "validity", "value_type", "memory",
        "unserialize_1", "unserialize_2", "unserialize_3", 
        "serialize_1","serialize_2", "serialize_3", 
        "prettify_1", "prettify_2", "prettify_3",
         "get_child", "add_child", "delete_child",
         "double_validity","double_accuracy", 
         "string_validity", "error_validity"
    };

    // PROJECT_DIR_PATH 宏在CMakeLists.txt中定义，指向项目目录的result文件夹
    std::ofstream ofs{ PROJECT_RESULT_PATH "/result.csv" };
    size_t len = test_project_name.size();
    ofs << test_project_name[0];
    for(size_t it = 1; it<len; ++it){
        ofs << ',' << test_project_name[it];
    }
    ofs << std::endl;

    for(const auto& it : mp){
        ofs << it.first;
        ofs << ',' << it.second.validity;
        ofs << ',' << it.second.value_type;
        ofs << ',' << it.second.memory;
        ofs << ',' << it.second.unserialize_1;
        ofs << ',' << it.second.unserialize_2;
        ofs << ',' << it.second.unserialize_3;
        ofs << ',' << it.second.serialize_1;
        ofs << ',' << it.second.serialize_2;
        ofs << ',' << it.second.serialize_3;
        ofs << ',' << it.second.prettify_1;
        ofs << ',' << it.second.prettify_2;
        ofs << ',' << it.second.prettify_3;
        ofs << ',' << it.second.get_child;
        ofs << ',' << it.second.add_child;
        ofs << ',' << it.second.delete_child;
        ofs << ',' << it.second.double_validity;
        ofs << ',' << it.second.double_accuracy;
        ofs << ',' << it.second.string_validity;
        ofs << ',' << it.second.error_validity << std::endl;
    }
}

// 判断两个浮点数是否相等
bool isEqual(double a, double b, double epsilon = 1.2e-16) {
    if(std::max(std::abs(a), std::abs(b)) < std::numeric_limits<double>::min()*1e20) return true;
    return std::abs(a - b) / std::max(std::abs(a), std::abs(b)) < epsilon;
}



/////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include <windows.h>
#include <psapi.h>

size_t get_memory_usage_KB() {
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize / 1024; // 返回 KB
    }
    return -1; // 失败
}
#endif

#if defined(__linux__) || defined(__APPLE__)
#include <unistd.h>

size_t get_memory_usage_KB() {
#ifdef __linux__
    // Linux 通过 /proc/self/status
    std::ifstream status("/proc/self/status");
    std::string line;
    while (std::getline(status, line)) {
        if (line.find("VmRSS:") == 0) { // 常驻内存
            size_t kb = 0;
            sscanf(line.c_str(), "VmRSS: %zu kB", &kb);
            return kb;
        }
    }
#elif defined(__APPLE__)
    // macOS 通过 sysctl
    pid_t pid = getpid();
    int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PID, pid};
    struct kinfo_proc info;
    size_t len = sizeof(info);
    if (sysctl(mib, 4, &info, &len, NULL, 0) == 0) {
        return info.kp_proc.p_vm_rssize; // 返回 KB
    }
#endif
    return -1; // 失败
}
#endif

/////////////////////////////////////////////////////////////////

