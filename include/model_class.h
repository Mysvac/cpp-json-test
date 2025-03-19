// model_class.h
#include <string>
#include <memory>
#include <exception>
#include <stdexcept>

class JsonCounter{
public:
    size_t objectNum {0};
    size_t arrayNum {0};
    size_t stringNum {0};
    size_t numberNum {0};
    size_t boolenNum {0};
    size_t nullNum {0};
};

// 不支持的操作，请直接抛出此异常
class NotSupportException: std::runtime_error{
public:
    NotSupportException(): std::runtime_error("NotSupportException"){}
    NotSupportException(const std::string& str): std::runtime_error(str){}
};

// 操作失败时，请支持此异常
class FailException: std::runtime_error{
public:
    FailException(): std::runtime_error("NotSupportException"){}
    FailException(const std::string& str): std::runtime_error(str){}
};

// 父类，纯储JSON反序列化后的对象
class JsonBase{  };

// 父类，存储字符串后的序列化
class StringBase{
    virtual std::string getString() = 0; 
};

class TestBase{
public:
    // 反序列化，输入字符指针
    virtual std::shared_ptr<JsonBase> unserialize_char(const char* str) = 0;

    // 反序列化，输入标准字符串
    virtual std::shared_ptr<JsonBase> unserialize_str(const std::string & str) = 0;

    // 序列化
    virtual std::shared_ptr<StringBase> serialize() = 0;

    // 序列化且美化（请添加换行内容）
    virtual std::shared_ptr<StringBase> serialize_prettify() = 0;

    // 任意方式统计内部元素个数，用于解析数据正确性
    virtual JsonCounter count_element(std::shared_ptr<JsonBase>) = 0;

    virtual double get_first_as_double(std::shared_ptr<JsonBase>) = 0;

    virtual bool get_first_as_bool(std::shared_ptr<JsonBase>) = 0;

    virtual long long get_first_as_int64(std::shared_ptr<JsonBase>) = 0;

    virtual std::string get_first_as_string(std::shared_ptr<JsonBase>) = 0;

    virtual void add_child_to_object(std::shared_ptr<JsonBase>, std::string& key, std::shared_ptr<JsonBase> value) = 0;

    virtual void add_child_to_array(std::shared_ptr<JsonBase>, std::shared_ptr<JsonBase> value) = 0;

};

