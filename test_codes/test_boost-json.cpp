// 导入模版类头文件
#include "model_class.h"

// 导入你的库的头文件
#include "boost/json.hpp"

/* ------------------------------------------- 1 ------------------------------------------- */
///////////////////////////////////////////////////////////////////////////////////////////////
//////// 重写2个类型


//// 1.1. 重写JsonBase类，内部存放你的库的，JSON类型。类名任意。
class BoostJsonObj : public JsonBase{
public:
    // 存放你的库中，操作JSON需要使用的类对象
    boost::json::value json;
};

//// 1.2. 重写StringBase类，内部存放你的库的字符串类。如果支持std::string,就直接使用std::string.
class BoostJsonStr : public StringBase{
public:
    // 存放你库 最常用的 字符串类型
    std::string str;
    // 重写虚函数，将你的库的字符串转换成标准字符串，此步骤不计时间。
    std::string getString() override{ return str; }
};

///////////////////////////////////////////////////////////////////////////////////////////////


/* ------------------------------------------- 2 ------------------------------------------- */
///////////////////////////////////////////////////////////////////////////////////////////////
//////// 编写 统计子元素数量的静态函数

/*
函数名和类型随便写，静态函数是为了防止与外部重名，需要将数据存入JsonCounter对象。
实现方式不止一种，本操作不记录耗时，只看JsonCounter对象结果是否正确

JsonCounter对象内部有8个属性，需要你通过函数填写：
objectNum : 全体元素中，对象类型的数量
objectChildNum : 全体元素中，对象类型的子元素数量的累加和
arrayNum : 全体元素中，数组类型的数量
arrayChildNum : 全体元素中，数组类型的子元素数量的累加和
stringNum : 全体元素中，字符串类型的数量
numberNum : 全体元素中，数值类型的数量
boolenNum : 全体元素中，bool类型的数量
nullNum : 全体元素中，null的数量
*/ 
// 例：
static void get_element_num(const boost::json::value& json, JsonCounter& counter){
    switch (json.kind())
    {
    case boost::json::kind::object:
        {
            const boost::json::object &o = json.get_object();
            for(auto it = o.begin(); o.end() != it; ++it){
                get_element_num(it->value(), counter);
                counter.objectChildNum += 1;
            }
            counter.objectNum += 1;
        }
        break;
    case boost::json::kind::array:
        {
            const boost::json::array &a = json.get_array();
            for(auto it = a.begin(); a.end() != it; ++it){
                get_element_num(*it, counter);
                counter.arrayChildNum += 1;
            }
            counter.arrayNum += 1;
        }
        break;
    case boost::json::kind::string:
        counter.stringNum += 1;
        break;
    case boost::json::kind::uint64:
    case boost::json::kind::int64:
    case boost::json::kind::double_:
        counter.numberNum += 1;
        break;
    case boost::json::kind::bool_:
        counter.boolenNum += 1;
        break;
    case boost::json::kind::null:
        counter.nullNum += 1;
        break;
    }
};

/*
    具体例子请参考 test_cpp-jsonlib.cpp
*/

///////////////////////////////////////////////////////////////////////////////////////////////


/* ------------------------------------------- 3 ------------------------------------------- */
///////////////////////////////////////////////////////////////////////////////////////////////
//////// 编写测试类的子类，实现全部虚函数

/*
    内部可以抛出2种异常：
    throw FailException {};  表示操作失败。请在任何 非法操作/JSON文本格式错误/等 异常发生时抛出此异常，用于测试库的准确性
    throw NotSupportException {}; 表示你的库不支持这种操作，比如 输出美化过的JSON文本/增删改查。请在任何不支持的操作函数内部抛出此异常。
*/


class BoostJsonTest : public TestBase{
    

    // 1. 重写 反序列化函数   样例如下
    std::shared_ptr<JsonBase> deserialize(const std::string & str) override{
        // 定义 第1步 定义的 JSON子类指针
        auto json_ptr = std::make_shared<BoostJsonObj>();
        try{
            boost::json::error_code ec;
            json_ptr->json = boost::json::parse(str, ec);
            // 用 JSON子类指针 内部的 JSON数据对象 反序列化字符串
            if(ec) throw FailException {}; 
        }catch(...){ 
            // 转换失败时，务必抛出 FailException异常
            throw FailException {}; 
        }
        // 将 JSON子类指针 转换成 JSON父类指针 并返回
        return std::static_pointer_cast<JsonBase>(json_ptr);
    }

    // 2. 重写 序列化函数
    std::shared_ptr<StringBase> serialize(std::shared_ptr<JsonBase> json_base_ptr) override{
        // 定义 第1步 定义的 String子类指针
        auto str_ptr = std::make_shared<BoostJsonStr>();
        try{
            // 将输入的 JSON父类指针 转换成 第1步定义的 JSON子类指针
            auto json_ptr = std::dynamic_pointer_cast<BoostJsonObj>(json_base_ptr);
            // 将 JSON子类指针 内部的 JSON数据对象 序列化成字符串，并赋值给 String子类指针 内部的字符串变量
            str_ptr->str =  boost::json::serialize(json_ptr->json);
        }catch(...){ 
            throw FailException {}; 
        }
        // 将 String子类指针 转换成 String父类指针 并返回
        return std::static_pointer_cast<StringBase>(str_ptr);
    }

    // 3. 重写 序列化+美化函数
    std::shared_ptr<StringBase> serialize_pretty(std::shared_ptr<JsonBase> json_base_ptr) override{
        throw NotSupportException {};
        auto str_ptr = std::make_shared<BoostJsonStr>();
        return std::static_pointer_cast<StringBase>(str_ptr);
    }


    // 4. 重写计数器函数函数。 本函数必须正确无误，否则不进行其他测试。
    JsonCounter count_element(std::shared_ptr<JsonBase> json_base_ptr) override{
        // 第2步有解释，一个简单的存数值类型，用于记录内部各种元素的数量
        JsonCounter counter;
        try{
            auto json_ptr = std::dynamic_pointer_cast<BoostJsonObj>(json_base_ptr);
            //（使用第2步中定义的函数，统计内部元素数量。）
            get_element_num(json_ptr->json, counter);
        }catch(...){
            // 发生错误时可以返回
            throw FailException {}; 
        }
        return counter;
    }

    // 5. 在Object类型内部添加子元素，如果类型不是OBJECT，请抛出异常。请使用拷贝赋值/构造，而非移动。
    void add_child_to_object(std::shared_ptr<JsonBase> json_base_ptr, const std::string& key, std::shared_ptr<JsonBase> value) override{
        // 如果不支持添加内容，请直接抛出 NotSupportException 异常。
        try{
            auto json_ptr = std::dynamic_pointer_cast<BoostJsonObj>(json_base_ptr);
            auto value_ptr = std::dynamic_pointer_cast<BoostJsonObj>(value);
            // if(!json_ptr->json.is_object()) throw FailException {}; 
            json_ptr->json.as_object()[key] = value_ptr->json;
        }catch(...){ 
            throw FailException {}; 
        }
    }

    // 6. 在Array类型内部添加子元素，如果类型不是Array，请抛出异常。请使用拷贝赋值/构造，而非移动。
    void add_child_to_array(std::shared_ptr<JsonBase> json_base_ptr, const size_t& index,  std::shared_ptr<JsonBase> value) override{
        // 如果不支持添加内容，请直接抛出 NotSupportException 异常。
        try{
            auto json_ptr = std::dynamic_pointer_cast<BoostJsonObj>(json_base_ptr);
            auto value_ptr = std::dynamic_pointer_cast<BoostJsonObj>(value);
            // if(!json_ptr->json.is_array()) throw FailException {}; 
            json_ptr->json.as_array().insert(json_ptr->json.as_array().begin()+index, value_ptr->json);
        }catch(...){ 
            throw FailException {}; 
        }
    }

    // 7. 删除Object的指定子元素
    void delete_child_from_object(std::shared_ptr<JsonBase> json_base_ptr, const std::string& key) override{
        // 如果不支持添加内容，请直接抛出 NotSupportException 异常。
        try{
            auto json_ptr = std::dynamic_pointer_cast<BoostJsonObj>(json_base_ptr);
            // if(!json_ptr->json.is_object()) throw FailException {}; 
            json_ptr->json.as_object().erase(key);
        }catch(...){ 
            throw FailException {}; 
        }
    }

    // 8. 删除Array的指定子元素
    void delete_child_from_array(std::shared_ptr<JsonBase> json_base_ptr, const size_t& index) override{
        // 如果不支持添加内容，请直接抛出 NotSupportException 异常。
        try{
            auto json_ptr = std::dynamic_pointer_cast<BoostJsonObj>(json_base_ptr);
            json_ptr->json.as_array().erase(json_ptr->json.as_array().begin()+index);
        }catch(...){ 
            throw FailException {}; 
        }
    }

    // 9. 获取Object子元素的方法，请使用拷贝而非移动
    std::shared_ptr<JsonBase> get_child_from_object(std::shared_ptr<JsonBase> json_base_ptr, const std::string & str) override{
        // 如果不支持添加内容，请直接抛出 NotSupportException 异常。
        auto json_ptr = std::make_shared<BoostJsonObj>();
        try{
            auto old_ptr = std::dynamic_pointer_cast<BoostJsonObj>(json_base_ptr);
            json_ptr->json = old_ptr->json.at(str);
            return std::static_pointer_cast<JsonBase>(json_ptr);
        }catch(...){ 
            throw FailException {}; 
        }
        return std::static_pointer_cast<JsonBase>(json_ptr);
    }

    // 10. 获取Array子元素的方法，请使用拷贝而非移动
    std::shared_ptr<JsonBase> get_child_from_array(std::shared_ptr<JsonBase> json_base_ptr, const size_t& index) override{
        // 如果不支持添加内容，请直接抛出 NotSupportException 异常。
        auto json_ptr = std::make_shared<BoostJsonObj>();
        try{
            auto old_ptr = std::dynamic_pointer_cast<BoostJsonObj>(json_base_ptr);
            json_ptr->json = old_ptr->json.at(index);
            return std::static_pointer_cast<JsonBase>(json_ptr);
        }catch(...){ 
            throw FailException {}; 
        }
        return std::static_pointer_cast<JsonBase>(json_ptr);
    }

    // 11. 值类型，获取浮点型的值
    double get_value_as_double(std::shared_ptr<JsonBase> json_base_ptr) override{
        // 请务必支持。
        try{
            auto json_ptr = std::dynamic_pointer_cast<BoostJsonObj>(json_base_ptr);
            return json_ptr->json.as_double();
        }catch(...){ 
            throw FailException {}; 
        }
        return 0;
    }

    // 12. 值类型，获取布尔类型的值
    bool get_value_as_bool(std::shared_ptr<JsonBase> json_base_ptr) override{
        // 请务必支持。
        try{
            auto json_ptr = std::dynamic_pointer_cast<BoostJsonObj>(json_base_ptr);
            return json_ptr->json.as_bool();
        }catch(...){ 
            throw FailException {}; 
        }
        return 0;
    }

    // 13. 值类型，获取长整数类型的值
    long long get_value_as_int64(std::shared_ptr<JsonBase> json_base_ptr) override{
        // 请务必支持。
        try{
            auto json_ptr = std::dynamic_pointer_cast<BoostJsonObj>(json_base_ptr);
            return json_ptr->json.as_int64();
        }catch(...){ 
            throw FailException {}; 
        }
        return 0;
    }

    // 14. 值类型，获取标准字符串类型的值
    std::string get_value_as_string(std::shared_ptr<JsonBase> json_base_ptr) override{
        // 请务必支持，可以先转const char* 然后转std::string。
        try{
            auto json_ptr = std::dynamic_pointer_cast<BoostJsonObj>(json_base_ptr);
            return std::string {json_ptr->json.as_string().c_str(), json_ptr->json.as_string().size()};
        }catch(...){ 
            throw FailException {}; 
        }
        return "";
    }

    // 15. 值类型，判断内部是否是null值。如果是null请返回null，如果不是请抛出异常或返回false
    bool value_is_null(std::shared_ptr<JsonBase> json_base_ptr) override{
        try{
            auto json_ptr = std::dynamic_pointer_cast<BoostJsonObj>(json_base_ptr);
            return json_ptr->json.is_null();
        }catch(...){ 
            throw FailException {}; 
        }
        return 0;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////



/* ------------------------------------------- 3 ------------------------------------------- */
///////////////////////////////////////////////////////////////////////////////////////////////
//////// 使用下面这个宏，注册你的测试类对象

// 参数1：你库的名字，随便写   参数2: 你的测试子类名，是第3步中的类名，不能加双引号
REGISTER_CLASS("boost-json", BoostJsonTest)


///////////////////////////////////////////////////////////////////////////////////////////////
