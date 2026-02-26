cmake_minimum_required(VERSION 3.20)
project(lab LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0)
FetchContent_MakeAvailable(googletest)

add_executable(tests test/test.cpp src/solution.cpp)
target_link_libraries(tests GTest::gtest_main)
enable_testing()
include(GoogleTest)
gtest_discover_tests(tests)
