#include <gtest/gtest.h>
#include "../src/solution.h"
#include <algorithm>

// --- MedianFinder ---

TEST(MedianFinder, OddCount) {
    MedianFinder mf;
    mf.add_num(1);
    mf.add_num(2);
    mf.add_num(3);
    EXPECT_DOUBLE_EQ(mf.find_median(), 2.0);
}

TEST(MedianFinder, EvenCount) {
    MedianFinder mf;
    mf.add_num(1);
    mf.add_num(2);
    EXPECT_DOUBLE_EQ(mf.find_median(), 1.5);
}

TEST(MedianFinder, Streaming) {
    MedianFinder mf;
    mf.add_num(5);
    EXPECT_DOUBLE_EQ(mf.find_median(), 5.0);
    mf.add_num(3);
    EXPECT_DOUBLE_EQ(mf.find_median(), 4.0);
    mf.add_num(8);
    EXPECT_DOUBLE_EQ(mf.find_median(), 5.0);
    mf.add_num(4);
    EXPECT_DOUBLE_EQ(mf.find_median(), 4.5);
}

TEST(MedianFinder, Duplicates) {
    MedianFinder mf;
    mf.add_num(2);
    mf.add_num(2);
    mf.add_num(2);
    EXPECT_DOUBLE_EQ(mf.find_median(), 2.0);
}

// --- top_k_frequent ---

TEST(TopKFrequent, BasicCase) {
    // [1,1,1,2,2,3], k=2 => {1, 2} in any order
    auto result = top_k_frequent({1,1,1,2,2,3}, 2);
    std::sort(result.begin(), result.end());
    ASSERT_EQ(static_cast<int>(result.size()), 2);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[1], 2);
}

TEST(TopKFrequent, KEqualsOne) {
    auto result = top_k_frequent({4,4,4,3,3,2}, 1);
    ASSERT_EQ(static_cast<int>(result.size()), 1);
    EXPECT_EQ(result[0], 4);
}

TEST(TopKFrequent, SingleElement) {
    auto result = top_k_frequent({7}, 1);
    ASSERT_EQ(static_cast<int>(result.size()), 1);
    EXPECT_EQ(result[0], 7);
}

// --- task_scheduler ---

TEST(TaskScheduler, BasicCase) {
    // tasks = [A,A,A,B,B,B], cooldown = 2  => 8
    // A B _ A B _ A B
    std::vector<char> tasks = {'A','A','A','B','B','B'};
    EXPECT_EQ(task_scheduler(tasks, 2), 8);
}

TEST(TaskScheduler, NoCooldown) {
    // cooldown = 0: just run all tasks back-to-back
    std::vector<char> tasks = {'A','A','B','B'};
    EXPECT_EQ(task_scheduler(tasks, 0), 4);
}

TEST(TaskScheduler, AllSameTask) {
    // [A,A,A], cooldown = 3  => (3-1)*(3+1) + 1 = 9
    std::vector<char> tasks = {'A','A','A'};
    EXPECT_EQ(task_scheduler(tasks, 3), 9);
}

TEST(TaskScheduler, LargeEnoughVariety) {
    // Many distinct tasks: scheduler never idles
    // [A,A,B,C,D,E], cooldown=2: tasks.size()=6 >= formula => 6
    std::vector<char> tasks = {'A','A','B','C','D','E'};
    EXPECT_EQ(task_scheduler(tasks, 2), 6);
}
