#include <test_macros.hpp>
//------- 导入头文件 ---------
#include "nlohmann/json.hpp"
//--------------------------
import std;
import jtu;
//------ 导入模块文件---------

//--------------------------

/* ----------------------------- 1 ----------------------------- */
//////////////////////////////////////////////////////////////////
/////// 重写2个类型

//// 1.1. 重写JsonBase类，内部存放你的库的，JSON类型。类名任意。
class NlohmannJsonObj final : public jtu::JsonBase{
public:
    // 存放你的库中，操作JSON需要使用的类对象
    nlohmann::json json;
};

//// 1.2. 重写StringBase类，内部存放你的库的字符串类。如果支持std::string,就直接使用std::string.
class NlohmannJsonStr final : public jtu::StringBase{
public:
    // 存放你库 最常用的 字符串类型
    std::string str;
    // 重写虚函数，将你的库的字符串转换成标准字符串，此步骤不计时间。
    std::string get_string() override { return str; }
};

///////////////////////////////////////////////////////////////////



/* ------------------------------------------- 2 ------------------------------------------- */
///////////////////////////////////////////////////////////////////////////////////////////////
//////// 编写 统计子元素数量的静态函数

/*
函数名和类型随便写，静态函数是为了防止与外部重名，需要将数据存入JsonCounter对象。
实现方式不止一种，本操作不记录耗时，只看JsonCounter对象结果是否正确

JsonCounter对象内部有8个属性，需要你通过函数填写：
object_num : 全体元素中，对象类型的数量
object_size : 全体元素中，对象类型的子元素数量的累加和
array_num : 全体元素中，数组类型的数量
array_size : 全体元素中，数组类型的子元素数量的累加和
string_num : 全体元素中，字符串类型的数量
number_num : 全体元素中，数值类型的数量
false_num : 全体元素中，false 的数量
true_num : 全体元素中，true 的数量
null_num : 全体元素中，null的数量
*/
// 例：
static void get_element_num(const nlohmann::json& json, jtu::JsonCounter& counter){
    switch (json.type()){
        case nlohmann::json::value_t::object:
            for (auto it = json.begin(); it != json.end(); ++it) {
                get_element_num(it.value(),counter);
                counter.m_object_size += 1;
            }
            counter.m_object_num += 1;
            break;
        case nlohmann::json::value_t::array:
            for (auto& it : json){
                get_element_num(it, counter);
                counter.m_array_size += 1;
            }
            counter.m_array_num += 1;
            break;
        case nlohmann::json::value_t::string:
            counter.m_string_num += 1;
            break;
        case nlohmann::json::value_t::number_integer:
        case nlohmann::json::value_t::number_unsigned:
        case nlohmann::json::value_t::number_float:
            counter.m_number_num += 1;
            break;
        case nlohmann::json::value_t::boolean:
            json.get<bool>() ? counter.m_true_num += 1 : counter.m_false_num += 1;
            break;
        case nlohmann::json::value_t::null:
            counter.m_null_num += 1;
            break;
        default: break;
    }
};

/*
    具体例子请参考 src/libs/vct-tools-json.cpp
*/

///////////////////////////////////////////////////////////////////////////////////////////////


/* ------------------------------------------- 3 ------------------------------------------- */
///////////////////////////////////////////////////////////////////////////////////////////////
//////// 编写测试类的子类，实现全部虚函数

/*
    内部可以抛出2种异常：
    throw jtu::FailException {};  表示操作失败。请在任何 非法操作/JSON文本格式错误/等 异常发生时抛出此异常，用于测试库的准确性
    throw jtu::NotSupportException {}; 表示你的库不支持这种操作，比如 输出美化过的JSON文本/增删改查。请在任何不支持的操作函数内部抛出此异常。
*/


class NlohmannJsonTest final : public jtu::TestBase{


    // 1. 重写 反序列化函数   样例如下
    std::shared_ptr<jtu::JsonBase> deserialize(const std::string & str) override{
        // 定义 第1步 定义的 JSON子类指针
        const auto json_ptr = std::make_shared<NlohmannJsonObj>();
        try{
            // 用 JSON子类指针 内部的 JSON数据对象 反序列化字符串
            json_ptr->json = nlohmann::json::parse(str);
        }catch(...){
            // 转换失败时，务必抛出 FailException异常
            throw jtu::FailException {};
        }
        // 将 JSON子类指针 转换成 JSON父类指针 并返回
        return std::static_pointer_cast<jtu::JsonBase>(json_ptr);
    }

    // 2. 重写 序列化函数
    std::shared_ptr<jtu::StringBase> serialize(std::shared_ptr<jtu::JsonBase> json_base_ptr) override{
        // 定义 第1步 定义的 String子类指针
        const auto str_ptr = std::make_shared<NlohmannJsonStr>();
        try{
            // 将输入的 JSON父类指针 转换成 第1步定义的 JSON子类指针
            const auto json_ptr = std::dynamic_pointer_cast<NlohmannJsonObj>(json_base_ptr);
            // 将 JSON子类指针 内部的 JSON数据对象 序列化成字符串，并赋值给 String子类指针 内部的字符串变量
            str_ptr->str = json_ptr->json.dump();
        }catch(...){
            throw jtu::FailException {};
        }
        // 将 String子类指针 转换成 String父类指针 并返回
        return std::static_pointer_cast<jtu::StringBase>(str_ptr);
    }

    // 3. 重写 序列化+美化函数
    std::shared_ptr<jtu::StringBase> serialize_pretty(std::shared_ptr<jtu::JsonBase> json_base_ptr) override{
        // 如果不支持美化（带缩进和换行的序列化），请直接抛出 NotSupportException 异常。
        // throw NotSupportException {};
        const auto str_ptr = std::make_shared<NlohmannJsonStr>();
        try{
            const auto json_ptr = std::dynamic_pointer_cast<NlohmannJsonObj>(json_base_ptr);
            str_ptr->str = json_ptr->json.dump(2);
        }catch(...){
            throw jtu::FailException {};
        }
        return std::static_pointer_cast<jtu::StringBase>(str_ptr);
    }


    // 4. 重写计数器函数函数。 本函数必须正确无误，否则不进行其他测试。
    jtu::JsonCounter count_element(std::shared_ptr<jtu::JsonBase> json_base_ptr) override{
        // 第2步有解释，一个简单的存数值类型，用于记录内部各种元素的数量
        jtu::JsonCounter counter;
        try{
            const auto json_ptr = std::dynamic_pointer_cast<NlohmannJsonObj>(json_base_ptr);
            //（使用第2步中定义的函数，统计内部元素数量。）
            get_element_num(json_ptr->json, counter);
        }catch(...){
            // 发生错误时可以返回
            throw jtu::FailException {};
        }
        return counter;
    }

    // 5. 在Object类型内部添加子元素，如果类型不是OBJECT，请抛出异常。请使用拷贝赋值/构造，而非移动。
    void add_child_to_object(std::shared_ptr<jtu::JsonBase> json_base_ptr, const std::string& key, std::shared_ptr<jtu::JsonBase> value) override{
        // 如果不支持添加内容，请直接抛出 NotSupportException 异常。
        try{
            const auto json_ptr = std::dynamic_pointer_cast<NlohmannJsonObj>(json_base_ptr);
            const auto value_ptr = std::dynamic_pointer_cast<NlohmannJsonObj>(value);
            json_ptr->json[key] = value_ptr->json;
        }catch(...){
            throw jtu::FailException {};
        }
    }

    // 6. 在Array类型内部添加子元素，如果类型不是Array，请抛出异常。请使用拷贝赋值/构造，而非移动。
    void add_child_to_array(std::shared_ptr<jtu::JsonBase> json_base_ptr, const std::size_t& index,  std::shared_ptr<jtu::JsonBase> value) override{
        // 如果不支持添加内容，请直接抛出 NotSupportException 异常。
        try{
            const auto json_ptr = std::dynamic_pointer_cast<NlohmannJsonObj>(json_base_ptr);
            const auto value_ptr = std::dynamic_pointer_cast<NlohmannJsonObj>(value);
            json_ptr->json.insert(json_ptr->json.begin()+index, value_ptr->json);
        }catch(...){
            throw jtu::FailException {};
        }
    }

    // 7. 删除Object的指定子元素
    void delete_child_from_object(std::shared_ptr<jtu::JsonBase> json_base_ptr, const std::string& key) override{
        // 如果不支持添加内容，请直接抛出 NotSupportException 异常。
        try{
            const auto json_ptr = std::dynamic_pointer_cast<NlohmannJsonObj>(json_base_ptr);
            json_ptr->json.erase(key);
        }catch(...){
            throw jtu::FailException {};
        }
    }

    // 8. 删除Array的指定子元素
    void delete_child_from_array(std::shared_ptr<jtu::JsonBase> json_base_ptr, const std::size_t& index) override{
        // 如果不支持添加内容，请直接抛出 NotSupportException 异常。
        try{
            const auto json_ptr = std::dynamic_pointer_cast<NlohmannJsonObj>(json_base_ptr);
            json_ptr->json.erase(index);
        }catch(...){
            throw jtu::FailException {};
        }
    }

    // 9. 获取Object子元素的方法，请使用拷贝而非移动
    std::shared_ptr<jtu::JsonBase> get_child_from_object(std::shared_ptr<jtu::JsonBase> json_base_ptr, const std::string & str) override{
        // 如果不支持添加内容，请直接抛出 NotSupportException 异常。
        const auto json_ptr = std::make_shared<NlohmannJsonObj>();
        try{
            const auto old_ptr = std::dynamic_pointer_cast<NlohmannJsonObj>(json_base_ptr);
            json_ptr->json = old_ptr->json.at(str);
            return std::static_pointer_cast<jtu::JsonBase>(json_ptr);
        }catch(...){
            throw jtu::FailException {};
        }
        return std::static_pointer_cast<jtu::JsonBase>(json_ptr);
    }

    // 10. 获取Array子元素的方法，请使用拷贝而非移动
    std::shared_ptr<jtu::JsonBase> get_child_from_array(std::shared_ptr<jtu::JsonBase> json_base_ptr, const std::size_t& index) override{
        // 如果不支持添加内容，请直接抛出 NotSupportException 异常。
        const auto json_ptr = std::make_shared<NlohmannJsonObj>();
        try{
            const auto old_ptr = std::dynamic_pointer_cast<NlohmannJsonObj>(json_base_ptr);
            json_ptr->json = old_ptr->json.at(index);
            return std::static_pointer_cast<jtu::JsonBase>(json_ptr);
        }catch(...){
            throw jtu::FailException {};
        }
        return std::static_pointer_cast<jtu::JsonBase>(json_ptr);
    }

    // 11. 值类型，获取浮点型的值
    double get_value_as_double(std::shared_ptr<jtu::JsonBase> json_base_ptr) override{
        // 请务必支持。
        try{
            const auto json_ptr = std::dynamic_pointer_cast<NlohmannJsonObj>(json_base_ptr);
            return json_ptr->json.get<double>();
        }catch(...){
            throw jtu::FailException {};
        }
        return 0;
    }

    // 12. 值类型，获取布尔类型的值
    bool get_value_as_bool(std::shared_ptr<jtu::JsonBase> json_base_ptr) override{
        // 请务必支持。
        try{
            const auto json_ptr = std::dynamic_pointer_cast<NlohmannJsonObj>(json_base_ptr);
            return json_ptr->json.get<bool>();
        }catch(...){
            throw jtu::FailException {};
        }
        return false;
    }

    // 13. 值类型，获取长整数类型的值
    long long get_value_as_int64(std::shared_ptr<jtu::JsonBase> json_base_ptr) override{
        // 请务必支持。
        try{
            const auto json_ptr = std::dynamic_pointer_cast<NlohmannJsonObj>(json_base_ptr);
            return json_ptr->json.get<long long>();
        }catch(...){
            throw jtu::FailException {};
        }
        return 0;
    }

    // 14. 值类型，获取标准字符串类型的值
    std::string get_value_as_string(std::shared_ptr<jtu::JsonBase> json_base_ptr) override{
        // 请务必支持，可以先转const char* 然后转std::string。
        try{
            const auto json_ptr = std::dynamic_pointer_cast<NlohmannJsonObj>(json_base_ptr);
            return json_ptr->json.get<std::string>();
        }catch(...){
            throw jtu::FailException {};
        }
        return "";
    }

    // 15. 值类型，判断内部是否是null值。如果是null请返回null，如果不是请抛出异常或返回false
    bool value_is_null(std::shared_ptr<jtu::JsonBase> json_base_ptr) override{
        try{
            const auto json_ptr = std::dynamic_pointer_cast<NlohmannJsonObj>(json_base_ptr);
            return json_ptr->json.is_null();
        }catch(...){
            throw jtu::FailException {};
        }
        return false;
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////



/* ------------------------------------------- 3 ------------------------------------------- */
///////////////////////////////////////////////////////////////////////////////////////////////
//////// 使用下面这个宏，注册你的测试类对象

// 参数1：你库的名字，随便写   参数2: 你的测试子类名，是第3步中的类名，不能加双引号
M_REGISTER_CLASS("nlohmann-json", NlohmannJsonTest)


///////////////////////////////////////////////////////////////////////////////////////////////
