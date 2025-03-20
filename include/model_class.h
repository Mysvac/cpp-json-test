#pragma once

// model_class.h
#include <string>
#include <map>
#include <memory>
#include <exception>
#include <stdexcept>

class JsonCounter{
public:
    size_t objectNum {0};
    size_t objectChildNum { 0 };
    size_t arrayNum {0};
    size_t arrayChildNum { 0 };
    size_t stringNum {0};
    size_t numberNum {0};
    size_t boolenNum {0};
    size_t nullNum {0};

    inline bool operator==(const JsonCounter& jsonCounter){
        return objectNum == jsonCounter.objectNum && 
            objectChildNum == jsonCounter.objectChildNum &&
            arrayNum == jsonCounter.arrayNum &&
            arrayChildNum == jsonCounter.arrayChildNum &&
            stringNum == jsonCounter.stringNum &&
            numberNum == jsonCounter.numberNum &&
            boolenNum == jsonCounter.boolenNum &&
            nullNum == jsonCounter.nullNum;
    }

    inline std::string value(){
        std::string str;
        str += "objectNum :" + std::to_string(objectNum);
        str += "\nobjectChildNum :" + std::to_string(objectChildNum);
        str += "\narrayNum :" + std::to_string(arrayNum);
        str += "\narrayChildNum :" + std::to_string(arrayChildNum);
        str += "\nstringNum :" + std::to_string(stringNum);
        str += "\nnumberNum :" + std::to_string(numberNum);
        str += "\nboolenNum :" + std::to_string(boolenNum);
        str += "\nnullNum :" + std::to_string(nullNum) + "\n";
        return std::move(str);
    }
};

// 库不支持的操作，请直接抛出此异常
class NotSupportException: std::runtime_error{
public:
    NotSupportException(): std::runtime_error("NotSupportException"){}
    NotSupportException(const std::string& str): std::runtime_error(str){}
};

// 任何操作失败，JSON结构错误，JSON类型与运算符不匹配时，请抛出此异常
class FailException: std::runtime_error{
public:
    FailException(): std::runtime_error("NotSupportException"){}
    FailException(const std::string& str): std::runtime_error(str){}
};

// 父类，纯储JSON反序列化后的对象
class JsonBase{ 
public:
    virtual ~JsonBase() = default;
};

// 父类，存储字符串后的序列化
class StringBase{
public:
    virtual ~StringBase() = default;
    virtual std::string getString() = 0; 
};

class TestBase{
public:

    // 反序列化，输入标准字符串，可以使用.c_str()获取字符指针，几乎无额外耗时
    virtual std::shared_ptr<JsonBase> unserialize(const std::string & str) = 0;

    // 序列化
    virtual std::shared_ptr<StringBase> serialize(std::shared_ptr<JsonBase> jsonBase) = 0;

    // 序列化且美化（请添加换行和缩进内容）
    virtual std::shared_ptr<StringBase> serialize_pretty(std::shared_ptr<JsonBase> jsonBase) = 0;

    // 任意方式统计内部元素个数，用于解析数据正确性
    virtual JsonCounter count_element(std::shared_ptr<JsonBase> jsonBase) = 0;

    // 对象类型添加内容
    virtual void add_child_to_object(std::shared_ptr<JsonBase> jsonBase, const std::string& key, std::shared_ptr<JsonBase> value) = 0;

    // 数组类型添加内容
    virtual void add_child_to_array(std::shared_ptr<JsonBase> jsonBase, const size_t& index,  std::shared_ptr<JsonBase> value) = 0;

    // 对象类型删除内容
    virtual void delete_child_from_object(std::shared_ptr<JsonBase> jsonBase, const std::string& key) = 0;

    // 数组类型删除内容
    virtual void delete_child_from_array(std::shared_ptr<JsonBase> jsonBase, const size_t& index) = 0;

    // 对象类型获取内容
    virtual std::shared_ptr<JsonBase> get_child_from_object(std::shared_ptr<JsonBase> jsonBase, const std::string & str) = 0;

    // 对象类型获取内容
    virtual std::shared_ptr<JsonBase> get_child_from_array(std::shared_ptr<JsonBase> jsonBase, const size_t& index) = 0;

    // 获取double类型的值
    virtual double get_value_as_double(std::shared_ptr<JsonBase> jsonBase) = 0;

    // 获取bool类型的值
    virtual bool get_value_as_bool(std::shared_ptr<JsonBase> jsonBase) = 0;

    // 获取long long类型的值
    virtual long long get_value_as_int64(std::shared_ptr<JsonBase> jsonBase) = 0;

    // 获取字符串类型的值
    virtual std::string get_value_as_string(std::shared_ptr<JsonBase> jsonBase) = 0;

    // 判断是不是null
    virtual bool value_is_null(std::shared_ptr<JsonBase> jsonBase) = 0;

    virtual ~TestBase() = default;
};

/* ----------------------------------------------------------- */

class TestClassFactory {
public:
    static TestClassFactory& instance();
    
template <typename T>
void registerClass(const std::string& libName) {
    dict_[libName] = std::static_pointer_cast<TestBase>(std::make_shared<T>());
}
std::map<std::string, std::shared_ptr<TestBase>> dict_;
    
private:
    TestClassFactory() = default;
    TestClassFactory(const TestClassFactory&) = delete;
    TestClassFactory& operator=(const TestClassFactory&) = delete;
};

#define REGISTER_CLASS(libName, className) \
    static void register_##className() { \
        TestClassFactory::instance().registerClass<className>( libName ); \
    } \
    static bool registered_##className = ( register_##className() , true) ;



