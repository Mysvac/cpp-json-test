cmake_minimum_required(VERSION 4.0.0)

project(cpp-json-test
    VERSION 0.1.0                                     # Semantic versioning (MAJOR.MINOR.PATCH)
    HOMEPAGE_URL "https://github.com/Mysvac/cpp-json-test"
    DESCRIPTION "A C++ json lib testing framework"
    LANGUAGES CXX                                     # C++ only project
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_MODULE_STD ON)

###################################################
# 各种JSON库，不需要的可以删除
# find_package(your_package_name CONFIG REQUIRED)
find_package(mysvac-jsonlib CONFIG REQUIRED)
find_package(RapidJSON CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
# find_package(Qt6 REQUIRED COMPONENTS Core)
# find_package(Boost REQUIRED COMPONENTS json)

###################################################
add_subdirectory(3rdparty/vct-tools-json-0.7.0)
# cpp files
file(GLOB_RECURSE cxx_module_files ${CMAKE_CURRENT_SOURCE_DIR}/modules/*.ixx)
file(GLOB_RECURSE cxx_header_files ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
file(GLOB_RECURSE cxx_source_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(main ${cxx_source_files})
target_sources(main PUBLIC
        FILE_SET cxx_modules                          # Define a file set for modules
        TYPE CXX_MODULES                              # Specify this is a C++ modules file set
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/modules # Base directory for module resolution
        FILES ${cxx_module_files}                     # List of module files to include
)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include) # Include directory for headers

target_link_libraries(main PRIVATE vct::tools-json) # Link the vct-tools-json library

target_compile_definitions(main PRIVATE PROJECT_RESULT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/result")
target_compile_definitions(main PRIVATE PROJECT_JSONFILE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/json_files")

###################################################
# 不需要的可以删除
# target_link_libraries(main PRIVATE your_lib_name)
target_link_libraries(main PRIVATE mysvac::jsonlib)
target_link_libraries(main PRIVATE rapidjson)
target_link_libraries(main PRIVATE JsonCpp::JsonCpp)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)
# target_link_libraries(main PRIVATE Qt6::Core)
# target_link_libraries(main PRIVATE Boost::json)

###################################################


