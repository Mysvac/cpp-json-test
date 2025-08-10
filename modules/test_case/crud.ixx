export module crud;

import std;
import jtu;
import test_tools;

export namespace jtu {
    void crud(
        const std::string& lib_name,
        std::shared_ptr<TestBase> test_class,
        TestScore& test_score,
        std::ofstream& ofs,
        const std::string& crud_file,
        const std::string& big_varies,
        const std::string& big_object ,
        const std::string& big_array
    ) noexcept {
        std::shared_ptr<JsonBase> json_ptr = nullptr;

        // 1. Get and copy
        try {
            std::cout << lib_name << " >> crud testing......"  << std::flush;
            std::shared_ptr<JsonBase> json_ptr = test_class->deserialize(crud_file);
            const auto begin = std::chrono::steady_clock::now();

            for (int cnt=0;cnt<10;++cnt) {
                for(int i = 1;i<=99;++i){
                    std::ignore = test_class->get_value_as_int64(test_class->get_child_from_object(json_ptr, std::to_string(i)));
                }
                for(int i=100;i<=199;++i){
                    std::ignore = test_class->get_value_as_string(test_class->get_child_from_object(json_ptr, std::to_string(i)));
                }
                for(int i=300;i<=399;++i){
                    std::ignore = test_class->get_value_as_bool(test_class->get_child_from_object(json_ptr, std::to_string(i)));
                }
                for(int i = 5; i<=95;++i){
                    std::ignore = test_class->get_value_as_double(
                        test_class->get_child_from_array(
                            test_class->get_child_from_object(json_ptr, "list"),
                            i
                        )
                    );
                }
                for(int i = 1;i<=99;++i){
                    std::ignore = test_class->get_value_as_int64(
                        test_class->get_child_from_object(
                            test_class->get_child_from_array(
                                test_class->get_child_from_object(json_ptr, "list"),
                                1
                            ),
                            std::to_string(i)
                        )
                    );
                }
            }
            const auto end = std::chrono::steady_clock::now();
            const auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
            test_score.get_child = micros.count();
            std::cout << '\r' << lib_name << " >> crud-get passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;

        } catch(const NotSupportException& ){
            test_score.get_child = -1;
            ofs << " crud-get test not support." << std::endl;
            ofs << " file: json_test_files/crud_file.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> crud-get not support." << std::string(20, ' ') << std::endl;
        }
        catch(...){
            test_score.get_child = -2;
            ofs << " crud-get test failed." << std::endl;
            ofs << " file: json_test_files/crud_file.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> crud-get failed." << std::string(20, ' ') << std::endl;
        }

        // add and copy test
        try {
            std::cout << lib_name << " >> add child speed testing......" << std::flush;
            std::shared_ptr<JsonBase> json_map = test_class->deserialize(crud_file);
            std::shared_ptr<JsonBase> json_list = test_class->deserialize( "[ true , false ]" );
            std::shared_ptr<JsonBase> json_list_tmp = test_class->deserialize( "[ null , 1 ]" );
            std::vector<std::shared_ptr<JsonBase>> vc1;
            vc1.reserve(500);
            for(int i = 1;i<=500;++i){
                vc1.push_back(test_class->deserialize( "[ " + std::to_string(i) + " ]" ));
            }

            const auto begin = std::chrono::steady_clock::now();

            for(int i = 1;i<=500;++i){
                test_class->add_child_to_array(json_list, i , vc1[i-1] );
            }
            test_class->add_child_to_array(json_list_tmp, 1 ,json_map);


            test_class->add_child_to_object(json_map, "self_map1" ,json_list_tmp);
            test_class->add_child_to_object(json_map, "self_map2" ,json_list_tmp);
            for(int i = 1000; i<=1299 ;++i){
                test_class->add_child_to_object(json_map, std::to_string(i), json_list);
            }
            test_class->add_child_to_object(json_map, "self_map3" ,json_list_tmp);

            const auto end = std::chrono::steady_clock::now();
            const auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
            test_score.add_child = micros.count();
            std::cout << '\r' << lib_name << " >> crud-add testing passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;

        } catch (const NotSupportException&) {
            test_score.add_child = -1;
            ofs << " crud-add test not support." << std::endl;
            ofs << " file: json_test_files/crud_file.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> crud-add not support." << std::string(20, ' ') << std::endl;
        }catch (...) {
            test_score.add_child = -2;
            ofs << " crud-add test not support." << std::endl;
            ofs << " file: json_test_files/crud_file.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> crud-add not support." << std::string(20, ' ') << std::endl;
        }


        // delete test
        try {
            std::cout << lib_name << " >> delete child speed testing......"  << std::flush;

            std::array<std::shared_ptr<JsonBase>,10> json_ptrs;
            std::array<std::shared_ptr<JsonBase>,10> json_maps;
            std::array<std::shared_ptr<JsonBase>,10> json_lists;

            for(int i = 0;i<10;++i) {
                json_ptrs[i] = test_class->deserialize(big_varies);
                json_maps[i] = test_class->deserialize(big_object);
                json_lists[i] = test_class->deserialize(big_array);
            }
            auto begin = std::chrono::steady_clock::now();

            for (int cnt = 0;cnt<10;++cnt) {
                test_class->delete_child_from_object(json_ptrs[cnt], "events");
                test_class->delete_child_from_object(json_ptrs[cnt], "blockNames");

                for(int i=1;i<=1999;++i){
                    test_class->delete_child_from_object(json_maps[cnt], std::to_string(i));
                }
                for(int i=3999;i>=2001;--i){
                    test_class->delete_child_from_object(json_maps[cnt], std::to_string(i));
                }
                for(int i=3990;i>=1900;--i){
                    test_class->delete_child_from_array(json_lists[cnt], i);
                }
                for(int i=600;i<=800;++i){
                    test_class->delete_child_from_array(json_lists[cnt], i);
                }
            }

            const auto end = std::chrono::steady_clock::now();
            const auto micros = std::chrono::duration_cast<std::chrono::microseconds>(end-begin);
            test_score.delete_child = micros.count();
            std::cout << '\r' << lib_name << " >> crud-delete testing passed: " << micros.count()/1000ll << "." << micros.count()%1000ll  << " ms" << std::string(10, ' ')<< std::endl;

        } catch (const NotSupportException&) {
            test_score.delete_child = -1;
            ofs << " crud-delete test not support." << std::endl;
            ofs << " file: json_test_files/crud_file.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> crud-delete not support." << std::string(20, ' ') << std::endl;
        }catch (...) {
            test_score.delete_child = -2;
            ofs << " crud-delete test not support." << std::endl;
            ofs << " file: json_test_files/crud_file.json " << std::endl;
            ofs << "CPP TEST CODE FILE: " << __FILE__ << std::endl;
            ofs << "CPP TEST CODE LINE: " << std::to_string(__LINE__) << std::endl;
            ofs << "\n-----------------------------------------\n" << std::endl;
            std::cerr << '\r' << lib_name << " >> crud-delete not support." << std::string(20, ' ') << std::endl;
        }


    }

}
