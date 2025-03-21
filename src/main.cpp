// 测试类模版头文件
#include "model_class.h"
// 主程序专用 函数/类 头文件
#include "main.h"

// 测试类的工厂的单例函数的定义
TestClassFactory& TestClassFactory::instance(){
    static TestClassFactory instance;
    return instance;
}


// 主函数
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
    std::string big_varies;
    std::string big_test;
    std::string big_object;
    std::string big_array;
    JsonCounter counter_big_varies;

    // 开始读取测试需要的JSON文件和初始化计数器
    {
        std::cout << "Reading the test JSON files..." << std::endl;
        try{
            big_normal = readFile( PROJECT_JSONFILE_PATH "/big_normal.json");
            big_double = readFile( PROJECT_JSONFILE_PATH "/big_double.json");
            big_nesting = readFile( PROJECT_JSONFILE_PATH "/big_nesting.json");
            big_varies  = readFile( PROJECT_JSONFILE_PATH "/big_varies.json");
            big_test  = readFile( PROJECT_JSONFILE_PATH "/big_test.json");
            big_object = readFile( PROJECT_JSONFILE_PATH "/big_object.json");
            big_array = readFile( PROJECT_JSONFILE_PATH "/big_array.json");
        }
        catch(...){
            std::cerr << "The JSON file used for testing failed to be read. Please check the working directory.\n" <<std::endl;
            return 0;
        }
        std::cout << "Reading success." << std::endl;
            // 开始准备测试数据
        {
            counter_big_varies.objectNum = 10937;
            counter_big_varies.objectChildNum = 25871;
            counter_big_varies.arrayNum = 10451;
            counter_big_varies.arrayChildNum = 11908;
            counter_big_varies.stringNum = 735;
            counter_big_varies.numberNum = 14392;
            counter_big_varies.boolenNum = 2;
            counter_big_varies.nullNum = 1263;
        }
    }


    std::cout << "\n------------------Begin testing------------------\n" << std::endl;
    // 开始遍历全部库的测试类，跑内部的测试函数
    for(auto& it : dict){
        // 获取库名
        std::string libName = it.first;
        // 获取库注册的测试类对象
        std::shared_ptr<TestBase>& testClass = it.second;
        // 统计当前库的测试分数
        TestScore testScore;
        // 当前库的错误报告
        std::ofstream ofs{ std::string{ PROJECT_RESULT_PATH } + "/" + libName + "_error_report.txt" };
        ofs << "[ test lib: " << libName << " ]\n" << std::endl;

        std::cout << "[ Begin of test lib: " << libName << " ]" << std::endl;


        // 基础测试 1.解析的数据是否正确 2.是否支持解析值类型 
        {
            JsonCounter jsonCounter;

            // 1. validity 解析正确性测试
            try{
                std::cout << libName << " >> integrity testing......"  << std::flush;
                std::shared_ptr<JsonBase> json_ptr =  testClass->deserialize(big_varies);

                jsonCounter = testClass->count_element(json_ptr);

                if(counter_big_varies == jsonCounter){
                    testScore.validity = 1;
                    std::cout << '\r' << libName << " >> integrity test passed." << std::string(20, ' ') << std::endl;
                }
                else throw std::exception {};
            }
            catch(const std::exception& e){
                testScore.validity = -1;

                ofs << " 1. integrity test failed:" << std::endl;
                ofs << "deserialize file: 'json_test_files/big_varies.json'" << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs<< "--------------Actual Quantity--------------" << std::endl;
                ofs << "objectNum: " << counter_big_varies.objectNum << std::endl;
                ofs << "objectChildNum: " << counter_big_varies.objectChildNum << std::endl;
                ofs << "arrayNum: " << counter_big_varies.arrayNum << std::endl;
                ofs << "arrayChildNum: " << counter_big_varies.arrayChildNum << std::endl;
                ofs << "stringNum: " << counter_big_varies.stringNum << std::endl;
                ofs << "numberNum: " << counter_big_varies.numberNum << std::endl;
                ofs << "boolenNum: " << counter_big_varies.boolenNum << std::endl;
                ofs << "nullNum: " << counter_big_varies.nullNum << std::endl;
                ofs<< "--------------Your Quantity--------------" << std::endl;
                ofs << "objectNum: " << jsonCounter.objectNum << std::endl;
                ofs << "objectChildNum: " << jsonCounter.objectChildNum << std::endl;
                ofs << "arrayNum: " << jsonCounter.arrayNum << std::endl;
                ofs << "arrayChildNum: " << jsonCounter.arrayChildNum << std::endl;
                ofs << "stringNum: " << jsonCounter.stringNum << std::endl;
                ofs << "numberNum: " << jsonCounter.numberNum << std::endl;
                ofs << "boolenNum: " << jsonCounter.boolenNum << std::endl;
                ofs << "nullNum: " << jsonCounter.nullNum << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;


                // std::cerr << e.what() << std::endl;
                std::cerr << '\r' << libName << " >> Integrity test faile. Test terminated, please refer to the 'result/" << libName << "_error_report.txt'."<< std::endl;
                std::cerr << "[ End of test lib: " << libName << " ]" << std::endl;
                std::cerr << "\n---------------------------------------------------------\n" << std::endl;
                continue;
            }
        

            // 2. value_type 值类型解析支持测试
            try{
                std::cout << libName << " >> JSON value type testing......" << std::flush;
                ofs << " 2. value type deserialize test......" << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;

                if(12345678ll != testClass->get_value_as_int64(testClass->deserialize("12345678"))){ 
                    long long it = testClass->get_value_as_int64(testClass->deserialize("12345678"));
                    ofs << '\n' << "String \"12345678\" parsing failed, the return result is not equal to 12345678ll." << std::endl;
                    throw FailException{ R"_(12345678ll != testClass->get_value_as_int64(testClass->deserialize("12345678")))_" }; 
                }
                if(std::string{"string-text-test"} != testClass->get_value_as_string(testClass->deserialize("\"string-text-test\""))){ 
                    ofs << '\n' << "String \"\"string-text-test\"\" parsing failed, the return result is not equal to \"string-text-test\"." << std::endl;
                    throw FailException{R"_(std::string{"string-text-test"} != testClass->get_value_as_string(testClass->deserialize("\"string-text-test\"")))_"}; 
                }
                if( !(testClass->get_value_as_bool(testClass->deserialize("true")))){ 
                    ofs << '\n' << "String \"true\" parsing failed, the return result is not equal to true." << std::endl;
                    throw FailException{R"_(!(testClass->get_value_as_bool(testClass->deserialize("true"))))_"}; 
                }
                if( testClass->get_value_as_bool(testClass->deserialize("false"))){ 
                    ofs << '\n' << "String \"false\" parsing failed, the return result is not equal to false." << std::endl;
                    throw FailException{R"_(testClass->get_value_as_bool(testClass->deserialize("false")))_"}; 
                }
                if( !testClass->value_is_null(testClass->deserialize("null"))){ 
                    ofs << '\n' << "String \"null\" parsing failed, the return result is not equal to null." << std::endl;
                    throw FailException{R"_(!testClass->value_is_null(testClass->deserialize("null")))_"}; 
                }
                testScore.value_type = 1;
                std::cout << '\r' << libName << " >> JSON value type passed." << std::string(20, ' ')<< std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
            }
            catch(const NotSupportException& e){
                testScore.value_type = -1;
                ofs << " 2. value type deserialize test not support!!!" << std::endl;
                ofs << "Test samples:" << std::endl;
                ofs << R"_( testClass->get_value_as_int64(testClass->deserialize("123456789012345")) )_" << std::endl;
                ofs << R"_( testClass->get_value_as_string(testClass->deserialize("\"string-text-test\"")) )_" << std::endl;
                ofs << R"_( testClass->get_value_as_bool(testClass->deserialize("true")) )_" << std::endl;
                ofs << R"_( testClass->get_value_as_bool(testClass->deserialize("false")) )_" << std::endl;
                ofs << R"_( testClass->value_is_null(testClass->deserialize("null")) )_" << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;

                std::cerr << '\r' << libName << " >> JSON value type not support." << std::string(20, ' ') << std::endl;
            }
            catch(const std::exception& e){
                testScore.value_type = -2;

                ofs << " 2. value type deserialize test faild!!!" << std::endl;
                ofs << "Exception result:" << e.what() << std::endl;
                ofs << "Test samples:" << std::endl;
                ofs << R"_( testClass->get_value_as_int64(testClass->deserialize("123456789012345")) )_" << std::endl;
                ofs << R"_( testClass->get_value_as_string(testClass->deserialize("\"string-text-test\"")) )_" << std::endl;
                ofs << R"_( testClass->get_value_as_bool(testClass->deserialize("true")) )_" << std::endl;
                ofs << R"_( testClass->get_value_as_bool(testClass->deserialize("false")) )_" << std::endl;
                ofs << R"_( testClass->value_is_null(testClass->deserialize("null")) )_" << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;

                std::cerr << '\r' << libName << " >> JSON value type failed." << std::string(20, ' ') << std::endl;
            }
        }

        // 3-11. unserialize and serialize speed test 反序列化与序列化测速
        {
            // 三个指针，分别存储上面读取的3个文件的反序列化结果
            std::shared_ptr<JsonBase> json_ptr_1 = nullptr;
            std::shared_ptr<JsonBase> json_ptr_2 = nullptr;
            std::shared_ptr<JsonBase> json_ptr_3 = nullptr;

            // 3. big_normal.json 反序列化测试 较大的文本内容为主的文件
            try{
                std::cout << libName << " >> unserialize (1) testing......"  << std::flush;
                auto begin = std::chrono::system_clock::now();

                json_ptr_1 =  testClass->deserialize(big_normal);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                testScore.unserialize_1 = micros.count();
                std::cout << '\r' << libName << " >> unserialize (1) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.unserialize_1 = -1;
                ofs << " 3. unserialize (1) test not support." << std::endl;
                ofs << " file: json_test_files/big_normal.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> unserialize (1) not support." << std::string(20, ' ') << std::endl;
            }
            catch(...){
                testScore.unserialize_1 = -2;
                ofs << " 3. unserialize (1) test failed." << std::endl;
                ofs << " file: json_test_files/big_normal.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> unserialize (1) failed." << std::string(20, ' ') << std::endl;
            }

            // 4. big_doubles.json 反序列化测试 包含大量数值类型的文件
            try{
                std::cout << libName << " >> unserialize (2) testing......"  << std::flush;
                auto begin = std::chrono::system_clock::now();

                json_ptr_2 =  testClass->deserialize(big_double);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                testScore.unserialize_2 = micros.count();
                std::cout << '\r' << libName << " >> unserialize (2) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.unserialize_2 = -1;
                ofs << " 4. unserialize (2) test not support." << std::endl;
                ofs << " file: json_test_files/big_double.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> unserialize (2) not support." << std::string(20, ' ') << std::endl;
            }
            catch(...){
                testScore.unserialize_2 = -2;
                ofs << " 4. unserialize (2) test failed." << std::endl;
                ofs << " file: json_test_files/big_double.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> unserialize (2) failed." << std::string(20, ' ') << std::endl;
            }

            // 5. big_nesting 反序列化测试 含有较深层次的文件，内容较少
            try{
                std::cout << libName << " >> unserialize (3) testing......"  << std::flush;
                auto begin = std::chrono::system_clock::now();

                json_ptr_3 =  testClass->deserialize(big_nesting);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                testScore.unserialize_3 = micros.count();
                std::cout << '\r' << libName << " >> unserialize (3) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(1, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.unserialize_3 = -1;
                ofs << " 5. unserialize (3) test not support." << std::endl;
                ofs << " file: json_test_files/big_nesting.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> unserialize (3) not support." << std::string(20, ' ') << std::endl;
            }
            catch(...){
                testScore.unserialize_3 = -2;
                ofs << " 5. unserialize (3) test not support." << std::endl;
                ofs << " file: json_test_files/big_nesting.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> unserialize (3) failed." << std::string(20, ' ') << std::endl;
            }

            // 6. big_normal.json 序列化测试 较大的文本内容为主的文件
            try{
                std::cout << libName << " >> serialize (1) testing......"  << std::flush;
                auto begin = std::chrono::system_clock::now();

                auto tmp =  testClass->serialize(json_ptr_1);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                std::string s = tmp->getString();
                s[1] = '1'; // 测试内容是否正常
                testScore.serialize_1 = micros.count();
                std::cout << '\r' << libName << " >> serialize (1) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.serialize_1 = -1;
                ofs << " 6. serialize (1) test not support." << std::endl;
                ofs << " file: json_test_files/big_normal.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> serialize (1) not support." << std::string(20, ' ') << std::endl;
            }
            catch(...){
                testScore.serialize_1 = -2;
                ofs << " 6. serialize (1) test failed." << std::endl;
                ofs << " file: json_test_files/big_normal.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> serialize (1) failed."  << std::string(20, ' ') << std::endl;
            }

            // 7. big_doubles.json 序列化测试 包含大量数值类型的文件
            try{
                std::cout << libName << " >> serialize (2) testing......"  << std::flush;
                auto begin = std::chrono::system_clock::now();

                auto tmp =  testClass->serialize(json_ptr_2);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                std::string s = tmp->getString();
                s[1] = '1'; // 测试内容是否正常
                testScore.serialize_2 = micros.count();
                std::cout << '\r' << libName << " >> serialize (2) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.serialize_2 = -1;
                ofs << " 7. serialize (2) test not support." << std::endl;
                ofs << " file: json_test_files/big_double.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> serialize (2) not support." << std::string(20, ' ') << std::endl;
            }
            catch(...){
                testScore.serialize_2 = -2;
                ofs << " 7. serialize (2) test failed." << std::endl;
                ofs << " file: json_test_files/big_double.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> serialize (2) failed."  << std::string(20, ' ')<< std::endl;
            }

            // 8. big_nesting 序列化测试 含有较深层次的文件，内容较少
            try{
                std::cout << libName << " >> serialize (3) testing......"  << std::flush;
                auto begin = std::chrono::system_clock::now();

                auto tmp =  testClass->serialize(json_ptr_3);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                std::string s = tmp->getString();
                s[1] = '1'; // 测试内容是否正常
                testScore.serialize_3 = micros.count();
                std::cout << '\r' << libName << " >> serialize (3) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.serialize_3 = -1;
                ofs << " 8. serialize (3) test not support." << std::endl;
                ofs << " file: json_test_files/big_nesting.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> serialize (3) not support." << std::string(20, ' ') << std::endl;
            }
            catch(...){
                testScore.serialize_3 = -2;
                ofs << " 8. serialize (3) test failed." << std::endl;
                ofs << " file: json_test_files/big_nesting.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> serialize (3) failed." << std::string(20, ' ') << std::endl;
            }

            // 9. big_normal.json 序列化+美化测试 较大的文本内容为主的文件
            try{
                std::cout << libName << " >> serialize_pretty (1) testing......"  << std::flush;
                auto begin = std::chrono::system_clock::now();

                auto tmp =  testClass->serialize_pretty(json_ptr_1);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                testScore.prettify_1 = micros.count();
                std::cout << '\r' << libName << " >> serialize_pretty (1) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.prettify_1 = -1;
                ofs << " 9. serialize_pretty (1) test not support." << std::endl;
                ofs << " file: json_test_files/big_normal.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> serialize_pretty (1) not support." << std::string(20, ' ') << std::endl;
            }
            catch(...){
                testScore.prettify_1 = -2;
                ofs << " 9. serialize_pretty (1) test failed." << std::endl;
                ofs << " file: json_test_files/big_normal.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> serialize_pretty (1) failed."  << std::string(20, ' ') << std::endl;
            }

            // big_doubles.json 序列化+美化测试 包含大量数值类型的文件
            try{
                std::cout << libName << " >> serialize_pretty (2) testing......"  << std::flush;
                auto begin = std::chrono::system_clock::now();

                auto tmp =  testClass->serialize_pretty(json_ptr_2);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                testScore.prettify_2 = micros.count();
                std::cout << '\r' << libName << " >> serialize_pretty (2) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.prettify_2 = -1;
                ofs << " 10. serialize_pretty (2) test not support." << std::endl;
                ofs << " file: json_test_files/big_double.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> serialize_pretty (2) not support." << std::string(20, ' ')<< std::endl;
            }
            catch(...){
                testScore.prettify_2 = -2;
                ofs << " 10. serialize_pretty (2) test failed." << std::endl;
                ofs << " file: json_test_files/big_double.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> serialize_pretty (2) failed." << std::string(20, ' ') << std::endl;
            }

            // big_nesting 序列化+美化测试 含有较深层次的文件，内容较少
            try{
                std::cout << libName << " >> serialize_pretty (3) testing......"  << std::flush;
                auto begin = std::chrono::system_clock::now();

                auto tmp =  testClass->serialize_pretty(json_ptr_3);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                testScore.prettify_3 = micros.count();
                std::cout << '\r' << libName << " >> serialize_pretty (3) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.prettify_3 = -1;
                ofs << " 11. serialize_pretty (3) test not support." << std::endl;
                ofs << " file: json_test_files/big_nesting.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> serialize_pretty (3) not support." << std::string(20, ' ') << std::endl;
            }
            catch(...){
                testScore.prettify_3 = -2;
                ofs << " 11. serialize_pretty (3) test failed." << std::endl;
                ofs << " file: json_test_files/big_nesting.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> serialize_pretty (3) failed." << std::string(20, ' ') << std::endl;
            }
        
        }


        // 12-14 get 获取  add 增加  delete 删除  speed test 速度测试
        {
            // 12. get_child speed test 测试内部访问速度 包含对象复制耗时
            try{
                std::cout << libName << " >> get child speed testing......"  << std::flush;
                std::shared_ptr<JsonBase> json_ptr = testClass->deserialize(big_test);
                auto begin = std::chrono::system_clock::now();

                long long tmp_int64 = 0;
                for(int i = 1;i<=99;++i){
                    long long tmp_int64 = testClass->get_value_as_int64(testClass->get_child_from_object(json_ptr, std::to_string(i)));
                }

                std::string tmp_str;
                for(int i=100;i<=199;++i){
                    tmp_str = testClass->get_value_as_string(testClass->get_child_from_object(json_ptr, std::to_string(i)));
                }
                bool tmp_bool = false;
                for(int i=300;i<=399;++i){
                    tmp_bool = testClass->get_value_as_bool(testClass->get_child_from_object(json_ptr, std::to_string(i)));
                }
                double tmp_double;
                for(int i = 5; i<=95;++i){
                    tmp_double = testClass->get_value_as_double(
                        testClass->get_child_from_array(
                            testClass->get_child_from_object(json_ptr, "list"),
                            i
                        )
                    );
                }

                for(int i = 1;i<=99;++i){
                    tmp_int64 = testClass->get_value_as_int64(
                        testClass->get_child_from_object(
                            testClass->get_child_from_array(
                                testClass->get_child_from_object(json_ptr, "list"),
                                1
                            ),
                            std::to_string(i)
                        )
                    );
                }

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                testScore.get_child = micros.count();
                std::cout << '\r' << libName << " >> get child speed testing passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms"  << std::string(10, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.get_child = -1;
                ofs << e.what() << std::endl;
                ofs << " 12. get child test not support." << std::endl;
                ofs << " file: json_test_files/big_test.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> get child test not support." << std::string(20, ' ') << std::endl;
            }
            catch(const std::exception& e){
                testScore.get_child = -2; 
                ofs << e.what() << std::endl;
                ofs << " 12. get child test failed." << std::endl;
                ofs << " file: json_test_files/big_test.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> get child test failed." << std::string(20, ' ')<< std::endl;
                continue;
            }

            // 13. add_child speed test 增加子元素测速
            try{
                std::cout << libName << " >> add child speed testing......" << std::flush;
                std::shared_ptr<JsonBase> json_map = testClass->deserialize(big_test);
                std::shared_ptr<JsonBase> json_list = testClass->deserialize( "[ true , false ]" );
                std::shared_ptr<JsonBase> json_list_tmp = testClass->deserialize( "[ null , 1 ]" );
                auto begin = std::chrono::system_clock::now();


                for(int i = 1;i<=500;++i){
                    testClass->add_child_to_array(json_list, i , testClass->deserialize( "[ " + std::to_string(i) + " ]" ) );
                }

                testClass->add_child_to_array(json_list_tmp, 1 ,json_map);
                testClass->add_child_to_object(json_map, "self_map1" ,json_list_tmp);
                testClass->add_child_to_object(json_map, "self_map2" ,json_list_tmp);

                for(int i = 1000; i<=1299 ;++i){
                    testClass->add_child_to_object(json_map, std::to_string(i), json_list);
                }
                testClass->add_child_to_object(json_map, "self_map3" ,json_list_tmp);



                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                testScore.add_child = micros.count();
                std::cout << '\r' << libName << " >> add child speed testing passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.add_child = -1;
                ofs << " 13. add child test not support." << std::endl;
                ofs << " file: json_test_files/big_test.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> add child test not support." << std::string(20, ' ') << std::endl;
            }
            catch(...){
                testScore.add_child = -2;
                ofs << " 13. add child test failed." << std::endl;
                ofs << " file: json_test_files/big_test.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> add child test failed." << std::string(20, ' ')<< std::endl;
                continue;
            }

            // 14. delete_child speed test 删除子元素测速
            try{
                std::cout << libName << " >> delete child speed testing......"  << std::flush;
                std::shared_ptr<JsonBase> json_ptr =  testClass->deserialize(big_varies);
                std::shared_ptr<JsonBase> json_map = testClass->deserialize(big_object);
                std::shared_ptr<JsonBase> json_list = testClass->deserialize(big_array);
                auto begin = std::chrono::system_clock::now();

                testClass->delete_child_from_object(json_ptr, "events");
                testClass->delete_child_from_object(json_ptr, "blockNames");

                for(int i=1;i<=1999;++i){
                    testClass->delete_child_from_object(json_map, std::to_string(i));
                }
                for(int i=3999;i>=2001;--i){
                    testClass->delete_child_from_object(json_map, std::to_string(i));
                }
                for(int i=3990;i>=1900;--i){
                    testClass->delete_child_from_array(json_list, i);
                }
                for(int i=500;i<=800;++i){
                    testClass->delete_child_from_array(json_list, i);
                }

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                testScore.delete_child = micros.count();
                std::cout << '\r' << libName << " >> delete child speed testing passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.delete_child = -1;
                ofs << " 14. delete child test not support." << std::endl;
                ofs << " file: json_test_files/big_test.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> delete child test not support." << std::string(20, ' ') << std::endl;
            }
            catch(...){
                testScore.delete_child = -2;
                ofs << " 14. delete child test failed." << std::endl;
                ofs << " file: json_test_files/big_test.json " << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << libName << " >> delete child test failed." << std::string(20, ' ')<< std::endl;
                continue;
            }

        }



        // 15-18 解析准确性 错误查找 字符串转义测试
        {

            // 15. double_validity 浮点数格式解析支持
            {
                std::cout << libName << " >> double validity testing......"  << std::flush;
                ofs << " 15. double_validity test." << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                testScore.double_validity = 0;
                try{ testClass->deserialize( "[ 0.14351516 ]" ); testScore.double_validity += 10; }catch(...){
                    ofs << "\n" <<  "[ 0.14351516 ] is valid but deserialize failed." << std::endl;
                }
                try{ testClass->deserialize( "[ 512561.16551 ]" ); testScore.double_validity += 10; }catch(...){
                    ofs << "\n" <<  "[ 512561.16551 ] is valid but deserialize failed." << std::endl;
                }
                try{ testClass->deserialize( "[ 51161.0 ]" ); testScore.double_validity += 5; }catch(...){
                    ofs << "\n" <<  "[ 51161.0 ] is valid but deserialize failed." << std::endl;
                }
                try{ testClass->deserialize( "[ 51161. ]" ); testScore.double_validity += 5; }catch(...){
                    ofs << "\n" <<  "[ 51161. ] is valid but deserialize failed." << std::endl;
                }
                try{ testClass->deserialize( "[ 51161e6 ]" ); testScore.double_validity += 5; }catch(...){
                    ofs << "\n" <<  "[ 51161e6 ] is valid but deserialize failed." << std::endl;
                }
                try{ testClass->deserialize( "[ 51.161E- ]" ); testScore.double_validity += 5; }catch(...){
                    ofs << "\n" <<  "[ 51.161E-  ] is valid but deserialize failed." << std::endl;
                }
                try{ testClass->deserialize( "[ 51161.e+106 ]" ); testScore.double_validity += 5; }catch(...){
                    ofs << "\n" <<  "[ 51161.e+106  ] is valid but deserialize failed." << std::endl;
                }
                try{ testClass->deserialize( "[ -51161.4e-6 ]" ); testScore.double_validity += 5; }catch(...){
                    ofs << "\n" <<  "[ -51161.4e-6  ] is valid but deserialize failed." << std::endl;
                }
                try{ testClass->deserialize( "[ 0.1E+6 ]" ); testScore.double_validity += 5; }catch(...){
                    ofs << "\n" <<  "[ 0.1E+6  ] is valid but deserialize failed." << std::endl;
                }
                try{ testClass->deserialize( "[ 0 ]" ); testScore.double_validity += 5; }catch(...){
                    ofs << "\n" <<  "[ 0 ] is valid but deserialize failed." << std::endl;
                }
                try{ testClass->deserialize( "[ 0. ]" ); testScore.double_validity += 4; }catch(...){
                    ofs << "\n" <<  "[ 0. ] is valid but deserialize failed." << std::endl;
                }
                try{ 
                    testClass->deserialize( "[ +5131.0 ]" ); 
                    ofs << "\n" <<  "[ +5131.0 ] is invalid but deserialize passed." << std::endl;
                }
                catch(...){ testScore.double_validity += 4; }
                try{ 
                    testClass->deserialize( "[ 01 ]" );  
                    ofs << "\n" <<  "[ 01 ] is invalid but deserialize passed." << std::endl;
                }
                catch(...){ testScore.double_validity += 4; }
                try{ 
                    testClass->deserialize( "[ 0E8 ]" );  
                    ofs << "\n" <<  "[ 0E8 ] is invalid but deserialize passed." << std::endl;
                }catch(...){ testScore.double_validity += 4;}
                try{ 
                    testClass->deserialize( "[ .51516 ]" ); 
                    ofs << "\n" <<  "[ .51516 ] is invalid but deserialize passed." << std::endl;
                }
                catch(...){ testScore.double_validity += 4; }
                try{ 
                    testClass->deserialize( "[ 51.516. ]" ); 
                    ofs << "\n" <<  "[ 51.516. ] is invalid but deserialize passed." << std::endl;
                }catch(...){ testScore.double_validity += 4; }
                try{ 
                    testClass->deserialize( "[ e51 ]" ); 
                    ofs << "\n" <<  "[ e51 ] is invalid but deserialize passed." << std::endl;
                }catch(...){ testScore.double_validity += 4; }
                try{ 
                    testClass->deserialize( "[ 515E+0.5 ]" ); 
                    ofs << "\n" <<  "[ 515E+0.5 ] is invalid but deserialize passed." << std::endl;
                }catch(...){ testScore.double_validity += 4; }

                try{ 
                    testClass->deserialize( "[ 5153616p16.15461616 ]"); 
                    ofs << "\n" <<  "[ 515E+0.5 ] is invalid but deserialize passed." << std::endl;
                }catch(...){ testScore.double_validity += 4; }
                try{ 
                    testClass->deserialize( "[ 1241351616161e. ]"); 
                    ofs << "\n" <<  "[ 1241351616161e. ] is invalid but deserialize passed." << std::endl;
                }catch(...){ testScore.double_validity += 4; }

                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cout << '\r' << libName << " >> double validity score: " << testScore.double_validity << "/100" << std::string(10, ' ') << std::endl;
            }

            // 16. double accuracy test 浮点数精度测试
            {
                std::cout << libName << " >> double accuracy testing......"  << std::flush;
                ofs << " 16. double_accuracy test." << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                try{
                    testScore.double_accuracy = 0;
                    if(isEqual(std::stod("1231415152.0") , testClass->get_value_as_double(testClass->deserialize( "1231415152.0" )))){ testScore.double_accuracy +=10; }
                    else{ ofs << '\n' << "The error in analyzing 1231415152.0 is greater than 3e-16." << std::endl; }

                    if(isEqual(std::stod("51161.0e+6") , testClass->get_value_as_double(testClass->deserialize( "51161.0e+6" )))){testScore.double_accuracy +=10; }
                    else{ ofs << '\n' << "The error in analyzing 51161.0e+6 is greater than 3e-16." << std::endl; }

                    if(isEqual(std::stod("512561.16551") , testClass->get_value_as_double(testClass->deserialize( "512561.16551" )))){testScore.double_accuracy +=10; }
                    else{ ofs << '\n' << "The error in analyzing 512561.16551is greater than 3e-16." << std::endl; }

                    if(isEqual(std::stod("-61.973609999999951") , testClass->get_value_as_double(testClass->deserialize( "-61.973609999999951" )))){testScore.double_accuracy +=10; }
                    else{ ofs << '\n' << "The error in analyzing -61.973609999999951 is greater than 3e-16." << std::endl; }

                    if(isEqual(std::stod("-126.64472999999992") , testClass->get_value_as_double(testClass->deserialize( "-126.64472999999992" )))){testScore.double_accuracy +=10; }
                    else{ ofs << '\n' << "The error in analyzing -126.64472999999992 is greater than 3e-16." << std::endl; }

                    if(isEqual(std::stod("78.2322080000014e-156") , testClass->get_value_as_double(testClass->deserialize( "78.2322080000014e-156" )))){testScore.double_accuracy +=10; }
                    else{ ofs << '\n' << "The error in analyzing 78.2322080000014e-156 is greater than 3e-16." << std::endl; }

                    if(isEqual(std::stod("80.402206000000035") , testClass->get_value_as_double(testClass->deserialize( "80.402206000000035" )))){testScore.double_accuracy +=10; }
                    else{ ofs << '\n' << "The error in analyzing 80.402206000000035 is greater than 3e-16." << std::endl; }

                    if(isEqual(std::stod("80.272E-18") , testClass->get_value_as_double(testClass->deserialize( "80.272E-18" )))){testScore.double_accuracy +=10; }
                    else{ ofs << '\n' << "The error in analyzing 80.272E-18 is greater than 3e-16." << std::endl; }

                    if(isEqual(std::stod("80.2741E+218") , testClass->get_value_as_double(testClass->deserialize( "80.2741E+218" )))){testScore.double_accuracy +=5; }
                    else{ ofs << '\n' << "The error in analyzing 80.2741E+218 is greater than 3e-16." << std::endl; }

                    if(isEqual(std::stod("135161616.1561123") , testClass->get_value_as_double(testClass->deserialize( "135161616.1561123" )))){testScore.double_accuracy +=5; }
                    else{ ofs << '\n' << "The error in analyzing 135161616.1561123 is greater than 3e-16." << std::endl; }

                    if(isEqual(std::stod("78.524155000000121") , testClass->get_value_as_double(testClass->deserialize( "78.524155000000121" )))){testScore.double_accuracy +=5; }
                    else{ ofs << '\n' << "The error in analyzing 78.524155000000121 is greater than 3e-16." << std::endl; }

                    if(isEqual(std::stod("0.00151521515") , testClass->get_value_as_double(testClass->deserialize( "0.00151521515" )))){testScore.double_accuracy +=5; }
                    else{ ofs << '\n' << "The error in analyzing 0.00151521515 is greater than 3e-16." << std::endl; }

                    std::cout << '\r' << libName << " >> double accuracy score: " << testScore.double_accuracy << "/100" << std::string(10, ' ') << std::endl;
                }
                catch(const NotSupportException& e){
                    testScore.double_accuracy = -1;
                    ofs << " 16. double_accuracy test not support (Unsupported deserialization of value types)." << std::endl;
                    std::cout << '\r' << libName << " >> double (json value type) accuracy not support. "<< std::string(10, ' ') << std::endl;
                }
                catch(...){
                    testScore.double_accuracy = -2;
                    ofs << " 16. double_accuracy test failed." << std::endl;
                    std::cout << '\r' << libName << " >> double accuracy test failed. "<< std::string(20, ' ') << std::endl;
                }

                ofs << "\n-----------------------------------------\n" << std::endl;
            }

            // 17. 测试字符串转义支持
            {
                std::cout << libName << " >> string validity testing......"  << std::flush;
                ofs << " 17. string_validity escape test." << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                try{
                    testScore.string_validity = 0;

                    if(std::string{ "thisisastring"} == testClass->get_value_as_string(testClass->deserialize( R"_("thisisastring")_" ))) {testScore.string_validity += 30;}
                    else{  ofs << '\n' << "Wrong: The parsing result of string \"thisisastring\" is not equal to \"thisisastring\"." << std::endl;  }
                    try{ 
                        if(std::string{ "this is a string"} == testClass->get_value_as_string(testClass->deserialize( R"_("this is a string")_" ))) {testScore.string_validity += 20;}
                        else{  ofs << '\n' << "Wrong: The parsing result of <\"this is a string\"> is not equal to <this is a string>." << std::endl; }
                    }catch(...){}
                    try{ 
                        if(std::string{ "this is a\"string\""} == testClass->get_value_as_string(testClass->deserialize( R"_("this is a\"string\"")_" ))){testScore.string_validity += 5;}
                        else{  ofs << '\n' << "Wrong: The parsing result of <\"this is a\\\"string\\\"\"> is not equal to <this is a\"string\">." << std::endl; }
                    }catch(...){}
                    try{ 
                        if(std::string{ "this is a\tstring"} == testClass->get_value_as_string(testClass->deserialize( R"_("this is a\tstring")_" ))){testScore.string_validity += 5;}
                        else{  ofs << '\n' << "Wrong: The parsing result of <\"this is a\\tstring\"> is not equal to <this is a\tstring>." << std::endl; }
                    }catch(...){}
                    try{ 
                        if(std::string{ "this is a\nstring"} == testClass->get_value_as_string(testClass->deserialize( R"_("this is a\nstring")_" ))){testScore.string_validity += 5;}
                        else{  ofs << '\n' << "Wrong: The parsing result of <\"this is a\\nstring\"> is not equal to <this is a\nstring>." << std::endl; }
                    }catch(...){}
                    try{ 
                        if(std::string{ "this is a\\string"} == testClass->get_value_as_string(testClass->deserialize( R"_("this is a\\string")_" ))){testScore.string_validity += 5;}
                        else{  ofs << '\n' << "Wrong: The parsing result of <\"this is a\\\\string\"> is not equal to <this is a\\string>." << std::endl; }
                    }catch(...){}
                    try{ 
                        if(std::string{ "this is a\rstring"} == testClass->get_value_as_string(testClass->deserialize( R"_("this is a\rstring")_" ))){testScore.string_validity += 5;}
                        else{  ofs << '\n' << "Wrong: The parsing result of <\"this is a\\rstring\"> is not equal to <this is a\rstring>." << std::endl; }
                    }catch(...){}
                    try{ 
                        if(std::string{ "this is a\fstring"} == testClass->get_value_as_string(testClass->deserialize( R"_("this is a\fstring")_" ))){testScore.string_validity += 5;}
                        else{  ofs << '\n' << "Wrong: The parsing result of <\"this is a\\fstring\"> is not equal to <this is a\fstring>." << std::endl; }
                    }catch(...){}
                    try{ 
                        if(std::string{ "this is a\bstring"} == testClass->get_value_as_string(testClass->deserialize( R"_("this is a\bstring")_" ))){testScore.string_validity += 5;}
                        else{  ofs << '\n' << "Wrong: The parsing result of <\"this is a\\bstring\"> is not equal to <this is a\bstring>." << std::endl; }
                    }catch(...){}
                    try{ 
                        if(std::string{ "this is a 字符串"} == testClass->get_value_as_string(testClass->deserialize( R"_("this is a 字符串")_" ))){testScore.string_validity += 5;}
                        else{  ofs << '\n' << "Wrong: The parsing result of <\"this is a 字符串\"> is not equal to <this is a 字符串>." << std::endl; }
                    }catch(...){}
                    try{
                        if(std::string{ "this is a\u03B1string"} == testClass->get_value_as_string(testClass->deserialize( R"_("this is a\u03B1string")_" ))){testScore.string_validity += 5;}
                        else{  ofs << '\n' << "Wrong: The parsing result of <\"this is a\\u03B1string\"> is not equal to <this is a\u03B1string>." << std::endl; }
                    }catch(...){}
                    try{
                        if(std::string{ "this is a😊string"} == testClass->get_value_as_string(testClass->deserialize( R"_("this is a\uD83D\uDE0Astring")_" ))){testScore.string_validity += 5;}
                        else{  ofs << '\n' << "Wrong: The parsing result of <\"this is a\\uD83D\\uDE0Astring\"> is not equal to <this is a😊string>." << std::endl; }
                    }catch(...){}
                    std::cout << '\r' << libName << " >> string validity score: " << testScore.string_validity << "/100" << std::string(10, ' ') << std::endl;
                }
                catch(const NotSupportException& e){
                    testScore.string_validity = -1;
                    ofs << " 17. string_validity test not support (Unsupported deserialization of value types)." << std::endl;
                    std::cout << '\r' << libName << " >> string (json value type) validity not support. "<< std::string(10, ' ') << std::endl;
                }
                catch(...){
                    testScore.string_validity = -2;
                    ofs << " 17. string_validity test failed." << std::endl;
                    std::cout << '\r' << libName << " >> string validity test failed. "<< std::string(20, ' ') << std::endl;
                }

                ofs << "\n-----------------------------------------\n" << std::endl;
            }

            // 18. 检测错误格式异常支持
            {
                std::cout << libName << " >> error structure validity testing......"  << std::flush;
                ofs << " 18. error_validity test." << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                try{
                    testScore.error_validity = 0;

                    testClass->deserialize( R"_([[[[[[[[ null ]]]]]]]])_" );
                    try{
                        testClass->deserialize( R"_([[[[[[[[[[[[[[ null ]]]]]]]]]]]]]]])_" );
                        ofs << '\n' << R"_([[[[[[[[[[[[[[ null ]]]]]]]]]]]]]]])_" << " is invalid but  deserialize passed." << std::endl;
                    }catch(...){ testScore.error_validity += 5; }
                    try{
                        testClass->deserialize( R"_([[])_" );
                        ofs << '\n' << R"_([[])_" << " is invalid but  deserialize passed." << std::endl;
                    }catch(...){ testScore.error_validity += 5; }
                    try{
                        testClass->deserialize( R"_({ "123"::"123" })_" );
                        ofs << '\n' << R"_({ "123"::"123" })_" << " is invalid but  deserialize passed." << std::endl;
                    }catch(...){ testScore.error_validity += 5; }
                    try{
                        testClass->deserialize( R"_({ "123":"123", })_" );
                        ofs << '\n' << R"_({ "123":"123", })_" << " is invalid but  deserialize passed." << std::endl;
                    }catch(...){ testScore.error_validity += 5; }
                    try{
                        testClass->deserialize( R"_([123 ,123 ,])_" );
                        ofs << '\n' << R"_([123 ,123 ,])_" << " is invalid but  deserialize passed." << std::endl;
                    }catch(...){ testScore.error_validity += 5; }
                    try{
                        testClass->deserialize( R"_( { [ } ] )_" );
                        ofs << '\n' << R"_( { [ } ] )_" << " is invalid but  deserialize passed." << std::endl;
                    }catch(...){ testScore.error_validity += 5; }
                    try{
                        testClass->deserialize( R"_( a )_" );
                        ofs << '\n' << R"_( a )_" << " is invalid but  deserialize passed." << std::endl;
                    }catch(...){ testScore.error_validity += 5; }
                    try{
                        testClass->deserialize( R"_( [][] )_" );
                        ofs << '\n' << R"_( [][] )_" << " is invalid but  deserialize passed." << std::endl;
                    }catch(...){ testScore.error_validity += 5; }
                    try{
                        testClass->deserialize( R"_( [ , 123 ] )_" );
                        ofs << '\n' << R"_( [ , 123 ] )_" << " is invalid but  deserialize passed." << std::endl;
                    }catch(...){ testScore.error_validity += 5; }
                    try{ testClass->deserialize( R"_( [ " 
                        " ] )_" ); 
                        ofs << '\n' << R"_( [ " 
                        " ] )_" << " is invalid but  deserialize passed." << std::endl;
                    }catch(...){ testScore.error_validity += 5; }
                    try{
                        testClass->deserialize( R"_( [ . ] )_" );
                        ofs << '\n' << R"_( [ . ] )_" << " is invalid but  deserialize passed." << std::endl;
                    }catch(...){ testScore.error_validity += 5; }
                    try{
                        testClass->deserialize( R"_( [ flase ] )_" );
                        ofs << '\n' << R"_( [ flase ] )_" << " is invalid but  deserialize passed." << std::endl;
                    }catch(...){ testScore.error_validity += 5; }
                    try{
                        testClass->deserialize( R"_( { "123" } )_" );
                        ofs << '\n' << R"_( { "123" } )_" << " is invalid but  deserialize passed." << std::endl;
                    }catch(...){ testScore.error_validity += 5; }
                    try{
                        testClass->deserialize( R"_( { as: "fa" } )_" );
                        ofs << '\n' << R"_( { as: "fa" } )_" << " is invalid but  deserialize passed." << std::endl;
                    }catch(...){ testScore.error_validity += 5; }
                    try{
                        testClass->deserialize( R"_( { "asd" : , "fa" } )_" );
                        ofs << '\n' << R"_( { "asd" : , "fa" } )_" << " is invalid but  deserialize passed." << std::endl;
                    }catch(...){ testScore.error_validity += 5; }
                    try{
                        testClass->deserialize( R"_( { "asd": +12 } )_" );
                        ofs << '\n' << R"_( { "asd": +12 } )_" << " is invalid but  deserialize passed." << std::endl;
                    }catch(...){ testScore.error_validity += 5; }
                    try{
                        testClass->deserialize( R"_( [ , ] )_" );
                        ofs << '\n' << R"_( [ , ] )_"  << " is invalid but  deserialize passed." << std::endl;
                    }catch(...){ testScore.error_validity += 5; }
                    try{
                        testClass->deserialize( R"_( {{}} )_" );
                        ofs << '\n' << R"_( {{}} )_"  << " is invalid but  deserialize passed." << std::endl;
                    }catch(...){ testScore.error_validity += 5; }
                    try{
                        testClass->deserialize( R"_( { "": "123" } )_" ); testScore.error_validity += 5;
                    }catch(...){ 
                        ofs << '\n' << R"_( { "": "123" } )_"  << " is valid but  deserialize failed." << std::endl;
                    }
                    try{ 
                        testClass->deserialize( R"_( [ -1.E+ ] )_" ); testScore.error_validity += 5; 
                    }catch(...){
                        ofs << '\n' << R"_( [ -1.E+ ] )_"  << " is valid but  deserialize failed." << std::endl;
                    }
                    
                    std::cout << '\r' << libName << " >> error structure validity score: " << testScore.error_validity << "/100" << std::string(10, ' ') << std::endl;
                }
                catch(const NotSupportException& e){
                    testScore.error_validity = -1;
                    ofs << " 18. error_validity test not support." << std::endl;
                    std::cout << '\r' << libName << " >> error structure validity not support. "<< std::string(15, ' ') << std::endl;
                }
                catch(...){
                    testScore.error_validity = -2;
                    ofs << " 18. error_validity test failed." << std::endl;
                    std::cout << '\r' << libName << " >> error structure test failed. "<< std::string(25, ' ') << std::endl;
                }

                ofs << "\n-----------------------------------------\n" << std::endl;
            }

        }

    
        ScoreMap[libName] = testScore;
        std::cout << "[ End of test lib: " << libName << " ]" << std::endl;
        std::cout << "\n---------------------------------------------------------\n" << std::endl;
    }


    try{
        my_write_csv(ScoreMap);
    }
    catch(const std::exception& e){
        std::cerr << "fail to write result." << std::endl;
        std::cerr << e.what() << std::endl;
    }

}
