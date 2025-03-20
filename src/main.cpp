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
            big_normal = readFile("json_test_files/big_normal.json");
            big_double = readFile("json_test_files/big_double.json");
            big_nesting = readFile("json_test_files/big_nesting.json");
            big_varies  = readFile("json_test_files/big_varies.json");
            big_test  = readFile("json_test_files/big_test.json");
            big_object = readFile("json_test_files/big_object.json");
            big_array = readFile("json_test_files/big_array.json");
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
        std::cout << "[ Begin of test lib: " << libName << " ]" << std::endl;

        // 基础的正确性和值类型支持性测试
        {
            // 基础库有效性测试，如果失败，直接结束
            try{
                std::cout << libName << " >> integrity testing......"  << std::flush;
                std::shared_ptr<JsonBase> json_ptr =  testClass->unserialize(big_varies);

                JsonCounter jsonCounter = testClass->count_element(json_ptr);

                if(counter_big_varies == jsonCounter){
                    testScore.integrity = 1;
                    std::cout << '\r' << libName << " >> integrity test passed." << std::string(20, ' ') << std::endl;
                }
                else throw std::exception {};
            }
            catch(...){
                testScore.integrity = -1;
                std::cerr << '\r' << libName << " >> integrity test failed. This library cannot parse data correctly!!!!!!!" << std::string(10, ' ')<< std::endl;
                std::cerr << "[ End of test lib: " << libName << " ]" << std::endl;
                std::cerr << "\n---------------------------------------------------------\n" << std::endl;
                continue;
            }
        
            // 数值类型解析支持性测试
            try{
                std::cout << libName << " >> JSON value type testing......" << std::flush;
                auto t1 = testClass->unserialize("123456789012345");
                auto t2 = testClass->unserialize("\"string text test\"");
                auto t3 = testClass->unserialize("true");
                auto t4 = testClass->unserialize("false");
                auto t5 = testClass->unserialize("null");
                testScore.value_unserialize = 1;
                std::cout << '\r' << libName << " >> JSON value type passed." << std::string(20, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.value_unserialize = -1;
                std::cerr << '\r' << libName << " >> JSON value type not support." << std::string(20, ' ') << std::endl;
            }
            catch(...){
                testScore.value_unserialize = -2;
                std::cerr << '\r' << libName << " >> JSON value type failed." << std::string(20, ' ') << std::endl;
            }
        }

        // 反序列化与序列化测速
        {
            // 三个指针，分别存储上面读取的3个文件的反序列化结果
            std::shared_ptr<JsonBase> json_ptr_1 = nullptr;
            std::shared_ptr<JsonBase> json_ptr_2 = nullptr;
            std::shared_ptr<JsonBase> json_ptr_3 = nullptr;

            // big_normal.json 反序列化测试 较大的文本内容为主的文件
            try{
                std::cout << libName << " >> unserialize (1) testing......"  << std::flush;
                auto begin = std::chrono::system_clock::now();

                json_ptr_1 =  testClass->unserialize(big_normal);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                testScore.unserialize_time_1 = micros.count();
                std::cout << '\r' << libName << " >> unserialize (1) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.unserialize_time_1 = -1;
                std::cerr << '\r' << libName << " >> unserialize (1) not support." << std::string(20, ' ') << std::endl;
            }
            catch(...){
                testScore.unserialize_time_1 = -2;
                std::cerr << '\r' << libName << " >> unserialize (1) failed." << std::string(20, ' ') << std::endl;
            }

            // big_doubles.json 反序列化测试 包含大量数值类型的文件
            try{
                std::cout << libName << " >> unserialize (2) testing......"  << std::flush;
                auto begin = std::chrono::system_clock::now();

                json_ptr_2 =  testClass->unserialize(big_double);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                testScore.unserialize_time_2 = micros.count();
                std::cout << '\r' << libName << " >> unserialize (2) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.unserialize_time_2 = -1;
                std::cerr << '\r' << libName << " >> unserialize (2) not support." << std::string(20, ' ') << std::endl;
            }
            catch(...){
                testScore.unserialize_time_2 = -2;
                std::cerr << '\r' << libName << " >> unserialize (2) failed." << std::string(20, ' ') << std::endl;
            }

            // big_nesting 反序列化测试 含有较深层次的文件，内容较少
            try{
                std::cout << libName << " >> unserialize (3) testing......"  << std::flush;
                auto begin = std::chrono::system_clock::now();

                json_ptr_3 =  testClass->unserialize(big_nesting);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                testScore.unserialize_time_3 = micros.count();
                std::cout << '\r' << libName << " >> unserialize (3) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(1, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.unserialize_time_3 = -1;
                std::cerr << '\r' << libName << " >> unserialize (3) not support." << std::string(20, ' ') << std::endl;
            }
            catch(...){
                testScore.unserialize_time_3 = -2;
                std::cerr << '\r' << libName << " >> unserialize (3) failed." << std::string(20, ' ') << std::endl;
            }

            // big_normal.json 序列化测试 较大的文本内容为主的文件
            try{
                std::cout << libName << " >> serialize (1) testing......"  << std::flush;
                auto begin = std::chrono::system_clock::now();

                auto tmp =  testClass->serialize(json_ptr_1);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                std::string s = tmp->getString();
                s[1] = '1'; // 测试内容是否正常
                testScore.serialize_time_1 = micros.count();
                std::cout << '\r' << libName << " >> serialize (1) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.serialize_time_1 = -1;
                std::cerr << '\r' << libName << " >> serialize (1) not support." << std::string(20, ' ') << std::endl;
            }
            catch(...){
                testScore.serialize_time_1 = -2;
                std::cerr << '\r' << libName << " >> serialize (1) failed."  << std::string(20, ' ') << std::endl;
            }

            // big_doubles.json 序列化测试 包含大量数值类型的文件
            try{
                std::cout << libName << " >> serialize (2) testing......"  << std::flush;
                auto begin = std::chrono::system_clock::now();

                auto tmp =  testClass->serialize(json_ptr_2);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                std::string s = tmp->getString();
                s[1] = '1'; // 测试内容是否正常
                testScore.serialize_time_2 = micros.count();
                std::cout << '\r' << libName << " >> serialize (2) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.serialize_time_2 = -1;
                std::cerr << '\r' << libName << " >> serialize (2) not support." << std::string(20, ' ') << std::endl;
            }
            catch(...){
                testScore.serialize_time_2 = -2;
                std::cerr << '\r' << libName << " >> serialize (2) failed."  << std::string(20, ' ')<< std::endl;
            }

            // big_nesting 序列化测试 含有较深层次的文件，内容较少
            try{
                std::cout << libName << " >> serialize (3) testing......"  << std::flush;
                auto begin = std::chrono::system_clock::now();

                auto tmp =  testClass->serialize(json_ptr_3);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                std::string s = tmp->getString();
                s[1] = '1'; // 测试内容是否正常
                testScore.serialize_time_3 = micros.count();
                std::cout << '\r' << libName << " >> serialize (3) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.serialize_time_3 = -1;
                std::cerr << '\r' << libName << " >> serialize (3) not support." << std::string(20, ' ') << std::endl;
            }
            catch(...){
                testScore.serialize_time_3 = -2;
                std::cerr << '\r' << libName << " >> serialize (3) failed." << std::string(20, ' ') << std::endl;
            }

            // big_normal.json 序列化+美化测试 较大的文本内容为主的文件
            try{
                std::cout << libName << " >> serialize_pretty (1) testing......"  << std::flush;
                auto begin = std::chrono::system_clock::now();

                auto tmp =  testClass->serialize_pretty(json_ptr_1);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                testScore.serialize_pretty_time_1 = micros.count();
                std::cout << '\r' << libName << " >> serialize_pretty (1) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.serialize_pretty_time_1 = -1;
                std::cerr << '\r' << libName << " >> serialize_pretty (1) not support." << std::string(20, ' ') << std::endl;
            }
            catch(...){
                testScore.serialize_pretty_time_1 = -2;
                std::cerr << '\r' << libName << " >> serialize_pretty (1) failed."  << std::string(20, ' ') << std::endl;
            }

            // big_doubles.json 序列化+美化测试 包含大量数值类型的文件
            try{
                std::cout << libName << " >> serialize_pretty (2) testing......"  << std::flush;
                auto begin = std::chrono::system_clock::now();

                auto tmp =  testClass->serialize_pretty(json_ptr_2);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                testScore.serialize_pretty_time_2 = micros.count();
                std::cout << '\r' << libName << " >> serialize_pretty (2) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.serialize_pretty_time_2 = -1;
                std::cerr << '\r' << libName << " >> serialize_pretty (2) not support." << std::string(20, ' ')<< std::endl;
            }
            catch(...){
                testScore.serialize_pretty_time_2 = -2;
                std::cerr << '\r' << libName << " >> serialize_pretty (2) failed." << std::string(20, ' ') << std::endl;
            }

            // big_nesting 序列化+美化测试 含有较深层次的文件，内容较少
            try{
                std::cout << libName << " >> serialize_pretty (3) testing......"  << std::flush;
                auto begin = std::chrono::system_clock::now();

                auto tmp =  testClass->serialize_pretty(json_ptr_3);

                auto end = std::chrono::system_clock::now();
                auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
                testScore.serialize_pretty_time_3 = micros.count();
                std::cout << '\r' << libName << " >> serialize_pretty (3) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.serialize_pretty_time_3 = -1;
                std::cerr << '\r' << libName << " >> serialize_pretty (3) not support." << std::string(20, ' ') << std::endl;
            }
            catch(...){
                testScore.serialize_pretty_time_3 = -2;
                std::cerr << '\r' << libName << " >> serialize_pretty (3) failed." << std::string(20, ' ') << std::endl;
            }
        
        }


        // 访问 增加 删除 速度测试
        {
            // 测试内部访问速度 包含对象复制耗时
            try{
                std::cout << libName << " >> get child speed testing......"  << std::flush;
                std::shared_ptr<JsonBase> json_ptr = testClass->unserialize(big_test);
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
                testScore.get_inner = micros.count();
                std::cout << '\r' << libName << " >> get child speed testing passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms"  << std::string(10, ' ')<< std::endl;
            }
            catch(const NotSupportException& e){
                testScore.get_inner = -1;
                std::cerr << '\r' << libName << " >> get child test not support." << std::string(20, ' ') << std::endl;
            }
            catch(...){
                testScore.get_inner = -2;
                std::cerr << '\r' << libName << " >> get child test failed." << std::string(20, ' ')<< std::endl;
                continue;
            }

            // 添加内容耗时
            try{
                std::cout << libName << " >> add child speed testing......" << std::flush;
                std::shared_ptr<JsonBase> json_map = testClass->unserialize(big_test);
                std::shared_ptr<JsonBase> json_list = testClass->unserialize( "[ true , false ]" );
                std::shared_ptr<JsonBase> json_list_tmp = testClass->unserialize( "[ null , 1 ]" );
                auto begin = std::chrono::system_clock::now();

                for(int i = 1;i<=500;++i){
                    testClass->add_child_to_array(json_list, i , testClass->unserialize( "[" + std::to_string(i) + "]" ) );
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
                std::cerr << '\r' << libName << " >> add child test not support." << std::string(20, ' ') << std::endl;
            }
            catch(...){
                testScore.add_child = -2;
                std::cerr << '\r' << libName << " >> add child test failed." << std::string(20, ' ')<< std::endl;
                continue;
            }

            // 删除内容耗时
            try{
                std::cout << libName << " >> delete child speed testing......"  << std::flush;
                std::shared_ptr<JsonBase> json_ptr =  testClass->unserialize(big_varies);
                std::shared_ptr<JsonBase> json_map = testClass->unserialize(big_object);
                std::shared_ptr<JsonBase> json_list = testClass->unserialize(big_array);
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
                std::cerr << '\r' << libName << " >> delete child test not support." << std::string(20, ' ') << std::endl;
            }
            catch(...){
                testScore.delete_child = -2;
                std::cerr << '\r' << libName << " >> delete child test failed." << std::string(20, ' ')<< std::endl;
                continue;
            }

        }

        // 准确性测试，错误查询测试
        {
            // 浮点数格式解析
            {
                std::cout << libName << " >> double validity testing......"  << std::flush;
                testScore.double_validity = 0;
                try{ testClass->unserialize( "[ 0.14351516 ]" ); testScore.double_validity += 10; }catch(...){}
                try{ testClass->unserialize( "[ 512561.16551 ]" ); testScore.double_validity += 10; }catch(...){}
                try{ testClass->unserialize( "[ 51161.0 ]" ); testScore.double_validity += 5; }catch(...){}
                try{ testClass->unserialize( "[ 51161. ]" ); testScore.double_validity += 5; }catch(...){}

                try{ testClass->unserialize( "[ 51161e6 ]" ); testScore.double_validity += 5; }catch(...){}
                try{ testClass->unserialize( "[ 51.161E-6 ]" ); testScore.double_validity += 5; }catch(...){}

                try{ testClass->unserialize( "[ 51161.e+6 ]" ); testScore.double_validity += 5; }catch(...){}
                try{ testClass->unserialize( "[ -51161.4e-6 ]" ); testScore.double_validity += 5; }catch(...){}
                try{ testClass->unserialize( "[ 0.1E+6 ]" ); testScore.double_validity += 5; }catch(...){}
                try{ testClass->unserialize( "[ 0 ]" ); testScore.double_validity += 5; }catch(...){}


                try{ testClass->unserialize( "[ 0. ]" ); testScore.double_validity += 4; }catch(...){}
                try{ testClass->unserialize( "[ +5131 ]" ); }catch(...){ testScore.double_validity += 4; }
                try{ testClass->unserialize( "[ 01 ]" );  }catch(...){ testScore.double_validity += 4; }
                try{ testClass->unserialize( "[ 0E8 ]" );  }catch(...){ testScore.double_validity += 4;}

                try{ testClass->unserialize( "[ .51516 ]" ); }catch(...){ testScore.double_validity += 4; }
                try{ testClass->unserialize( "[ 51.516. ]" ); }catch(...){ testScore.double_validity += 4; }
                try{ testClass->unserialize( "[ e51 ]" ); }catch(...){ testScore.double_validity += 4; }
                try{ testClass->unserialize( "[ 515E+0.5 ]" ); }catch(...){ testScore.double_validity += 4; }

                try{ testClass->unserialize( "[ 5153616p16.15461616 ]"); }catch(...){ testScore.double_validity += 4; }
                try{ testClass->unserialize( "[ 1241351616161e. ]"); }catch(...){ testScore.double_validity += 4; }

                std::cout << '\r' << libName << " >> double validity score: " << testScore.double_validity << "/100 ." << std::string(10, ' ') << std::endl;
            }

            // 浮点数精度
            {
                std::cout << libName << " >> double accuracy testing......"  << std::flush;
                try{
                    testScore.double_accuracy = 0;
                    if(isEqual(std::stod("1231415152.0") , testClass->get_value_as_double(testClass->unserialize( "1231415152.0" )))){ testScore.double_accuracy +=10; };
                    if(isEqual(std::stod("51161.0e+6") , testClass->get_value_as_double(testClass->unserialize( "51161.0e+6" )))){testScore.double_accuracy +=10; };
                    if(isEqual(std::stod("512561.16551") , testClass->get_value_as_double(testClass->unserialize( "512561.16551" )))){testScore.double_accuracy +=10; };
                    if(isEqual(std::stod("-61.973609999999951") , testClass->get_value_as_double(testClass->unserialize( "-61.973609999999951" )))){testScore.double_accuracy +=10; };
                    if(isEqual(std::stod("-126.64472999999992") , testClass->get_value_as_double(testClass->unserialize( "-126.64472999999992" )))){testScore.double_accuracy +=10; };
                    if(isEqual(std::stod("78.2322080000014e-156") , testClass->get_value_as_double(testClass->unserialize( "78.2322080000014e-156" )))){testScore.double_accuracy +=10; };
                    if(isEqual(std::stod("80.402206000000035") , testClass->get_value_as_double(testClass->unserialize( "80.402206000000035" )))){testScore.double_accuracy +=10; };
                    if(isEqual(std::stod("80.272E-18") , testClass->get_value_as_double(testClass->unserialize( "80.272E-18" )))){testScore.double_accuracy +=10; };
                    if(isEqual(std::stod("80.2741E+218") , testClass->get_value_as_double(testClass->unserialize( "80.2741E+218" )))){testScore.double_accuracy +=5; };
                    if(isEqual(std::stod("135161616.1561123") , testClass->get_value_as_double(testClass->unserialize( "135161616.1561123" )))){testScore.double_accuracy +=5; };
                    if(isEqual(std::stod("78.524155000000121") , testClass->get_value_as_double(testClass->unserialize( "78.524155000000121" )))){testScore.double_accuracy +=5; };
                    if(isEqual(std::stod("0.00151521515") , testClass->get_value_as_double(testClass->unserialize( "0.00151521515" )))){testScore.double_accuracy +=5; };
                    std::cout << '\r' << libName << " >> double accuracy score: " << testScore.double_accuracy << "/100 ." << std::string(10, ' ') << std::endl;
                }
                catch(const NotSupportException& e){
                    testScore.double_accuracy = -1;
                    std::cout << '\r' << libName << " >> double (json value type) accuracy not support. "<< std::string(10, ' ') << std::endl;
                }
                catch(...){
                    testScore.double_accuracy = -2;
                    std::cout << '\r' << libName << " >> double accuracy test failed. "<< std::string(20, ' ') << std::endl;
                }
            }

            // 测试字符串转义支持
            {
                std::cout << libName << " >> string validity testing......"  << std::flush;
                try{
                    testScore.string_validity = 0;

                    if(std::string{ "thisisastring"} == testClass->get_value_as_string(testClass->unserialize( R"_("thisisastring")_" ))) {testScore.string_validity += 30;}
                    try{ if(std::string{ "this is a string"} == testClass->get_value_as_string(testClass->unserialize( R"_("this is a string")_" ))) {testScore.string_validity += 20;} }catch(...){}
                    try{ if(std::string{ "this is a\"string\""} == testClass->get_value_as_string(testClass->unserialize( R"_("this is a\"string\"")_" ))){testScore.string_validity += 5;} }catch(...){}
                    try{ if(std::string{ "this is a\tstring"} == testClass->get_value_as_string(testClass->unserialize( R"_("this is a\tstring")_" ))){testScore.string_validity += 5;} }catch(...){}
                    try{ if(std::string{ "this is a\nstring"} == testClass->get_value_as_string(testClass->unserialize( R"_("this is a\nstring")_" ))){testScore.string_validity += 5;} }catch(...){}
                    try{ if(std::string{ "this is a\\string"} == testClass->get_value_as_string(testClass->unserialize( R"_("this is a\\string")_" ))){testScore.string_validity += 5;} }catch(...){}
                    try{ if(std::string{ "this is a\rstring"} == testClass->get_value_as_string(testClass->unserialize( R"_("this is a\rstring")_" ))){testScore.string_validity += 5;} }catch(...){}
                    try{ if(std::string{ "this is a\fstring"} == testClass->get_value_as_string(testClass->unserialize( R"_("this is a\fstring")_" ))){testScore.string_validity += 5;} }catch(...){}
                    try{ if(std::string{ "this is a\bstring"} == testClass->get_value_as_string(testClass->unserialize( R"_("this is a\bstring")_" ))){testScore.string_validity += 5;} }catch(...){}
                    try{ if(std::string{ "this is a 字符串"} == testClass->get_value_as_string(testClass->unserialize( R"_("this is a 字符串")_" ))){testScore.string_validity += 5;} }catch(...){}
                    try{ if(std::string{ "this is a\u03B1string"} == testClass->get_value_as_string(testClass->unserialize( R"_("this is a\u03B1string")_" ))){testScore.string_validity += 5;} }catch(...){}
                    try{ if(std::string{ "this is a😊string"} == testClass->get_value_as_string(testClass->unserialize( R"_("this is a\uD83D\uDE0Astring")_" ))){testScore.string_validity += 5;} }catch(...){}
                    std::cout << '\r' << libName << " >> string validity score: " << testScore.string_validity << "/100 ." << std::string(10, ' ') << std::endl;
                }
                catch(const NotSupportException& e){
                    testScore.string_validity = -1;
                    std::cout << '\r' << libName << " >> string (json value type) validity not support. "<< std::string(10, ' ') << std::endl;
                }
                catch(...){
                    testScore.string_validity = -2;
                    std::cout << '\r' << libName << " >> string validity test failed. "<< std::string(20, ' ') << std::endl;
                }
            }

            // 错误格式检测
            {
                std::cout << libName << " >> error structure validity testing......"  << std::flush;
                try{
                    testScore.error_validity = 0;

                    testClass->unserialize( R"_([[[[[[[[ null ]]]]]]]])_" );
                    try{ testClass->unserialize( R"_([[[[[[[[[[[[[[ null ]]]]]]]]]]]]]]])_" ); }catch(...){ testScore.error_validity += 5; }
                    try{ testClass->unserialize( R"_([[])_" ); }catch(...){ testScore.error_validity += 5; }
                    try{ testClass->unserialize( R"_({ "123"::"123" })_" ); }catch(...){ testScore.error_validity += 5; }
                    try{ testClass->unserialize( R"_({ "123":"123", })_" ); }catch(...){ testScore.error_validity += 5; }
                    try{ testClass->unserialize( R"_([123 ,123 ,])_" ); }catch(...){ testScore.error_validity += 5; }
                    try{ testClass->unserialize( R"_( { [ } ] )_" ); }catch(...){ testScore.error_validity += 5; }
                    try{ testClass->unserialize( R"_( a )_" ); }catch(...){ testScore.error_validity += 5; }
                    try{ testClass->unserialize( R"_( [][] )_" ); }catch(...){ testScore.error_validity += 5; }
                    try{ testClass->unserialize( R"_( [ , 123 ] )_" ); }catch(...){ testScore.error_validity += 5; }
                    try{ testClass->unserialize( R"_( [ " 
                        " ] )_" ); }catch(...){ testScore.error_validity += 5; }
                    try{ testClass->unserialize( R"_( [ . ] )_" ); }catch(...){ testScore.error_validity += 5; }
                    try{ testClass->unserialize( R"_( [ flase ] )_" ); }catch(...){ testScore.error_validity += 5; }
                    try{ testClass->unserialize( R"_( { 123 } )_" ); }catch(...){ testScore.error_validity += 5; }
                    try{ testClass->unserialize( R"_( { as: "fa" } )_" ); }catch(...){ testScore.error_validity += 5; }
                    try{ testClass->unserialize( R"_( { "asd" : , "fa" } )_" ); }catch(...){ testScore.error_validity += 5; }
                    try{ testClass->unserialize( R"_( { "asd": +12 } )_" ); }catch(...){ testScore.error_validity += 5; }
                    try{ testClass->unserialize( R"_( [ , ] )_" ); }catch(...){ testScore.error_validity += 5; }
                    try{ testClass->unserialize( R"_( {{}} )_" ); }catch(...){ testScore.error_validity += 5; }
                    try{ testClass->unserialize( R"_( { "": "123" } )_" ); testScore.error_validity += 5; }catch(...){ }
                    try{ testClass->unserialize( R"_( [ -1.E+ ] )_" ); testScore.error_validity += 5; }catch(...){ }
                    
                    std::cout << '\r' << libName << " >> error structure validity score: " << testScore.error_validity << "/100 ." << std::string(10, ' ') << std::endl;
                }
                catch(const NotSupportException& e){
                    testScore.error_validity = -1;
                    std::cout << '\r' << libName << " >> error structure validity not support. "<< std::string(15, ' ') << std::endl;
                }
                catch(...){
                    testScore.error_validity = -2;
                    std::cout << '\r' << libName << " >> error structure test failed. "<< std::string(25, ' ') << std::endl;
                }
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
