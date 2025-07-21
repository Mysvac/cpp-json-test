
import std;
import jtu;
import test_tools;

import validate;
import speed;

int main() {
    std::println("------------------Prepare to start testing------------------");
    std::map<std::string, std::shared_ptr<jtu::TestBase>>& dict = jtu::TestClassFactory::instance().m_dict;
    // 统计分数的map
    std::map<std::string, jtu::TestScore> ScoreMap;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    /////// 文件资源读取
    std::string big_normal;
    std::string big_double;
    std::string big_nesting;
    std::string big_varies;
    std::string big_test;
    std::string big_object;
    std::string big_array;
    std::string path = PROJECT_RESULT_PATH "/reports";
    std::filesystem::create_directories(path);
    std::cout << "Reading the test JSON files..." << std::endl;
    try{
        big_normal  = jtu::read_file( PROJECT_JSONFILE_PATH "/big_normal.json");
        big_double  = jtu::read_file( PROJECT_JSONFILE_PATH "/big_double.json");
        big_nesting = jtu::read_file( PROJECT_JSONFILE_PATH "/big_nesting.json");
        big_varies  = jtu::read_file( PROJECT_JSONFILE_PATH "/big_varies.json");
        big_test    = jtu::read_file( PROJECT_JSONFILE_PATH "/big_test.json");
        big_object  = jtu::read_file( PROJECT_JSONFILE_PATH "/big_object.json");
        big_array   = jtu::read_file( PROJECT_JSONFILE_PATH "/big_array.json");
    } catch(...) {
        std::println(std::cerr, "The JSON file used for testing failed to be read. Please check the working directory.\n");
        return 0;
    }
    std::cout << "Reading success." << std::endl;
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    std::println("\n------------------Begin testing------------------\n");

    for(auto& [ lib_name, test_class ] : dict) {
        jtu::TestScore test_score;
        std::ofstream ofs{ std::string{ PROJECT_RESULT_PATH } + "/reports/" + lib_name + "_error_report.txt" };
        ofs << "[ test lib: " << lib_name << " ]\n" << std::endl;

        // 基础有效性测试
        jtu::validate(lib_name, test_class, test_score, ofs, big_varies);
        jtu::speed(lib_name, test_class, test_score, ofs, big_normal, big_double);


    }
}

