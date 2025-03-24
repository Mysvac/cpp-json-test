cmake_minimum_required(VERSION 3.24)

set(json_test_VERSION 3.0.0)

project(cpp-json-test
    VERSION ${json_test_VERSION}
    LANGUAGES CXX 
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "/O2")

###################################################
# 各种JSON库，不需要的可以删除
# find_package(your_package_name CONFIG REQUIRED)
find_package(cpp-jsonlib CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Boost REQUIRED COMPONENTS json)

###################################################

# cpp files
FILE(GLOB SRC_FILES LIST_DIRECTORIES FALSE src/*.cpp test_codes/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_RESULT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/result")
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_JSONFILE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/json_test_files")
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_USE_BOOST_PROCESS="ON")

###################################################
# 是否通过boost-process库进行内存测试
# 如果不需要内存占用测试，或不想下载大量的boost-process库依赖，请 删除/注释 掉下面的内容
# 通过宏控制，宏不存在时不会编译相关代码。 注释后重写配置和编译即可
find_package(Boost REQUIRED COMPONENTS process)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::process)
target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_USE_BOOST_PROCESS="ON")

###################################################

###################################################
# 不需要的可以删除
# target_link_libraries(${PROJECT_NAME} PRIVATE your_lib_name)
target_link_libraries(${PROJECT_NAME} PRIVATE jsonlib::jsonlib)
target_link_libraries(${PROJECT_NAME} PRIVATE rapidjson)
target_link_libraries(${PROJECT_NAME} PRIVATE JsonCpp::JsonCpp)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::json)

###################################################


