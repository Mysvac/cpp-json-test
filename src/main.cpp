#include "model_class.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>
#include <chrono>

TestClassFactory& TestClassFactory::instance(){
    static TestClassFactory instance;
    return instance;
}

class TestScore{
public:
    // 反序列化耗时
    long long unserialize_time_1 { -1 };  // 常规大型文本数据 big_normal.json 259KB
    long long  unserialize_time_2 { -1 };  // 超级多的浮点数内容 big_double.json 2199KB
    long long  unserialize_time_3 { -1 }; // 比较深的list和map嵌套 big_nesting.json 6KB

    // 序列化耗时
    long long  serialize_time_1 { -1 }; // 常规大型文本数据 big_normal.json 259KB
    long long  serialize_time_2 { -1 }; // 超级多的浮点数内容 big_double.json 2199KB
    long long  serialize_time_3 { -1 }; // 比较深的list和map嵌套 big_nesting.json 6KB

    // 序列化+美化
    long long  serialize_pretty_time_1 { -1 }; // 常规大型文本数据 big_normal.json 259KB
    long long  serialize_pretty_time_2 { -1 }; // 超级多的浮点数内容 big_double.json 2199KB
    long long  serialize_pretty_time_3 { -1 }; // 比较深的list和map嵌套 big_nesting.json 6KB
};

std::string readFile(const std::string& path){
    std::stringstream buffer;
    std::ifstream ifs { path };
    // 其他解析逻辑...
    if (!ifs.is_open()) {
        throw std::runtime_error("Could not open file: " + path);
        return "";
    }
    buffer << ifs.rdbuf();
    ifs.close();
    return buffer.str();
}

int main(){

    std::cout << "------------------Prepare to start testing------------------" << std::endl;

    // 获取所有库的测试类对象。 （测试.cpp文件末尾的宏，在单例的std::map中添加了对象，此处获取单例中的std::map对象。）
    std::map<std::string, std::shared_ptr<TestBase>>& dict = TestClassFactory::instance().dict_;

    // 统计分数的map
    std::map<std::string, TestScore> ScoreMap;

    // 存储测试用的json文本数据
    std::string big_normal;
    std::string big_double;
    std::string big_nesting;

    // 开始读取测试需要的JSON文件
    std::cout << "Reading the test JSON files..." << std::endl;
    try{
        big_normal = readFile("json_test_files/big_normal.json");
        big_double = readFile("json_test_files/big_double.json");
        big_nesting = readFile("json_test_files/big_nesting.json");
    }
    catch(...){
        std::cerr << "The JSON file used for testing failed to be read. Please check the working directory.\n" <<std::endl;
        return 0;
    }
    std::cout << "Reading success." << std::endl;
    std::cout << "\n------------------Begin testing------------------\n" << std::endl;


    // /**
    //  * 测试遍历一遍字符串需要的时间 快的不合理，暂时不测了
    //  */
    // std::cout << "[ Begin test strlen() speed. ]" << std::endl;
    // {
    //     // 记录当前测试分数
    //     TestScore testScore;
    //     {
    //         // big_normal.json 259KB
    //         auto begin = std::chrono::system_clock::now();
    //         size_t len = big_normal.size();
    //         size_t it = 0, rd = 1;
    //         while(it < len) ++it, rd = (rd+3)>>1;
    //         auto end = std::chrono::system_clock::now();
    //         auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
    //         std::cout << "strlen() >> (1)-big_normal.json lenth:" << big_double.size() << " time:" << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms"<< std::endl;
    //         testScore.unserialize_time_1 = micros.count();
    //         testScore.serialize_time_1 = micros.count();
    //         testScore.serialize_pretty_time_1 = micros.count();
    //     }
    //     {
    //         // big_double.json 2199KB
    //         auto begin = std::chrono::system_clock::now();
    //         size_t len = big_double.size();
    //         size_t it = 0, rd = 1;
    //         while(it < len) ++it, rd = (rd+3)>>1;
    //         auto end = std::chrono::system_clock::now();
    //         auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
    //         std::cout << "strlen() >> (2)-big_doubles.json lenth:" << big_double.size() << " time:" << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms"<< std::endl;
    //         testScore.unserialize_time_2 = micros.count();
    //         testScore.serialize_time_2 = micros.count();
    //         testScore.serialize_pretty_time_2 = micros.count();
    //     }
    //     {
    //         // big_nesting.json 6KB
    //         auto begin = std::chrono::system_clock::now();
    //         size_t len = big_nesting.size();
    //         size_t it = 0, rd = 1;
    //         while(it < len) ++it, rd = (rd+3)>>1;
    //         auto end = std::chrono::system_clock::now();
    //         auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
    //         std::cout << "strlen() >> (3)-big_nesting.json lenth:" << big_double.size() << " time:" << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms"<< std::endl;
    //         testScore.unserialize_time_3 = micros.count();
    //         testScore.serialize_time_3 = micros.count();
    //         testScore.serialize_pretty_time_3 = micros.count();
    //     }
    //     ScoreMap["function-strlen()"] = testScore;
    // }
    // std::cout << "[ End of testing strlen() speed. ]" << std::endl;
    // std::cout << "\n---------------------------------------------------------\n" << std::endl;


    // 开始遍历全部库的测试类，跑内部的测试函数
    for(auto& it : dict){
        // 获取库名
        std::string libName = it.first;
        // 获取库注册的测试类对象
        std::shared_ptr<TestBase>& testClass = it.second;
        // 统计当前库的测试分数
        TestScore testScore;
        std::cout << "[ Begin of test lib: " << libName << " ]" << std::endl;

        // 反序列化与序列化测速
        {
            // 三个指针，分别存储上面读取的3个文件的反序列化结果
            std::shared_ptr<JsonBase> json_ptr_1 = nullptr;
            std::shared_ptr<JsonBase> json_ptr_2 = nullptr;
            std::shared_ptr<JsonBase> json_ptr_3 = nullptr;

            // big_normal.json 反序列化测试 较大的文本内容为主的文件
            try{
                auto begin = std::chrono::system_clock::now();

                json_ptr_1 =  testClass->unserialize(big_normal);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                testScore.unserialize_time_1 = micros.count();
                std::cout << libName << " >> unserialize (1) success: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms"<< std::endl;
            }
            catch(NotSupportException){
                testScore.unserialize_time_1 = -1;
                std::cerr << libName << " >> not support unserialize (1)." << std::endl;
            }
            catch(...){
                testScore.unserialize_time_1 = -2;
                std::cerr << libName << " >> failed in unserialize (1)." << std::endl;
            }

            // big_doubles.json 反序列化测试 包含大量数值类型的文件
            try{
                auto begin = std::chrono::system_clock::now();

                json_ptr_2 =  testClass->unserialize(big_double);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                testScore.unserialize_time_2 = micros.count();
                std::cout << libName << " >> unserialize (2) success: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms"<< std::endl;
            }
            catch(NotSupportException){
                testScore.unserialize_time_2 = -1;
                std::cerr << libName << " >> not support unserialize (2)." << std::endl;
            }
            catch(...){
                testScore.unserialize_time_2 = -2;
                std::cerr << libName << " >> failed in unserialize (2)." << std::endl;
            }

            // big_nesting 反序列化测试 含有较深层次的文件，内容较少
            try{
                auto begin = std::chrono::system_clock::now();

                json_ptr_3 =  testClass->unserialize(big_nesting);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                testScore.unserialize_time_3 = micros.count();
                std::cout << libName << " >> unserialize (3) success: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms"<< std::endl;
            }
            catch(NotSupportException){
                testScore.unserialize_time_3 = -1;
                std::cerr << libName << " >> not support unserialize (3)." << std::endl;
            }
            catch(...){
                testScore.unserialize_time_3 = -2;
                std::cerr << libName << " >> failed in unserialize (3)." << std::endl;
            }

            // big_normal.json 序列化测试 较大的文本内容为主的文件
            try{
                auto begin = std::chrono::system_clock::now();

                auto tmp =  testClass->serialize(json_ptr_1);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                std::string s = tmp->getString();
                s[1] = '1'; // 测试内容是否正常
                testScore.serialize_time_1 = micros.count();
                std::cout << libName << " >> serialize (1) success: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms"<< std::endl;
            }
            catch(NotSupportException){
                testScore.serialize_time_1 = -1;
                std::cerr << libName << " >> not support serialize (1)." << std::endl;
            }
            catch(...){
                testScore.serialize_time_1 = -2;
                std::cerr << libName << " >> failed in serialize (1)." << std::endl;
            }

            // big_doubles.json 序列化测试 包含大量数值类型的文件
            try{
                auto begin = std::chrono::system_clock::now();

                auto tmp =  testClass->serialize(json_ptr_2);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                std::string s = tmp->getString();
                s[1] = '1'; // 测试内容是否正常
                testScore.serialize_time_2 = micros.count();
                std::cout << libName << " >> serialize (2) success: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms"<< std::endl;
            }
            catch(NotSupportException){
                testScore.serialize_time_2 = -1;
                std::cerr << libName << " >> not support serialize (2)." << std::endl;
            }
            catch(...){
                testScore.serialize_time_2 = -2;
                std::cerr << libName << " >> failed in serialize (2)." << std::endl;
            }

            // big_nesting 序列化测试 含有较深层次的文件，内容较少
            try{
                auto begin = std::chrono::system_clock::now();

                auto tmp =  testClass->serialize(json_ptr_3);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                std::string s = tmp->getString();
                s[1] = '1'; // 测试内容是否正常
                testScore.serialize_time_3 = micros.count();
                std::cout << libName << " >> serialize (3) success: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms"<< std::endl;
            }
            catch(NotSupportException){
                testScore.serialize_time_3 = -1;
                std::cerr << libName << " >> not support serialize (3)." << std::endl;
            }
            catch(...){
                testScore.serialize_time_3 = -2;
                std::cerr << libName << " >> failed in serialize (3)." << std::endl;
            }

            // big_normal.json 序列化+美化测试 较大的文本内容为主的文件
            try{
                auto begin = std::chrono::system_clock::now();

                auto tmp =  testClass->serialize_pretty(json_ptr_1);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                testScore.serialize_pretty_time_1 = micros.count();
                std::cout << libName << " >> serialize_pretty (1) success: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms"<< std::endl;
            }
            catch(NotSupportException){
                testScore.serialize_pretty_time_1 = -1;
                std::cerr << libName << " >> not support serialize_pretty (1)." << std::endl;
            }
            catch(...){
                testScore.serialize_pretty_time_1 = -2;
                std::cerr << libName << " >> failed in serialize_pretty (1)." << std::endl;
            }

            // big_doubles.json 序列化+美化测试 包含大量数值类型的文件
            try{
                auto begin = std::chrono::system_clock::now();

                auto tmp =  testClass->serialize_pretty(json_ptr_2);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                testScore.serialize_pretty_time_2 = micros.count();
                std::cout << libName << " >> serialize_pretty (2) success: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms"<< std::endl;
            }
            catch(NotSupportException){
                testScore.serialize_pretty_time_2 = -1;
                std::cerr << libName << " >> not support serialize_pretty (2)." << std::endl;
            }
            catch(...){
                testScore.serialize_pretty_time_2 = -2;
                std::cerr << libName << " >> failed in serialize_pretty (2)." << std::endl;
            }

            // big_nesting 序列化+美化测试 含有较深层次的文件，内容较少
            try{
                auto begin = std::chrono::system_clock::now();

                auto tmp =  testClass->serialize_pretty(json_ptr_3);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                testScore.serialize_pretty_time_3 = micros.count();
                std::cout << libName << " >> serialize_pretty (3) success: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms"<< std::endl;
            }
            catch(NotSupportException){
                testScore.serialize_pretty_time_3 = -1;
                std::cerr << libName << " >> not support serialize_pretty (3)." << std::endl;
            }
            catch(...){
                testScore.serialize_pretty_time_3 = -2;
                std::cerr << libName << " >> failed in serialize_pretty (3)." << std::endl;
            }
        }

    
        ScoreMap[libName] = testScore;
        std::cout << "[ End of test lib: " << libName << " ]" << std::endl;
        std::cout << "\n---------------------------------------------------------\n" << std::endl;
    }

}
