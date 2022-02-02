cmake_minimum_required(VERSION 3.16)
project(List)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.10.0
)
FetchContent_MakeAvailable(googletest)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-fprofile-instr-generate -fcoverage-mapping -std=c++17")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using GCC
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    # using Intel C++
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using Visual Studio C++
endif()

set(TEST_NAME ${PROJECT_NAME}_tests)
add_executable(${TEST_NAME} list.h tests.cpp main.cpp)
target_link_libraries(${TEST_NAME} gtest_main pthread)

enable_testing()
include(GoogleTest)
gtest_discover_tests(${TEST_NAME})

set_target_properties(
        ${TEST_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)