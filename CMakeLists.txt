cmake_minimum_required(VERSION 3.24)

set(json_test_VERSION 3.0.0)

project(cpp-json-test
    VERSION ${json_test_VERSION}
    LANGUAGES CXX 
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(cpp-jsonlib CONFIG REQUIRED)



FILE(GLOB_RECURSE SRC_FILES LIST_DIRECTORIES FALSE src/* test_codes/*)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)



target_link_libraries(${PROJECT_NAME} PRIVATE jsonlib::jsonlib)



add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_CURRENT_SOURCE_DIR}/json_test_files
${CMAKE_CURRENT_BINARY_DIR}/Debug/json_test_files
COMMENT "Copying json_test_files directory..."
VERBATIM
)

