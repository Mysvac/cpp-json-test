
import std;
import jtu;
import test_tools;

import validate;
import speed;
import crud;

int main() {
    std::println("------------------Prepare to start testing------------------");
    std::map<std::string, std::shared_ptr<jtu::TestBase>>& dict = jtu::TestClassFactory::instance().m_dict;
    // 统计分数的map
    std::map<std::string, jtu::TestScore> ScoreMap;

    ////////////////////////////////////////////////////////////////////////////////////////////////
    /////// 文件资源读取

    std::string path = PROJECT_RESULT_PATH "/reports";
    std::filesystem::create_directories(path);

    std::string big_normal;
    std::string big_double;
    std::string big_varies;
    std::string big_string;
    std::string crud_file;
    std::string big_object;
    std::string big_array;


    std::cout << "Reading the test JSON files..." << std::endl;
    try{
        big_normal  = jtu::read_file( PROJECT_JSONFILE_PATH "/big_normal.json");
        big_double  = jtu::read_file( PROJECT_JSONFILE_PATH "/big_double.json");
        big_varies  = jtu::read_file( PROJECT_JSONFILE_PATH "/big_varies.json");
        big_string  = jtu::read_file( PROJECT_JSONFILE_PATH "/big_string.json");
        crud_file   = jtu::read_file( PROJECT_JSONFILE_PATH "/crud_file.json");
        big_object  = jtu::read_file( PROJECT_JSONFILE_PATH "/big_object.json");
        big_array   = jtu::read_file( PROJECT_JSONFILE_PATH "/big_array.json");
    } catch(...) {
        std::println(std::cerr, "The JSON file used for testing failed to be read. Please check the working directory.\n");
        return 0;
    }
    std::cout << "Reading success." << std::endl;

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    std::println("\n------------------Begin testing------------------\n");

    std::map<std::string, jtu::TestScore> dict_score;
    std::map<std::string, jtu::TestScore> dict_max;
    std::map<std::string, jtu::TestScore> dict_min;

    // 重读测试 TEXT_COUNT 轮次，计算均值
    constexpr long long TEXT_COUNT = 9;
    static_assert(TEXT_COUNT >= 3, "TEXT_COUNT must be greater than or equal to 5 ");
    for (int i : std::views::iota(0, static_cast<int>(TEXT_COUNT))) {
        std::println("\n--------------  testing counter {}/{}  ----------------\n", i+1, TEXT_COUNT);

        for(auto& [ lib_name, test_class ] : dict) {
            if (i==0) {
                dict_max[lib_name] = jtu::TestScore{};
                dict_min[lib_name] = jtu::TestScore::get_max();
            }

            jtu::TestScore test_score;
            std::ofstream ofs{ std::string{ PROJECT_RESULT_PATH } + "/reports/" + lib_name + "_error_report.txt" };
            ofs << "[ test lib: " << lib_name << " ]\n" << std::endl;

            // 基础有效性测试
            if (i==0) {
                jtu::validate(lib_name, test_class, test_score, ofs, big_varies, true);
            } else {
                jtu::validate(lib_name, test_class, test_score, ofs, big_varies, false);
            }

            jtu::speed(lib_name, test_class, test_score, ofs, big_normal, big_double, big_string);
            jtu::crud(lib_name, test_class, test_score, ofs, crud_file, big_varies, big_object, big_array);

            if (i==0) {
                dict_score[lib_name] = test_score;
            } else {
                dict_score[lib_name] += test_score;
            }
            dict_max[lib_name].maxed(test_score);
            dict_min[lib_name].mined(test_score);
        }
        std::println("\n------------------Tests complete------------------\n");
    }

    for (auto& [ lib_name, test_class ] : dict) {
        dict_score[lib_name] -= dict_min[lib_name];
        dict_score[lib_name] -= dict_max[lib_name];
        dict_score[lib_name] /= (TEXT_COUNT - 2);
    }

    for (auto& val : dict_score | std::views::values) {

        val /= TEXT_COUNT;
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    // test_tools.ixx
    jtu::generate_csv(dict_score, std::string{ PROJECT_RESULT_PATH } + "/result.csv" );

    std::println("\n----------------------end----------------------\n");
}

