// 导入模版类头文件
#include "model_class.h"

// 导入你的库
#include "jsonlib.h"

// 写子类，重新三个父类

// 1. 存储JSON实例化对象的容器
class CppJsonlibObj : JsonBase{
public:
    // 存放你库的类型
    Json::JsonBasic jsonBasic;
};

// 2. 纯储字符串对象的容器（有点库有自己的字符串，但是最终通过getString转换成标准字符串）。
class CppJsonStr : public StringBase{
public:
    // 重写方法，将你的库的字符串转换成标准字符串，此步骤不计时间。
    std::string getString() override{ return str; }
    // 存放你库支持的字符串类型
    std::string str;
};

// 3. 继承测试类，重写方法


