#pragma once
#include <string>

// TextBuffer: a cursor-aware text buffer.
// Rule of Zero: no manual memory management — use std::string internally.
class TextBuffer {
public:
    TextBuffer();

    void insert(char ch);
    void insert_string(const std::string& text);
    void erase();

    void move_left();
    void move_right();
    void move_to(int pos);

    std::string text() const;
    int cursor_pos() const;

private:
    // TODO: add private members
};
