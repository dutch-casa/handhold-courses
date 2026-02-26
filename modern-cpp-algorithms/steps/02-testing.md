---
title: Testing from day one
---

# Why test first

{{show: red-green grow 0.5s spring}} You write a test before you write the code it tests. Sounds backward. It's not.

{{focus: red-phase}} {{annotate: red-phase "Write the test"}} You write a test that describes what you want. It fails. Of course it fails — the code doesn't exist yet.

{{focus: green-phase}} {{annotate: green-phase "Make it pass"}} Then you write just enough code to make the test pass. Nothing more.

{{focus: refactor-phase}} {{annotate: refactor-phase "Clean up"}} Then you clean up. The test stays green. You didn't break anything.

{{focus: none}} This loop is the heartbeat of professional development. Red, green, refactor. Every exercise in this course follows it.

```diagram:red-green
write-test [service]
make-pass [service]
clean-up [service]
write-test --fails--> make-pass
make-pass --passes--> clean-up
clean-up --next test--> write-test
{Red-Green-Refactor: write-test, make-pass, clean-up}
---
red-phase: write-test
green-phase: make-pass
refactor-phase: clean-up
```

# GoogleTest basics

{{clear: slide}}

{{show: gtest-basics typewriter 2s linear}} GoogleTest is the standard testing framework for C++. Here's a minimal test file.

{{focus: include-gtest}} {{zoom: 1.3x}} {{annotate: include-gtest "Testing framework"}} Include the gtest header. This gives you the test macros.

{{zoom: 1x}} {{focus: test-macro}} {{zoom: 1.2x}} {{annotate: test-macro "One test case"}} The TEST macro defines a test case. Two arguments: the suite name and the test name. Suite groups related tests. Name describes what you're checking.

{{zoom: 1x}} {{focus: expect-line}} {{zoom: 1.3x}} {{annotate: expect-line "The assertion"}} EXPECT EQ checks that two values are equal. If they're not, the test fails and tells you what it got versus what it expected. Clear feedback.

{{zoom: 1x}} {{focus: assert-line}} {{zoom: 1.2x}} {{annotate: assert-line "Hard stop"}} ASSERT TRUE is stricter. If it fails, the test stops immediately. Use ASSERT for conditions that make the rest of the test meaningless. Use EXPECT when you want to see all failures.

{{zoom: 1x}} {{focus: none}} Two macros handle most cases. EXPECT EQ for values, ASSERT TRUE for preconditions.

```code:gtest-basics lang=cpp
#include <gtest/gtest.h>
#include "math.h"

TEST(MathTest, AddPositiveNumbers) {
    EXPECT_EQ(add(2, 3), 5);
}

TEST(MathTest, AddNegativeNumbers) {
    EXPECT_EQ(add(-1, -2), -3);
}

TEST(MathTest, ResultIsPositive) {
    int result = add(2, 3);
    ASSERT_TRUE(result > 0);
    EXPECT_EQ(result, 5);
}
---
include-gtest: 1
test-macro: 4
expect-line: 5
assert-line: 14-16
```

# Writing your first test

{{clear: slide}}

{{show: test-first typewriter 1.5s linear}} You're about to implement an add function. But you write the test first.

{{focus: test-body}} {{zoom: 1.2x}} {{annotate: test-body "Specification"}} This test IS the specification. Two plus three equals five. Negative one plus negative two equals negative three. Zero plus anything equals that thing. The tests define exactly what add must do.

{{zoom: 1x}} {{focus: edge-case}} {{zoom: 1.2x}} {{annotate: edge-case "Boundary"}} Testing zero is a boundary check. Off-by-one errors hide at boundaries. Always test them.

{{zoom: 1x}} {{focus: none}} The function doesn't exist yet. These tests won't compile. That's the point. Red first.

```code:test-first lang=cpp
#include <gtest/gtest.h>
#include "math.h"

TEST(AddTest, PositiveNumbers) {
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(10, 20), 30);
}

TEST(AddTest, NegativeNumbers) {
    EXPECT_EQ(add(-1, -2), -3);
}

TEST(AddTest, Zero) {
    EXPECT_EQ(add(0, 5), 5);
    EXPECT_EQ(add(5, 0), 5);
    EXPECT_EQ(add(0, 0), 0);
}
---
test-body: 4-7
edge-case: 13-17
```

# Red, green

{{clear: slide}}

{{show: header-file typewriter 1s linear}} Now write just enough to make it compile and pass.

{{focus: declaration}} {{zoom: 1.3x}} {{annotate: declaration "The interface"}} The header declares the function. Two ints in, one int out. This is the contract.

{{zoom: 1x}} {{focus: none}} That's the header.

{{clear: slide}}

{{show: impl-file typewriter 1s linear}} The implementation is one line.

{{focus: impl-body}} {{zoom: 1.3x}} {{annotate: impl-body "The implementation"}} Return a plus b. That's it. No validation, no edge case handling, no cleverness. The tests pass. Done.

{{zoom: 1x}} {{focus: none}} The temptation is to add more. Resist. Write the next test first. Let the tests drive the design.

```code:header-file lang=cpp
#pragma once

int add(int a, int b);
---
declaration: 3
```

```code:impl-file lang=cpp
#include "math.h"

int add(int a, int b) {
    return a + b;
}
---
impl-body: 3-5
```

# Test organization

{{clear: slide}}

{{show: test-org typewriter 1.5s linear}} One test file per source file. Name it test underscore the thing.

{{focus: file-layout}} {{zoom: 1.2x}} {{annotate: file-layout "Mirror structure"}} The test directory mirrors the source directory. math dot cpp gets test underscore math dot cpp. When you open a source file, the test file is predictable.

{{zoom: 1x}} {{focus: naming}} {{zoom: 1.2x}} {{annotate: naming "Suite = class, test = behavior"}} Suite names match the component. Test names describe behavior, not implementation. "adds positive numbers" tells you what the function does. "calls internal helper" tells you how — which is fragile.

{{zoom: 1x}} {{focus: none}} Keep tests next to what they test. Keep names behavioral. A good test suite reads like documentation.

```code:test-org lang=text
project/
  src/
    math.h
    math.cpp
    string_utils.h
    string_utils.cpp
  test/
    test_math.cpp
    test_string_utils.cpp
  CMakeLists.txt
---
file-layout: 1-10
naming: 7-8
```

# CMake and GoogleTest

{{clear: slide}}

{{show: cmake-file typewriter 2s linear}} CMake is the build system. It fetches GoogleTest, compiles your code, and links the test binary.

{{focus: cmake-version}} {{zoom: 1.2x}} {{annotate: cmake-version "Minimum version"}} CMake version 3.20 or higher. This gives us FetchContent, which downloads dependencies automatically.

{{zoom: 1x}} {{focus: project-line}} {{annotate: project-line "Project name"}} Name the project. Set the language to C++.

{{zoom: 1x}} {{focus: std-line}} {{zoom: 1.2x}} {{annotate: std-line "C++23"}} Set the standard to C++ 23 globally. Every target in this project uses modern C++.

{{zoom: 1x}} {{focus: fetch-block}} {{zoom: 1.2x}} {{annotate: fetch-block "Auto-download"}} FetchContent declares GoogleTest as a dependency and downloads it during configuration. You don't install anything manually.

{{zoom: 1x}} {{focus: target-block}} {{zoom: 1.2x}} {{annotate: target-block "Build the test"}} Add an executable target that compiles both the test file and the source file. Link against gtest main, which provides the main function for you.

{{zoom: 1x}} {{focus: discover-line}} {{zoom: 1.2x}} {{annotate: discover-line "Auto-discovery"}} gtest discover tests scans the binary for TEST macros and registers them with ctest. You run ctest, it runs all your tests.

{{zoom: 1x}} {{focus: none}} To build and test: cmake dash B build, cmake dash dash build build, cd build, ctest. Four commands. That's your entire development loop.

```code:cmake-file lang=cmake
cmake_minimum_required(VERSION 3.20)
project(my_project LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0)
FetchContent_MakeAvailable(googletest)

add_executable(tests test/test_math.cpp src/math.cpp)
target_link_libraries(tests GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(tests)
---
cmake-version: 1
project-line: 2
std-line: 3
fetch-block: 5-9
target-block: 11-12
discover-line: 13-14
```
