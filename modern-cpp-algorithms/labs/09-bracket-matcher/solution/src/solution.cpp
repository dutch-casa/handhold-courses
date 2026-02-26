#include "solution.h"
#include <stack>

static char matching_open(char close) {
    switch (close) {
        case ')': return '(';
        case ']': return '[';
        case '}': return '{';
        default: return '\0';
    }
}

static bool is_open(char c) {
    return c == '(' || c == '[' || c == '{';
}

static bool is_close(char c) {
    return c == ')' || c == ']' || c == '}';
}

bool is_balanced(std::string_view s) {
    std::stack<char> st;
    for (char c : s) {
        if (is_open(c)) {
            st.push(c);
            continue;
        }
        if (!is_close(c)) {
            continue;
        }
        if (st.empty() || st.top() != matching_open(c)) {
            return false;
        }
        st.pop();
    }
    return st.empty();
}

int first_unbalanced(std::string_view s) {
    // Stack stores (bracket_char, index) pairs
    std::stack<std::pair<char, int>> st;
    for (int i = 0; i < static_cast<int>(s.size()); ++i) {
        char c = s[static_cast<size_t>(i)];
        if (is_open(c)) {
            st.push({c, i});
            continue;
        }
        if (!is_close(c)) {
            continue;
        }
        if (st.empty() || st.top().first != matching_open(c)) {
            return i;
        }
        st.pop();
    }
    if (!st.empty()) {
        // Find the earliest unmatched opener (bottom of stack)
        std::stack<std::pair<char, int>> reversed;
        while (!st.empty()) {
            reversed.push(st.top());
            st.pop();
        }
        return reversed.top().second;
    }
    return -1;
}
