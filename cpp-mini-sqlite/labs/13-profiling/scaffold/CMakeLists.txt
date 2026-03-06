cmake_minimum_required(VERSION 3.20)
project(lab13_profiling LANGUAGES CXX)
enable_testing()
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/v1.14.0.tar.gz
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(tests test/test.cpp src/solution.cpp)
target_include_directories(tests PRIVATE src)
target_link_libraries(tests PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(tests)
