export module test_tools;

import std;

export namespace jtu {
        /**
     * @brief 测试分数类，用于存储各个测试项目的分数
     */
    class TestScore{
    public:
        std::int64_t validity { -1 };       // 基础有效性测试
        std::int64_t value_type { -1 };
        std::int64_t deserialize_1 { -1 };   // 常规大型文本数据 big_normal.json 259KB
        std::int64_t deserialize_2 { -1 };  // 超级多的浮点数内容 big_double.json 2199KB
        std::int64_t deserialize_3 { -1 };  // 比较深的list和map嵌套 big_nesting.json 6KB
        std::int64_t serialize_1 { -1 };    // 常规大型文本数据 big_normal.json 259KB
        std::int64_t serialize_2 { -1 };    // 超级多的浮点数内容 big_double.json 2199KB
        std::int64_t serialize_3 { -1 };    // 比较深的list和map嵌套 big_nesting.json 6KB
        std::int64_t prettify_1 { -1 };     // 常规大型文本数据 big_normal.json 259KB
        std::int64_t prettify_2 { -1 };     // 超级多的浮点数内容 big_double.json 2199KB
        std::int64_t prettify_3 { -1 };     // 比较深的list和map嵌套 big_nesting.json 6KB
        std::int64_t memory { -1 };
        std::int64_t get_child { -1 };
        std::int64_t add_child { -1 };
        std::int64_t delete_child { -1 };
        std::int64_t double_validity { -1 };
        std::int64_t double_accuracy { -1 };
        std::int64_t string_validity { -1 };
        std::int64_t error_validity { -1 };
    };

    /**
     * @brief 读取文件内容
     * @param path 文件路径
     * @return 返回文件内容的字符串
     */
    std::string read_file(const std::string& path){
        std::filesystem::path file_path { path };
        if (!std::filesystem::exists(file_path)) {
            throw std::runtime_error("File does not exist: " + path);
        }
        const auto file_size = std::filesystem::file_size(file_path);
        std::string content;
        content.resize(file_size);

        std::ifstream file(file_path, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Cannot open file: " + file_path.string());
        }
        file.read(content.data(), file_size);
        return content;
    }

}

