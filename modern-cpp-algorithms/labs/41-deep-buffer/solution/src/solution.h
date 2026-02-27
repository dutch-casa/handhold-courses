#pragma once
#include <string>

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
    std::string left_;
    std::string right_;
};
