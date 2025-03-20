#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <chrono>

// 统计分数用的对象
class TestScore{
public:
    // 解析完整性测试
    long long integrity { -1 };

    // 数值类型支持性
    long long value_unserialize { -1 };

    // 反序列化耗时
    long long unserialize_time_1 { -1 };  // 常规大型文本数据 big_normal.json 259KB
    long long  unserialize_time_2 { -1 };  // 超级多的浮点数内容 big_double.json 2199KB
    long long  unserialize_time_3 { -1 }; // 比较深的list和map嵌套 big_nesting.json 6KB

    // 序列化耗时
    long long  serialize_time_1 { -1 }; // 常规大型文本数据 big_normal.json 259KB
    long long  serialize_time_2 { -1 }; // 超级多的浮点数内容 big_double.json 2199KB
    long long  serialize_time_3 { -1 }; // 比较深的list和map嵌套 big_nesting.json 6KB

    // 序列化+美化
    long long  serialize_pretty_time_1 { -1 }; // 常规大型文本数据 big_normal.json 259KB
    long long  serialize_pretty_time_2 { -1 }; // 超级多的浮点数内容 big_double.json 2199KB
    long long  serialize_pretty_time_3 { -1 }; // 比较深的list和map嵌套 big_nesting.json 6KB

    // 获取内部数据测试
    long long get_inner { -1 };

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
        "lib_name", "integrity", "value_unserialize","unserialize_time_1", 
        "unserialize_time_2", "unserialize_time_3", "serialize_time_1",
        "serialize_time_2", "serialize_time_3", "serialize_pretty_time_1", 
        "serialize_pretty_time_2", "serialize_pretty_time_3", "get_inner", 
        "add_child", "delete_child", "double_validity",
        "double_accuracy", "string_validity", "error_validity"
    };

    // PROJECT_DIR_PATH 宏在CMakeLists.txt中定义，指向项目目录的result文件夹
    std::ofstream ofs{ PROJECT_DIR_PATH "/result.csv" };
    size_t len = test_project_name.size();
    ofs << test_project_name[0];
    for(size_t it = 1; it<len; ++it){
        ofs << ',' << test_project_name[it];
    }
    ofs << std::endl;

    for(const auto& it : mp){
        ofs << it.first;
        ofs << ',' << it.second.integrity;
        ofs << ',' << it.second.value_unserialize;
        ofs << ',' << it.second.unserialize_time_1;
        ofs << ',' << it.second.unserialize_time_2;
        ofs << ',' << it.second.unserialize_time_3;
        ofs << ',' << it.second.serialize_time_1;
        ofs << ',' << it.second.serialize_time_2;
        ofs << ',' << it.second.serialize_time_3;
        ofs << ',' << it.second.serialize_pretty_time_1;
        ofs << ',' << it.second.serialize_pretty_time_2;
        ofs << ',' << it.second.serialize_pretty_time_3;
        ofs << ',' << it.second.get_inner;
        ofs << ',' << it.second.add_child;
        ofs << ',' << it.second.delete_child;
        ofs << ',' << it.second.double_validity;
        ofs << ',' << it.second.double_accuracy;
        ofs << ',' << it.second.string_validity;
        ofs << ',' << it.second.error_validity << std::endl;
    }
}

// 判断两个浮点数是否相等
bool isEqual(double a, double b, double epsilon = 5e-16) {
    if(std::max(std::abs(a), std::abs(b)) < std::numeric_limits<double>::min()*1e20) return true;
    return std::abs(a - b) / std::max(std::abs(a), std::abs(b)) < epsilon;
}

