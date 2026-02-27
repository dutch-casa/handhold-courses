#include <gtest/gtest.h>
#include "../src/solution.h"

TEST(TextBuffer, EmptyBuffer) {
    TextBuffer buf;
    EXPECT_EQ(buf.text(), "");
    EXPECT_EQ(buf.cursor_pos(), 0);
}

TEST(TextBuffer, InsertSingle) {
    TextBuffer buf;
    buf.insert('a');
    EXPECT_EQ(buf.text(), "a");
    EXPECT_EQ(buf.cursor_pos(), 1);
}

TEST(TextBuffer, InsertString) {
    TextBuffer buf;
    buf.insert_string("hello");
    EXPECT_EQ(buf.text(), "hello");
    EXPECT_EQ(buf.cursor_pos(), 5);
}

TEST(TextBuffer, EraseAtEnd) {
    TextBuffer buf;
    buf.insert_string("abc");
    buf.erase();
    EXPECT_EQ(buf.text(), "ab");
    EXPECT_EQ(buf.cursor_pos(), 2);
}

TEST(TextBuffer, EraseAtZero) {
    TextBuffer buf;
    buf.erase();
    EXPECT_EQ(buf.text(), "");
    EXPECT_EQ(buf.cursor_pos(), 0);
}

TEST(TextBuffer, MoveLeftRight) {
    TextBuffer buf;
    buf.insert_string("abc");
    buf.move_left();
    EXPECT_EQ(buf.cursor_pos(), 2);
    buf.move_right();
    EXPECT_EQ(buf.cursor_pos(), 3);
}

TEST(TextBuffer, MoveLeftAtZero) {
    TextBuffer buf;
    buf.move_left();
    EXPECT_EQ(buf.cursor_pos(), 0);
}

TEST(TextBuffer, InsertAtMiddle) {
    TextBuffer buf;
    buf.insert_string("ac");
    buf.move_left();
    buf.insert('b');
    EXPECT_EQ(buf.text(), "abc");
    EXPECT_EQ(buf.cursor_pos(), 2);
}

TEST(TextBuffer, EraseAtMiddle) {
    TextBuffer buf;
    buf.insert_string("abc");
    buf.move_left();
    buf.erase();
    EXPECT_EQ(buf.text(), "ac");
    EXPECT_EQ(buf.cursor_pos(), 1);
}

TEST(TextBuffer, MoveTo) {
    TextBuffer buf;
    buf.insert_string("hello");
    buf.move_to(2);
    EXPECT_EQ(buf.cursor_pos(), 2);
    buf.insert('X');
    EXPECT_EQ(buf.text(), "heXllo");
}

TEST(TextBuffer, MoveToClamps) {
    TextBuffer buf;
    buf.insert_string("abc");
    buf.move_to(-5);
    EXPECT_EQ(buf.cursor_pos(), 0);
    buf.move_to(100);
    EXPECT_EQ(buf.cursor_pos(), 3);
}
