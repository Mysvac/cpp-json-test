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

// 3. 继承测试类，重写方法
class CppJsonlibTest : public TestBase{

    // 重写 反序列化方法
    std::shared_ptr<JsonBase> unserialize(const std::string & str) override{
        auto json_ptr = std::make_shared<CppJsonlibObj>();
        try{
            json_ptr->jsonBasic = Json::JsonBasic {str};
        }catch(...){ 
            throw FailException {}; 
        }
        return std::static_pointer_cast<JsonBase>(json_ptr);
    }

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

    JsonCounter count_element(std::shared_ptr<JsonBase> jsonBase) override{
        return JsonCounter{};
    }

    void add_child_to_object(std::shared_ptr<JsonBase> jsonBase, const std::string& key, std::shared_ptr<JsonBase> value) override{
        try{
            auto json_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(jsonBase);
            auto value_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(value);
            json_ptr->jsonBasic[key] = value_ptr->jsonBasic;
        }catch(...){ 
            throw FailException {}; 
        }
    }

    void add_child_to_array(std::shared_ptr<JsonBase> jsonBase, const size_t& index,  std::shared_ptr<JsonBase> value) override{
        try{
            auto json_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(jsonBase);
            auto value_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(value);
            json_ptr->jsonBasic.insert(index, value_ptr->jsonBasic);
        }catch(...){ 
            throw FailException {}; 
        }
    }

    void delete_child_from_object(std::shared_ptr<JsonBase> jsonBase, const std::string& key) override{
        try{
            auto json_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(jsonBase);
            json_ptr->jsonBasic.erase(key);
        }catch(...){ 
            throw FailException {}; 
        }
    }

    void delete_child_from_object(std::shared_ptr<JsonBase> jsonBase, const size_t& index) override{
        try{
            auto json_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(jsonBase);
            json_ptr->jsonBasic.erase(index);
        }catch(...){ 
            throw FailException {}; 
        }
    }

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

    double get_value_as_double(std::shared_ptr<JsonBase> jsonBase) override{
        try{
            auto json_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(jsonBase);
            return json_ptr->jsonBasic.as_double();
        }catch(...){ 
            throw FailException {}; 
        }
        return 0;
    }

    bool get_value_as_bool(std::shared_ptr<JsonBase> jsonBase) override{
        try{
            auto json_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(jsonBase);
            return json_ptr->jsonBasic.as_bool();
        }catch(...){ 
            throw FailException {}; 
        }
        return 0;
    }

    long long get_value_as_int64(std::shared_ptr<JsonBase> jsonBase) override{
        try{
            auto json_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(jsonBase);
            return json_ptr->jsonBasic.as_int64();
        }catch(...){ 
            throw FailException {}; 
        }
        return 0;
    }

    std::string get_value_as_string(std::shared_ptr<JsonBase> jsonBase) override{
        try{
            auto json_ptr = std::dynamic_pointer_cast<CppJsonlibObj>(jsonBase);
            return json_ptr->jsonBasic.as_string();
        }catch(...){ 
            throw FailException {}; 
        }
        return 0;
    }

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

REGISTER_CLASS("cpp-jsonlib", CppJsonlibTest)
