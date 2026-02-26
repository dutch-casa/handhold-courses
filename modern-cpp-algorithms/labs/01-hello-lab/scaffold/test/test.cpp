#include <gtest/gtest.h>
#include "../src/solution.h"

TEST(HelloLab, GreetReturnsCorrectString) {
    EXPECT_EQ(greet(), "Hello, Handhold!");
}

TEST(HelloLab, GreetNameWithAlice) {
    EXPECT_EQ(greet_name("Alice"), "Hello, Alice!");
}

TEST(HelloLab, GreetNameWithEmptyString) {
    EXPECT_EQ(greet_name(""), "Hello, !");
}

TEST(HelloLab, GreetNameWithSpaces) {
    EXPECT_EQ(greet_name("Bob Smith"), "Hello, Bob Smith!");
}
