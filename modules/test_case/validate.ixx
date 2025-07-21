export module validate;

import std;
import jtu;
import test_memory;
import test_tools;

export namespace jtu {

    constexpr JsonCounter counter_big_varies{
        .m_object_num = 10937,
        .m_object_size = 25872,
        .m_array_num = 10451,
        .m_array_size = 11908,
        .m_string_num = 735,
        .m_number_num = 14392,
        .m_true_num = 2,
        .m_false_num = 1,
        .m_null_num = 1263,
    };

    void validate(
        const std::string& lib_name,
        std::shared_ptr<TestBase> test_class,
        TestScore& test_score,
        std::ofstream& ofs,
        const std::string& text
    ) noexcept {
        std::cout << "[ Begin of test lib: " << lib_name << " ]" << std::endl;
        std::size_t base_memory_KB;
        JsonCounter json_counter;


        {
            std::shared_ptr<JsonBase> base_test_ptr{ nullptr };

            // 基础测试 1.解析的数据是否正确 2.是否支持解析值类型
            // 1. validity 解析正确性测试
            try{
                std::cout << lib_name << " >> validity testing......" << std::endl;
                base_test_ptr =  test_class->deserialize(text);
                json_counter = test_class->count_element(base_test_ptr);
                if(counter_big_varies == json_counter){
                    test_score.validity = 1;
                    std::println("\r{} >> validity test passed.{}", lib_name, std::string(20, ' '));
                }else throw std::exception {};
            } catch(const std::exception& e) {
                test_score.validity = -1;
                ofs << " 1. integrity test failed:" << std::endl;
                ofs << "deserialize file: 'json_test_files/big_varies.json'" << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
                ofs << "--------------Actual Quantity--------------" << std::endl;
                ofs << "object_num: " << counter_big_varies.m_object_num << std::endl;
                ofs << "object_size: " << counter_big_varies.m_object_size << std::endl;
                ofs << "array_num: " << counter_big_varies.m_array_num << std::endl;
                ofs << "array_size: " << counter_big_varies.m_array_size << std::endl;
                ofs << "string_num: " << counter_big_varies.m_string_num << std::endl;
                ofs << "number_num: " << counter_big_varies.m_number_num << std::endl;
                ofs << "true_num: " << counter_big_varies.m_true_num << std::endl;
                ofs << "false_num: " << counter_big_varies.m_false_num << std::endl;
                ofs << "null_num: " << counter_big_varies.m_null_num << std::endl;
                ofs << "--------------Your Quantity--------------" << std::endl;
                ofs << "object_num: " << json_counter.m_object_num << std::endl;
                ofs << "object_size: " << json_counter.m_object_size << std::endl;
                ofs << "array_num: " << json_counter.m_array_num << std::endl;
                ofs << "array_size: " << json_counter.m_array_size << std::endl;
                ofs << "string_num: " << json_counter.m_string_num << std::endl;
                ofs << "number_num: " << json_counter.m_number_num << std::endl;
                ofs << "true_num: " << json_counter.m_true_num << std::endl;
                ofs << "false_num: " << json_counter.m_false_num << std::endl;
                ofs << "null_num: " << json_counter.m_null_num << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << lib_name << " >> Integrity test faile. Test terminated, please refer to the 'result/" << lib_name << "_error_report.txt'."<< std::endl;
                std::cerr << "[ End of test lib: " << lib_name << " ]" << std::endl;
                std::cerr << "\n---------------------------------------------------------\n" << std::endl;
            }

            // 2. value_type 值类型解析支持测试
            try{
                std::cout << lib_name << " >> JSON value type testing......" << std::flush;
                ofs << " 2. value type deserialize test......" << std::endl;
                ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
                ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;

                if(12345678ll != test_class->get_value_as_int64(test_class->deserialize("12345678"))){
                    long long it = test_class->get_value_as_int64(test_class->deserialize("12345678"));
                    ofs << '\n' << "String \"12345678\" parsing failed, the return result is not equal to 12345678ll." << std::endl;
                    throw FailException{ R"_(12345678ll != test_class->get_value_as_int64(test_class->deserialize("12345678")))_" };
                }
                if(std::string{"string-text-test"} != test_class->get_value_as_string(test_class->deserialize("\"string-text-test\""))){
                    ofs << '\n' << "String \"\"string-text-test\"\" parsing failed, the return result is not equal to \"string-text-test\"." << std::endl;
                    throw FailException{R"_(std::string{"string-text-test"} != test_class->get_value_as_string(test_class->deserialize("\"string-text-test\"")))_"};
                }
                if( !(test_class->get_value_as_bool(test_class->deserialize("true")))){
                    ofs << '\n' << "String \"true\" parsing failed, the return result is not equal to true." << std::endl;
                    throw FailException{R"_(!(test_class->get_value_as_bool(test_class->deserialize("true"))))_"};
                }
                if( test_class->get_value_as_bool(test_class->deserialize("false"))){
                    ofs << '\n' << "String \"false\" parsing failed, the return result is not equal to false." << std::endl;
                    throw FailException{R"_(test_class->get_value_as_bool(test_class->deserialize("false")))_"};
                }
                if( !test_class->value_is_null(test_class->deserialize("null"))){
                    ofs << '\n' << "String \"null\" parsing failed, the return result is not equal to null." << std::endl;
                    throw FailException{R"_(!test_class->value_is_null(test_class->deserialize("null")))_"};
                }
                test_score.value_type = 1;
                std::cout << '\r' << lib_name << " >> JSON value type passed." << std::string(20, ' ')<< std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
            } catch(const NotSupportException& e) {
                test_score.value_type = -1;
                ofs << " 2. value type deserialize test not support!!!" << std::endl;
                ofs << "Test samples:" << std::endl;
                ofs << R"_( test_class->get_value_as_int64(test_class->deserialize("123456789012345")) )_" << std::endl;
                ofs << R"_( test_class->get_value_as_string(test_class->deserialize("\"string-text-test\"")) )_" << std::endl;
                ofs << R"_( test_class->get_value_as_bool(test_class->deserialize("true")) )_" << std::endl;
                ofs << R"_( test_class->get_value_as_bool(test_class->deserialize("false")) )_" << std::endl;
                ofs << R"_( test_class->value_is_null(test_class->deserialize("null")) )_" << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;

                std::cerr << '\r' << lib_name << " >> JSON value type not support." << std::string(20, ' ') << std::endl;
            } catch(const std::exception& e) {
                test_score.value_type = -2;
                ofs << " 2. value type deserialize test faild!!!" << std::endl;
                ofs << "Exception result:" << e.what() << std::endl;
                ofs << "Test samples:" << std::endl;
                ofs << R"_( test_class->get_value_as_int64(test_class->deserialize("123456789012345")) )_" << std::endl;
                ofs << R"_( test_class->get_value_as_string(test_class->deserialize("\"string-text-test\"")) )_" << std::endl;
                ofs << R"_( test_class->get_value_as_bool(test_class->deserialize("true")) )_" << std::endl;
                ofs << R"_( test_class->get_value_as_bool(test_class->deserialize("false")) )_" << std::endl;
                ofs << R"_( test_class->value_is_null(test_class->deserialize("null")) )_" << std::endl;
                ofs << "\n-----------------------------------------\n" << std::endl;
                std::cerr << '\r' << lib_name << " >> JSON value type failed." << std::string(20, ' ') << std::endl;
            }


            std::this_thread::sleep_for(std::chrono::seconds(1));
            base_memory_KB = get_memory_usage_KB();
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        test_score.memory = base_memory_KB - get_memory_usage_KB();
        std::cout << lib_name << " >> memory used:" << test_score.memory << " K" << std::string(15, ' ') << std::endl;
    }

}

