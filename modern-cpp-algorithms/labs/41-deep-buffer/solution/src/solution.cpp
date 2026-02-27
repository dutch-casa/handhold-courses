#include "solution.h"
#include <algorithm>

TextBuffer::TextBuffer() = default;

void TextBuffer::insert(char ch) {
    left_ += ch;
}

void TextBuffer::insert_string(const std::string& text) {
    left_ += text;
}

void TextBuffer::erase() {
    if (!left_.empty()) {
        left_.pop_back();
    }
}

void TextBuffer::move_left() {
    if (!left_.empty()) {
        right_.insert(right_.begin(), left_.back());
        left_.pop_back();
    }
}

void TextBuffer::move_right() {
    if (!right_.empty()) {
        left_ += right_.front();
        right_.erase(right_.begin(), right_.begin() + 1);
    }
}

void TextBuffer::move_to(int pos) {
    int target = std::clamp(pos, 0, static_cast<int>(left_.size() + right_.size()));
    while (static_cast<int>(left_.size()) > target) move_left();
    while (static_cast<int>(left_.size()) < target) move_right();
}

std::string TextBuffer::text() const {
    return left_ + right_;
}

int TextBuffer::cursor_pos() const {
    return static_cast<int>(left_.size());
}
