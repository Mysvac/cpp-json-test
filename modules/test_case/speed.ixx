export module speed;

import std;
import jtu;
import test_tools;

export namespace jtu {
    void speed(
        const std::string& lib_name,
        std::shared_ptr<TestBase> test_class,
        TestScore& test_score,
        std::ofstream& ofs,
        const std::string& big_normal,
        const std::string& big_double,
        const std::string& big_string
    ) noexcept {
        std::shared_ptr<JsonBase> json_ptr_1 = nullptr;
        std::shared_ptr<JsonBase> json_ptr_2 = nullptr;
        std::shared_ptr<JsonBase> json_ptr_3 = nullptr;

        // 1-1. big_normal.json 反序列化测试 各类数据均衡的文件
        try{
            std::cout << lib_name << " >> deserialize (1) testing......"  << std::flush;
            const auto begin = std::chrono::system_clock::now();

            json_ptr_1 =  test_class->deserialize(big_normal);

            const auto end = std::chrono::system_clock::now();
            const auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
            test_score.deserialize_1 = micros.count();
            std::cout << '\r' << lib_name << " >> deserialize (1) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
        }
        catch(const NotSupportException& ){
            test_score.deserialize_1 = -1;
            ofs << " 3. deserialize (1) test not support." << std::endl;
            ofs << " file: json_test_files/big_normal.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> deserialize (1) not support." << std::string(20, ' ') << std::endl;
        }
        catch(...){
            test_score.deserialize_1 = -2;
            ofs << " 3. deserialize (1) test failed." << std::endl;
            ofs << " file: json_test_files/big_normal.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> deserialize (1) failed." << std::string(20, ' ') << std::endl;
        }

        // 1-2. big_doubles.json 反序列化测试 包含大量数值类型的文件
        try{
            std::cout << lib_name << " >> deserialize (2) testing......"  << std::flush;
            const auto begin = std::chrono::system_clock::now();

            json_ptr_2 =  test_class->deserialize(big_double);

            const auto end = std::chrono::system_clock::now();
            const auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
            test_score.deserialize_2 = micros.count();
            std::cout << '\r' << lib_name << " >> deserialize (2) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
        }
        catch(const NotSupportException& ){
            test_score.deserialize_2 = -1;
            ofs << " 4. deserialize (2) test not support." << std::endl;
            ofs << " file: json_test_files/big_double.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> deserialize (2) not support." << std::string(20, ' ') << std::endl;
        }
        catch(...){
            test_score.deserialize_2 = -2;
            ofs << " 4. deserialize (2) test failed." << std::endl;
            ofs << " file: json_test_files/big_double.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> deserialize (2) failed." << std::string(20, ' ') << std::endl;
        }

        // 1-3. big_string.json 反序列化测试 包含大量字符串类型的文件
        try{
            std::cout << lib_name << " >> deserialize (3) testing......"  << std::flush;
            const auto begin = std::chrono::system_clock::now();

            json_ptr_3 =  test_class->deserialize(big_string);

            const auto end = std::chrono::system_clock::now();
            const auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
            test_score.deserialize_3 = micros.count();
            std::cout << '\r' << lib_name << " >> deserialize (3) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
        }
        catch(const NotSupportException& ){
            test_score.deserialize_3 = -1;
            ofs << " 4. deserialize (3) test not support." << std::endl;
            ofs << " file: json_test_files/big_string.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> deserialize (3) not support." << std::string(20, ' ') << std::endl;
        }
        catch(...){
            test_score.deserialize_3 = -2;
            ofs << " 4. deserialize (3) test failed." << std::endl;
            ofs << " file: json_test_files/big_string.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> deserialize (3) failed." << std::string(20, ' ') << std::endl;
        }

        // 2-1. big_normal.json 序列化测试 各类数据均衡的文件
        try{
            std::cout << lib_name << " >> serialize (1) testing......"  << std::flush;
            const auto begin = std::chrono::system_clock::now();
            if(json_ptr_1 == nullptr) throw FailException{};
            const auto tmp =  test_class->serialize(json_ptr_1);

            const auto end = std::chrono::system_clock::now();
            const auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
            std::string s = tmp->get_string();
            s[1] = '1'; // 测试内容是否正常
            test_score.serialize_1 = micros.count();
            std::cout << '\r' << lib_name << " >> serialize (1) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
        }
        catch(const NotSupportException& ){
            test_score.serialize_1 = -1;
            ofs << " 6. serialize (1) test not support." << std::endl;
            ofs << " file: json_test_files/big_normal.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> serialize (1) not support." << std::string(20, ' ') << std::endl;
        }
        catch(...){
            test_score.serialize_1 = -2;
            ofs << " 6. serialize (1) test failed." << std::endl;
            ofs << " file: json_test_files/big_normal.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> serialize (1) failed."  << std::string(20, ' ') << std::endl;
        }

        // 2-2. big_doubles.json 序列化测试 包含大量数值类型的文件
        try{
            std::cout << lib_name << " >> serialize (2) testing......"  << std::flush;
            const auto begin = std::chrono::system_clock::now();
            if(json_ptr_2 == nullptr) throw FailException{};
            const auto tmp =  test_class->serialize(json_ptr_2);

            const auto end = std::chrono::system_clock::now();
            const auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
            std::string s = tmp->get_string();
            s[1] = '1'; // 测试内容是否正常
            test_score.serialize_2 = micros.count();
            std::cout << '\r' << lib_name << " >> serialize (2) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
        }
        catch(const NotSupportException& ){
            test_score.serialize_2 = -1;
            ofs << " 7. serialize (2) test not support." << std::endl;
            ofs << " file: json_test_files/big_double.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> serialize (2) not support." << std::string(20, ' ') << std::endl;
        }
        catch(...){
            test_score.serialize_2 = -2;
            ofs << " 7. serialize (2) test failed." << std::endl;
            ofs << " file: json_test_files/big_double.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> serialize (2) failed."  << std::string(20, ' ')<< std::endl;
        }

        // 2-3. big_string.json 序列化测试 包含大量字符串的文件
        try{
            std::cout << lib_name << " >> serialize (3) testing......"  << std::flush;
            const auto begin = std::chrono::system_clock::now();
            if(json_ptr_3 == nullptr) throw FailException{};
            const auto tmp =  test_class->serialize(json_ptr_3);

            const auto end = std::chrono::system_clock::now();
            const auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
            std::string s = tmp->get_string();
            s[1] = '1'; // 测试内容是否正常
            test_score.serialize_3 = micros.count();
            std::cout << '\r' << lib_name << " >> serialize (3) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
        }
        catch(const NotSupportException& ){
            test_score.serialize_3 = -1;
            ofs << " 7. serialize (3) test not support." << std::endl;
            ofs << " file: json_test_files/big_string.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> serialize (3) not support." << std::string(20, ' ') << std::endl;
        }
        catch(...){
            test_score.serialize_3 = -2;
            ofs << " 7. serialize (3) test failed." << std::endl;
            ofs << " file: json_test_files/big_string.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> serialize (3) failed."  << std::string(20, ' ')<< std::endl;
        }

        // 3-1. big_normal.json 序列化+美化测试 类型均匀的文件
        try{
            std::cout << lib_name << " >> serialize_pretty (1) testing......"  << std::flush;
            const auto begin = std::chrono::system_clock::now();
            if(json_ptr_1 == nullptr) throw FailException{};
            const auto tmp =  test_class->serialize_pretty(json_ptr_1);

            const auto end = std::chrono::system_clock::now();
            const auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
            test_score.prettify_1 = micros.count();
            std::cout << '\r' << lib_name << " >> serialize_pretty (1) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
        }
        catch(const NotSupportException& ){
            test_score.prettify_1 = -1;
            ofs << " 9. serialize_pretty (1) test not support." << std::endl;
            ofs << " file: json_test_files/big_normal.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> serialize_pretty (1) not support." << std::string(20, ' ') << std::endl;
        }
        catch(...){
            test_score.prettify_1 = -2;
            ofs << " 9. serialize_pretty (1) test failed." << std::endl;
            ofs << " file: json_test_files/big_normal.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> serialize_pretty (1) failed."  << std::string(20, ' ') << std::endl;
        }

        // 3-2 big_doubles.json 序列化+美化测试 包含大量数值类型的文件
        try{
            std::cout << lib_name << " >> serialize_pretty (2) testing......"  << std::flush;
            const auto begin = std::chrono::system_clock::now();
            if(json_ptr_2 == nullptr) throw FailException{};
            const auto tmp =  test_class->serialize_pretty(json_ptr_2);

            const auto end = std::chrono::system_clock::now();
            const auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
            test_score.prettify_2 = micros.count();
            std::cout << '\r' << lib_name << " >> serialize_pretty (2) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
        }
        catch(const NotSupportException& ){
            test_score.prettify_2 = -1;
            ofs << " 10. serialize_pretty (2) test not support." << std::endl;
            ofs << " file: json_test_files/big_double.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> serialize_pretty (2) not support." << std::string(20, ' ')<< std::endl;
        }
        catch(...){
            test_score.prettify_2 = -2;
            ofs << " 10. serialize_pretty (2) test failed." << std::endl;
            ofs << " file: json_test_files/big_double.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> serialize_pretty (2) failed." << std::string(20, ' ') << std::endl;
        }

        // 3-3 big_string.json 序列化+美化测试 包含大量字符串的文件
        try{
            std::cout << lib_name << " >> serialize_pretty (3) testing......"  << std::flush;
            const auto begin = std::chrono::system_clock::now();
            if(json_ptr_3 == nullptr) throw FailException{};
            const auto tmp =  test_class->serialize_pretty(json_ptr_3);

            std::ofstream ofs { PROJECT_RESULT_PATH "/" + lib_name + "_str.json"  };
            ofs.write(tmp->get_string().c_str(), tmp->get_string().size());

            const auto end = std::chrono::system_clock::now();
            const auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
            test_score.prettify_3 = micros.count();
            std::cout << '\r' << lib_name << " >> serialize_pretty (3) passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;
        }
        catch(const NotSupportException& ){
            test_score.prettify_3 = -1;
            ofs << " 10. serialize_pretty (3) test not support." << std::endl;
            ofs << " file: json_test_files/big_string.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> serialize_pretty (3) not support." << std::string(20, ' ')<< std::endl;
        }
        catch(...){
            test_score.prettify_3 = -2;
            ofs << " 10. serialize_pretty (3) test failed." << std::endl;
            ofs << " file: json_test_files/big_string.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> serialize_pretty (3) failed." << std::string(20, ' ') << std::endl;
        }
        
    }
}
