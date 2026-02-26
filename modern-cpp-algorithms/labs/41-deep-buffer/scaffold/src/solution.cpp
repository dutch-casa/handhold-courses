#include "solution.h"

TextBuffer::TextBuffer() {
    // TODO: initialize buffer state
}

void TextBuffer::insert(char ch) {
    // TODO: insert ch at cursor; cursor advances
}

void TextBuffer::insert_string(const std::string& text) {
    // TODO: insert all characters of text at cursor
}

void TextBuffer::erase() {
    // TODO: delete character before cursor (backspace); no-op at pos 0
}

void TextBuffer::move_left() {
    // TODO: move cursor one position left
}

void TextBuffer::move_right() {
    // TODO: move cursor one position right
}

void TextBuffer::move_to(int pos) {
    // TODO: move cursor to absolute position (clamp to valid range)
}

std::string TextBuffer::text() const {
    // TODO: return full text
    return "";
}

int TextBuffer::cursor_pos() const {
    // TODO: return cursor position
    return 0;
}
