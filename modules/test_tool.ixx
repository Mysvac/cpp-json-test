export module test_tools;

import std;

export namespace jtu {

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

    /**
     * @brief 测试分数类，用于存储各个测试项目的分数
     */
    class TestScore{
    public:
        std::int64_t validity { -1 };       // 基础有效性测试
        std::int64_t value_type { -1 };     // 值类型测试（是否支持将整数、字符串等直接作为根类型）
        std::int64_t deserialize_1 { -1 };   // 常规大型文本数据 big_normal.json 259KB
        std::int64_t deserialize_2 { -1 };  // 超级多的浮点数内容 big_double.json 2199KB
        std::int64_t deserialize_3 { -1 };  // 比较深的list和map嵌套 big_nesting.json 6KB
        std::int64_t serialize_1 { -1 };    // 常规大型文本数据 big_normal.json 259KB
        std::int64_t serialize_2 { -1 };    // 超级多的浮点数内容 big_double.json 2199KB
        std::int64_t serialize_3 { -1 };    // 比较深的list和map嵌套 big_nesting.json 6KB
        std::int64_t prettify_1 { -1 };     // 常规大型文本数据 big_normal.json 259KB
        std::int64_t prettify_2 { -1 };     // 超级多的浮点数内容 big_double.json 2199KB
        std::int64_t prettify_3 { -1 };     // 比较深的list和map嵌套 big_nesting.json 6KB
        std::int64_t memory { -1 };         // 内存占用测试
        std::int64_t get_child { -1 };      // 获取+拷贝子元素测试
        std::int64_t add_child { -1 };      // 添加+拷贝子元素测试
        std::int64_t delete_child { -1 };   // 删除子元素测试


        TestScore& operator+=(const TestScore& other) noexcept {
            if (memory == 0) memory += other.memory;
            deserialize_1 += other.deserialize_1;
            deserialize_2 += other.deserialize_2;
            deserialize_3 += other.deserialize_3;
            serialize_1 += other.serialize_1;
            serialize_2 += other.serialize_2;
            serialize_3 += other.serialize_3;
            prettify_1 += other.prettify_1;
            prettify_2 += other.prettify_2;
            prettify_3 += other.prettify_3;
            get_child += other.get_child;
            add_child += other.add_child;
            delete_child += other.delete_child;
            return *this;
        }
        TestScore& operator-=(const TestScore& other) noexcept {
            deserialize_1 -= other.deserialize_1;
            deserialize_2 -= other.deserialize_2;
            deserialize_3 -= other.deserialize_3;
            serialize_1 -= other.serialize_1;
            serialize_2 -= other.serialize_2;
            serialize_3 -= other.serialize_3;
            prettify_1 -= other.prettify_1;
            prettify_2 -= other.prettify_2;
            prettify_3 -= other.prettify_3;
            get_child -= other.get_child;
            add_child -= other.add_child;
            delete_child -= other.delete_child;
            return *this;
        }
        TestScore& operator/=(const long long divisor) noexcept {
            deserialize_1 /= divisor;
            deserialize_2 /= divisor;
            deserialize_3 /= divisor;
            serialize_1 /= divisor;
            serialize_2 /= divisor;
            serialize_3 /= divisor;
            prettify_1 /= divisor;
            prettify_2 /= divisor;
            prettify_3 /= divisor;
            get_child /= divisor;
            add_child /= divisor;
            delete_child /= divisor;
            return *this;
        }
        void maxed(const TestScore& other) noexcept {
            if (deserialize_1 < other.deserialize_1) deserialize_1 = other.deserialize_1;
            if (deserialize_2 < other.deserialize_2) deserialize_2 = other.deserialize_2;
            if (deserialize_3 < other.deserialize_3) deserialize_3 = other.deserialize_3;
            if (serialize_1 < other.serialize_1) serialize_1 = other.serialize_1;
            if (serialize_2 < other.serialize_2) serialize_2 = other.serialize_2;
            if (serialize_3 < other.serialize_3) serialize_3 = other.serialize_3;
            if (prettify_1 < other.prettify_1) prettify_1 = other.prettify_1;
            if (prettify_2 < other.prettify_2) prettify_2 = other.prettify_2;
            if (prettify_3 < other.prettify_3) prettify_3 = other.prettify_3;
            if (get_child < other.get_child) get_child = other.get_child;
            if (add_child < other.add_child) add_child = other.add_child;
            if (delete_child < other.delete_child) delete_child = other.delete_child;
        }
        void mined(const TestScore& other) noexcept {
            if (deserialize_1 > other.deserialize_1) deserialize_1 = other.deserialize_1;
            if (deserialize_2 > other.deserialize_2) deserialize_2 = other.deserialize_2;
            if (deserialize_3 > other.deserialize_3) deserialize_3 = other.deserialize_3;
            if (serialize_1 > other.serialize_1) serialize_1 = other.serialize_1;
            if (serialize_2 > other.serialize_2) serialize_2 = other.serialize_2;
            if (serialize_3 > other.serialize_3) serialize_3 = other.serialize_3;
            if (prettify_1 > other.prettify_1) prettify_1 = other.prettify_1;
            if (prettify_2 > other.prettify_2) prettify_2 = other.prettify_2;
            if (prettify_3 > other.prettify_3) prettify_3 = other.prettify_3;
            if (get_child > other.get_child) get_child = other.get_child;
            if (add_child > other.add_child) add_child = other.add_child;
            if (delete_child > other.delete_child) delete_child = other.delete_child;
        }
        static TestScore get_max() noexcept {
            TestScore res;
            res.deserialize_1 = std::numeric_limits<std::int64_t>::max() / 2;
            res.deserialize_2 = std::numeric_limits<std::int64_t>::max() / 2;
            res.deserialize_3 = std::numeric_limits<std::int64_t>::max() / 2;
            res.serialize_1 = std::numeric_limits<std::int64_t>::max() / 2;
            res.serialize_2 = std::numeric_limits<std::int64_t>::max() / 2;
            res.serialize_3 = std::numeric_limits<std::int64_t>::max() / 2;
            res.prettify_1 = std::numeric_limits<std::int64_t>::max() / 2;
            res.prettify_2 = std::numeric_limits<std::int64_t>::max() / 2;
            res.prettify_3 = std::numeric_limits<std::int64_t>::max() / 2;
            res.get_child = std::numeric_limits<std::int64_t>::max() / 2;
            res.add_child = std::numeric_limits<std::int64_t>::max() / 2;
            res.delete_child = std::numeric_limits<std::int64_t>::max() / 2;
            return res;
        }

    };



    void generate_csv(const std::map<std::string, TestScore>& score_map, const std::string& path) {
        std::ofstream ofs(path);
        if (!ofs.is_open()) {
            throw std::runtime_error("Failed to open file for writing: " + path);
        }

        ofs << "Library,Validity,ValueType,DeserializeAll,DeserializeNum,DeserializeStr,"
            << "SerializeAll,SerializeNum,SerializeStr,PrettifyAll,PrettifyNum,PrettifyStr,"
            << "Memory,GetChild,AddChild,DeleteChild\n";

        for (const auto& [lib_name, score] : score_map) {
            ofs << lib_name << ','
                << score.validity << ','
                << score.value_type << ','
                << score.deserialize_1 << ','
                << score.deserialize_2 << ','
                << score.deserialize_3 << ','
                << score.serialize_1 << ','
                << score.serialize_2 << ','
                << score.serialize_3 << ','
                << score.prettify_1 << ','
                << score.prettify_2 << ','
                << score.prettify_3 << ','
                << score.memory << ','
                << score.get_child << ','
                << score.add_child << ','
                << score.delete_child
                << '\n';
        }

    }


}

