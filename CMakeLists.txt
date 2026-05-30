cmake_minimum_required(VERSION 3.14)
project(BTreeLab LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

add_library(BTree_lib STATIC
        src/BTree.c
        )
target_include_directories(BTree_lib PUBLIC src)
add_executable(btree main.c)
target_link_libraries(btree BTree_lib)

option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/releases/download/v1.17.0/googletest-1.17.0.tar.gz
    )
    FetchContent_MakeAvailable(googletest)

    add_executable(btree_tests tests/tests.cpp)
    target_link_libraries(btree_tests BTree_lib gtest_main)

    enable_testing()
    add_test(NAME BTreeTests COMMAND btree_tests)
endif()