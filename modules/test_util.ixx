export module jtu;

import std;

export namespace jtu {
    /**
     * @brief 子元素数量统计类
     */
    struct JsonCounter{
        std::size_t m_object_num {0};
        std::size_t m_object_size {0};
        std::size_t m_array_num {0};
        std::size_t m_array_size {0};
        std::size_t m_string_num {0};
        std::size_t m_number_num {0};
        std::size_t m_true_num {0};
        std::size_t m_false_num {0};
        std::size_t m_null_num {0};

        bool operator==(const JsonCounter& jsonCounter) const {
            return (
                this->m_object_num == jsonCounter.m_object_num &&
                this->m_object_size == jsonCounter.m_object_size &&
                this->m_array_num == jsonCounter.m_array_num &&
                this->m_array_size == jsonCounter.m_array_size &&
                this->m_string_num == jsonCounter.m_string_num &&
                this->m_number_num == jsonCounter.m_number_num &&
                this->m_true_num == jsonCounter.m_true_num &&
                this->m_false_num == jsonCounter.m_false_num &&
                this->m_null_num == jsonCounter.m_null_num
            );
        }
    };

    /**
     * @brief 解析器和序列化器的基类
     */
    class JsonBase{
    public:
        virtual ~JsonBase() = default;
    };

    /**
     * @brief 字符类型的基类
     */
    class StringBase{
    public:
        virtual ~StringBase() = default;
        virtual std::string get_string() = 0;
    };


    /**
     * @brief 不支持的操作异常类
     */
    class NotSupportException final : public std::runtime_error{
    public:
        NotSupportException(): std::runtime_error{ "unknow support" }{}
        explicit NotSupportException(const std::string& str): std::runtime_error(str){}
    };

    /**
     * @brief 失败的操作异常类
     */
    class FailException final : public std::runtime_error{
    public:
        FailException(): std::runtime_error{ "unknow fail" }{}
        explicit FailException(const std::string& str): std::runtime_error(str){}
    };


    /**
     * @brief 测试基类，所有测试类都需要继承自此类
     */
    class TestBase{
    public:
        /**
         * @brief 反序列化函数，将字符串转换为JsonBase对象
         * @param str 需要反序列化的字符串
         * @return 生成的解析器指针
         */
        virtual std::shared_ptr<JsonBase> deserialize(const std::string & str) = 0;

        /**
         * @brief 序列化函数，将JsonBase对象转换为字符串
         * @param jsonBase 需要序列化的JsonBase对象
         * @return 生成的字符串指针
         */
        virtual std::shared_ptr<StringBase> serialize(std::shared_ptr<JsonBase> jsonBase) = 0;

        /**
         * @brief 美化序列化函数，将JsonBase对象转换为格式化的字符串
         * @param jsonBase 需要序列化的JsonBase对象
         * @return 生成的格式化字符串指针
         */
        virtual std::shared_ptr<StringBase> serialize_pretty(std::shared_ptr<JsonBase> jsonBase) = 0;

        /**
         * @brief 计数器函数，用于统计JsonBase对象中的元素数量
         * @param jsonBase 需要统计的JsonBase对象
         * @return 统计结果的JsonCounter对象
         */
        virtual JsonCounter count_element(std::shared_ptr<JsonBase> jsonBase) = 0;

        /**
         * @brief 向当前的JsonBase对象（键值对）添加子元素
         * @param jsonBase 需要添加子元素的JsonBase对象
         * @param key 子元素的键名
         * @param value 子元素的JsonBase对象
         */
        virtual void add_child_to_object(std::shared_ptr<JsonBase> jsonBase, const std::string& key, std::shared_ptr<JsonBase> value) = 0;

        /**
         * @brief 向当前的JsonBase对象的数组中添加子元素
         * @param jsonBase 需要添加子元素的JsonBase对象
         * @param index 数组中子元素的索引位置
         * @param value 子元素的JsonBase对象
         */
        virtual void add_child_to_array(std::shared_ptr<JsonBase> jsonBase, const std::size_t& index,  std::shared_ptr<JsonBase> value) = 0;

        /**
         * @brief 从当前的JsonBase对象中删除指定的子元素
         * @param jsonBase 需要删除子元素的JsonBase对象
         * @param key 子元素的键名
         */
        virtual void delete_child_from_object(std::shared_ptr<JsonBase> jsonBase, const std::string& key) = 0;

        /**
         * @brief 从当前的JsonBase对象的数组中删除指定的子元素
         * @param jsonBase 需要删除子元素的JsonBase对象
         * @param index 数组中子元素的索引位置
         */
        virtual void delete_child_from_array(std::shared_ptr<JsonBase> jsonBase, const std::size_t& index) = 0;

        /**
         * @brief 从当前的JsonBase对象中获取指定键名的子元素
         * @param jsonBase 需要获取子元素的JsonBase对象
         * @param str 子元素的键名
         * @return 返回获取到的子元素的JsonBase对象指针
         */
        virtual std::shared_ptr<JsonBase> get_child_from_object(std::shared_ptr<JsonBase> jsonBase, const std::string & str) = 0;

        /**
         * @brief 从当前的JsonBase对象的数组中获取指定索引位置的子元素
         * @param jsonBase 需要获取子元素的JsonBase对象
         * @param index 数组中子元素的索引位置
         * @return 返回获取到的子元素的JsonBase对象指针
         */
        virtual std::shared_ptr<JsonBase> get_child_from_array(std::shared_ptr<JsonBase> jsonBase, const std::size_t& index) = 0;

        /**
         * @brief 获取JsonBase对象中浮点数类型的值
         * @param jsonBase 需要获取值的JsonBase对象
         * @return 返回获取到的浮点数值
         */
        virtual double get_value_as_double(std::shared_ptr<JsonBase> jsonBase) = 0;

        /**
         * @brief 获取JsonBase对象中布尔类型的值
         * @param jsonBase 需要获取值的JsonBase对象
         * @return 返回获取到的布尔值
         */
        virtual bool get_value_as_bool(std::shared_ptr<JsonBase> jsonBase) = 0;

        /**
         * @brief 获取JsonBase对象中长整数类型的值
         * @param jsonBase 需要获取值的JsonBase对象
         * @return 返回获取到的长整数值
         */
        virtual long long get_value_as_int64(std::shared_ptr<JsonBase> jsonBase) = 0;

        /**
         * @brief 获取JsonBase对象中字符串类型的值
         * @param jsonBase 需要获取值的JsonBase对象
         * @return 返回获取到的字符串值
         */
        virtual std::string get_value_as_string(std::shared_ptr<JsonBase> jsonBase) = 0;

        /**
         * @brief 检查JsonBase对象的值是否为null
         * @param jsonBase 需要检查的JsonBase对象
         * @return 返回true如果值为null，否则返回false
         */
        virtual bool value_is_null(std::shared_ptr<JsonBase> jsonBase) = 0;

        virtual ~TestBase() = default;
    };

    class TestClassFactory {
    public:
        TestClassFactory(const TestClassFactory&) = delete;
        TestClassFactory& operator=(const TestClassFactory&) = delete;

        /**
         * @brief 获取单例实例
         * @return 返回TestClassFactory的单例实例
         */
        static TestClassFactory& instance() {
            static TestClassFactory s_instance;
            return s_instance;
        }

        template <typename T>
        void register_class(const std::string& libName) {
            m_dict[libName] = std::static_pointer_cast<TestBase>(std::make_shared<T>());
        }
        std::map<std::string, std::shared_ptr<TestBase>> m_dict;

    private:
        TestClassFactory() = default;
    };


}
