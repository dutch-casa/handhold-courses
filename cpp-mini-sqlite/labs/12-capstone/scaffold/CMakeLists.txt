cmake_minimum_required(VERSION 3.20)
project(lab LANGUAGES CXX)
enable_testing()
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0)
FetchContent_MakeAvailable(googletest)

# Test suite
add_executable(tests
  test/test.cpp
  src/solution.cpp
  src/helpers.cpp)
target_include_directories(tests PRIVATE src)
target_link_libraries(tests GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(tests)

# Interactive REPL binary
add_executable(mini_sqlite
  src/main.cpp
  src/solution.cpp
  src/helpers.cpp)
target_include_directories(mini_sqlite PRIVATE src)
set_target_properties(mini_sqlite PROPERTIES OUTPUT_NAME "mini-sqlite")
