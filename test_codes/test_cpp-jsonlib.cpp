// 导入模版类头文件
#include "model_class.h"

// 导入你的库
#include "jsonlib.h"

// 写子类，重新三个父类

// 1. 存储JSON实例化对象的容器
class CppJsonlibObj : public JsonBase{
public:
    // 存放库中JSON可操作对象的类型
    Json::JsonBasic jsonBasic;
};

// 2. 纯储字符串对象的容器（有点库有自己的字符串，但是最终通过getString转换成标准字符串）。
class CppJsonlibStr : public StringBase{
public:
    // 重写方法，将你的库的字符串转换成标准字符串，此步骤不计时间。
    std::string getString() override{ return str; }
    // 存放你库支持的字符串类型
    std::string str;
};


// 3. 编写函数，用于完备性测试，递归地获取内部各种元素的数量，统计各种元素的个数，并存入counter
static void get_element_num(const Json::JsonBasic& jsonBasic, JsonCounter& counter){
    switch (jsonBasic.type())
    {
    case Json::JsonType::OBJECT:
        {
            const Json::JsonBasic::Map& map = jsonBasic.getMapConst();
            for(const auto& it : map){
                get_element_num(it.second, counter);
                counter.objectChildNum += 1;
            }
            counter.objectNum += 1;
        }
        break;
    case Json::JsonType::ARRAY:
        {
            const Json::JsonBasic::List& list = jsonBasic.getListConst();
            for(const auto& it : list){
                get_element_num(it, counter);
                counter.arrayChildNum += 1;
            }
            counter.arrayNum += 1;
        }
        break;
    case Json::JsonType::STRING:
        counter.stringNum += 1;
        break;
    case Json::JsonType::NUMBER:
        counter.numberNum += 1;
        break;
    case Json::JsonType::BOOLEN:
        counter.boolenNum += 1;
        break;
    case Json::JsonType::ISNULL:
        counter.nullNum += 1;
        break;
    }
}

// 4. 继承测试类，重写方法
class CppJsonlibTest : public TestBase{

    // 重写 反序列化函数
    std::shared_ptr<JsonBase> unserialize(const std::string & str) override{
        auto json_ptr = std::make_shared<CppJsonlibObj>();
        try{
            json_ptr->jsonBasic = Json::JsonBasic {str};
        }catch(...){ 
            throw FailException {}; 
        }
        return std::static_pointer_cast<JsonBase>(json_ptr);
    }

    // 重写 序列化函数
    std::shared_ptr<StringBase> serialize(std::shared_ptr<JsonBase> jsonBase) override{
        auto str_ptr = std::make_shared<CppJsonlibStr>();
        try{
            auto json_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(jsonBase);
            str_ptr->str = json_ptr->jsonBasic.serialize();
        }catch(...){ 
            throw FailException {}; 
        }
        return std::static_pointer_cast<StringBase>(str_ptr);
    }

    // 重写 序列化+美化函数，如果不支持美化，请直接抛出NotSupportException异常
    std::shared_ptr<StringBase> serialize_pretty(std::shared_ptr<JsonBase> jsonBase) override{
        auto str_ptr = std::make_shared<CppJsonlibStr>();
        try{
            auto json_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(jsonBase);
            str_ptr->str = json_ptr->jsonBasic.serialize_pretty();
        }catch(...){ 
            throw FailException {}; 
        }
        return std::static_pointer_cast<StringBase>(str_ptr);
    }

    // 重写极速器函数。 使用任意方式，获得JSON对象后，统计内部各种元素的个数，存入JsonCounter对象并返回
    JsonCounter count_element(std::shared_ptr<JsonBase> jsonBase) override{
        JsonCounter counter;
        try{
            auto json_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(jsonBase);
            get_element_num(json_ptr->jsonBasic, counter);
            return counter;
        }catch(...){
            return JsonCounter{};
        }
        return JsonCounter{};
    }

    // 添加字元素的方法，注意，全都使用拷贝添加，不要使用移动
    void add_child_to_object(std::shared_ptr<JsonBase> jsonBase, const std::string& key, std::shared_ptr<JsonBase> value) override{
        try{
            auto json_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(jsonBase);
            auto value_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(value);
            json_ptr->jsonBasic[key] = value_ptr->jsonBasic;
        }catch(...){ 
            throw FailException {}; 
        }
    }

    // 添加字元素的方法，注意，全都使用拷贝添加，不要使用移动
    void add_child_to_array(std::shared_ptr<JsonBase> jsonBase, const size_t& index,  std::shared_ptr<JsonBase> value) override{
        try{
            auto json_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(jsonBase);
            auto value_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(value);
            json_ptr->jsonBasic.insert(index, value_ptr->jsonBasic);
        }catch(...){ 
            throw FailException {}; 
        }
    }

    // 删除子元素的方法
    void delete_child_from_object(std::shared_ptr<JsonBase> jsonBase, const std::string& key) override{
        try{
            auto json_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(jsonBase);
            json_ptr->jsonBasic.erase(key);
        }catch(...){ 
            throw FailException {}; 
        }
    }

    // 删除子元素的方法
    void delete_child_from_array(std::shared_ptr<JsonBase> jsonBase, const size_t& index) override{
        try{
            auto json_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(jsonBase);
            json_ptr->jsonBasic.erase(index);
        }catch(...){ 
            throw FailException {}; 
        }
    }

    // 获取子元素的方法
    std::shared_ptr<JsonBase> get_child_from_object(std::shared_ptr<JsonBase> jsonBase, const std::string & str) override{
        auto json_ptr = std::make_shared<CppJsonlibObj>();
        try{
            auto old_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(jsonBase);
            json_ptr->jsonBasic = old_ptr->jsonBasic.at(str);
            return std::static_pointer_cast<JsonBase>(json_ptr);
        }catch(...){ 
            throw FailException {}; 
        }
        return std::static_pointer_cast<JsonBase>(json_ptr);
    }

    // 获取子元素的方法
    std::shared_ptr<JsonBase> get_child_from_array(std::shared_ptr<JsonBase> jsonBase, const size_t& index) override{
        auto json_ptr = std::make_shared<CppJsonlibObj>();
        try{
            auto old_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(jsonBase);
            json_ptr->jsonBasic = old_ptr->jsonBasic.at(index);
            return std::static_pointer_cast<JsonBase>(json_ptr);
        }catch(...){ 
            throw FailException {}; 
        }
        return std::static_pointer_cast<JsonBase>(json_ptr);
    }

    // 值类型，获取浮点型的值
    double get_value_as_double(std::shared_ptr<JsonBase> jsonBase) override{
        try{
            auto json_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(jsonBase);
            return json_ptr->jsonBasic.as_double();
        }catch(...){ 
            throw FailException {}; 
        }
        return 0;
    }

    // 值类型，获取布尔类型的值
    bool get_value_as_bool(std::shared_ptr<JsonBase> jsonBase) override{
        try{
            auto json_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(jsonBase);
            return json_ptr->jsonBasic.as_bool();
        }catch(...){ 
            throw FailException {}; 
        }
        return 0;
    }

    // 值类型，获取长整数类型的值
    long long get_value_as_int64(std::shared_ptr<JsonBase> jsonBase) override{
        try{
            auto json_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(jsonBase);
            return json_ptr->jsonBasic.as_int64();
        }catch(...){ 
            throw FailException {}; 
        }
        return 0;
    }

    // 值类型，获取标准字符串类型的值
    std::string get_value_as_string(std::shared_ptr<JsonBase> jsonBase) override{
        try{
            auto json_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(jsonBase);
            return json_ptr->jsonBasic.as_string();
        }catch(...){ 
            throw FailException {}; 
        }
        return "";
    }

    // 值类型，判断内部是否是null值。
    bool value_is_null(std::shared_ptr<JsonBase> jsonBase) override{
        try{
            auto json_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(jsonBase);
            return json_ptr->jsonBasic.is_null();
        }catch(...){ 
            throw FailException {}; 
        }
        return 0;
    }

};



// 5. 注册测试类，左边是字符串的类名。 右边是你编写的测试类的子类名，注意不是字符串。
REGISTER_CLASS("cpp-jsonlib", CppJsonlibTest)
