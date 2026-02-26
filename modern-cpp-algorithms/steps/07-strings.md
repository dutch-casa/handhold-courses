---
title: Strings, string_view, and I/O
---

# std::string

{{show: string-code typewriter 1.5s linear}} std string is a dynamic, heap-allocated character array. It manages its own memory.

{{focus: create}} {{zoom: 1.2x}} {{annotate: create "Owning string"}} Construction copies the characters into heap-allocated storage. The string owns its data.

{{zoom: 1x}} {{focus: concat}} {{zoom: 1.2x}} {{annotate: concat "Operator +"}} Concatenation with plus creates a new string. It allocates, copies both sides, and returns the result.

{{zoom: 1x}} {{focus: methods}} {{zoom: 1.2x}} {{annotate: methods "Rich API"}} Strings have methods for everything — length, substring, find, replace, compare. They're not just character arrays.

{{zoom: 1x}} {{focus: iterate}} {{zoom: 1.2x}} {{annotate: iterate "Range-for"}} Range-for loops iterate over each character. No index juggling.

{{zoom: 1x}} {{focus: none}} std string handles allocation and deallocation. You never call new or delete for strings.

```code:string-code lang=cpp
#include <string>

std::string greeting = "Hello";
std::string name = "Alice";
std::string msg = greeting + ", " + name + "!";   // "Hello, Alice!"

auto len = msg.length();               // 13
auto pos = msg.find("Alice");          // 7
auto sub = msg.substr(0, 5);           // "Hello"
bool has = msg.contains("Alice");      // true (C++23)

for (char c : msg) {
    // process each character
}
---
create: 3-4
concat: 5
methods: 7-10
iterate: 12-14
```

# Small string optimization

{{clear: slide}}

{{show: sso-viz grow 0.5s spring}} Short strings don't allocate on the heap. They live inside the string object itself.

{{focus: inline-str}} {{annotate: inline-str "Inline buffer"}} Strings under about 22 characters fit in the object's internal buffer. No heap allocation. No pointer indirection. The data sits right there in the stack frame.

{{focus: heap-str}} {{annotate: heap-str "Heap allocated"}} Longer strings spill to the heap. The object stores a pointer to heap memory plus the length and capacity.

{{focus: none}} This is SSO — small string optimization. It makes short strings fast. You don't control it. You don't need to. Just know it exists.

```data:sso-viz type=array
["short: [H|e|l|l|o|\\0|_|_|_] inline", "long: [ptr → heap] ─→ [H|e|l|l|o|,| |w|o|r|l|d|!|\\0]"]
---
inline-str: 0
heap-str: 1
```

# string_view

{{clear: slide}}

{{show: sv-code typewriter 1.5s linear}} string view is a non-owning, read-only view into a string. It doesn't allocate. It doesn't copy. It just points.

{{focus: from-string}} {{zoom: 1.2x}} {{annotate: from-string "Zero-copy view"}} Creating a string view from a string copies nothing. It stores a pointer and a length. Two words.

{{zoom: 1x}} {{focus: from-literal}} {{zoom: 1.2x}} {{annotate: from-literal "From literal"}} It works with string literals too. No allocation. The view points directly into the read-only data segment.

{{zoom: 1x}} {{focus: substr-sv}} {{zoom: 1.3x}} {{annotate: substr-sv "Free substring"}} string view's substr doesn't allocate either. It adjusts the pointer and length. Substring in constant time.

{{zoom: 1x}} {{focus: param}} {{zoom: 1.2x}} {{annotate: param "Ideal parameter"}} Use string view for function parameters when you only read the string. It accepts both std string and string literals without copying.

{{zoom: 1x}} {{focus: danger}} {{zoom: 1.3x}} {{annotate: danger "Lifetime risk"}} The danger: string view doesn't own the data. If the underlying string is destroyed, the view dangles. Don't store string views that outlive their source.

{{zoom: 1x}} {{focus: none}} string view is the const reference of the string world. Zero-copy reads. Don't store it.

```code:sv-code lang=cpp
#include <string>
#include <string_view>

std::string msg = "Hello, world!";
std::string_view sv = msg;              // no copy, just a view

std::string_view lit = "Hello";         // points to string literal

auto sub = sv.substr(0, 5);            // "Hello" — no allocation

void print(std::string_view s) {       // accepts string and literal
    std::cout << s << "\n";
}
print(msg);                            // works
print("Hi");                           // works, no conversion

// DANGER: don't outlive the source
// std::string_view bad = get_temp_string();  // dangling!
---
from-string: 5
from-literal: 7
substr-sv: 9
param: 11-15
danger: 18
```

# std::format and std::print

{{clear: slide}}

{{show: format-code typewriter 1.5s linear}} C++ 20 introduced format. C++ 23 added print. Both replace the awkward iostream insertion operators.

{{focus: format-basic}} {{zoom: 1.2x}} {{annotate: format-basic "Python-style"}} std format uses curly braces as placeholders. The arguments fill them in order. Familiar if you know Python's f-strings.

{{zoom: 1x}} {{focus: format-spec}} {{zoom: 1.2x}} {{annotate: format-spec "Format specs"}} Colon inside the braces adds formatting. Dot 2 f means two decimal places. 0 greater-than 10 means zero-padded to width 10.

{{zoom: 1x}} {{focus: print-fn}} {{zoom: 1.3x}} {{annotate: print-fn "Direct output"}} std print combines format and output in one call. No endl. No double less-than. Just the format string and the arguments.

{{zoom: 1x}} {{focus: println-fn}} {{annotate: println-fn "With newline"}} std println adds the newline automatically.

{{zoom: 1x}} {{focus: none}} Use std format for building strings. Use std print and println for output. Forget cout with insertion operators.

```code:format-code lang=cpp
#include <format>
#include <print>
#include <string>

auto msg = std::format("Hello, {}!", "world");       // "Hello, world!"
auto pi = std::format("pi = {:.2f}", 3.14159);       // "pi = 3.14"
auto hex = std::format("{:#x}", 255);                 // "0xff"
auto pad = std::format("{:0>10}", 42);                // "0000000042"

std::print("Name: {}, Age: {}", "Alice", 30);
std::println("Score: {:.1f}%", 95.6);
---
format-basic: 5
format-spec: 6-8
print-fn: 10
println-fn: 11
```

# File I/O with RAII

{{clear: slide}}

{{show: file-code typewriter 2s linear}} File streams follow RAII. Open in the constructor, close in the destructor.

{{focus: write-file}} {{zoom: 1.2x}} {{annotate: write-file "Write to file"}} ofstream opens a file for writing. The insertion operator writes text. When the stream goes out of scope, the file is closed and flushed automatically.

{{zoom: 1x}} {{focus: read-file}} {{zoom: 1.2x}} {{annotate: read-file "Read from file"}} ifstream opens a file for reading. getline reads one line at a time into a string.

{{zoom: 1x}} {{focus: check-open}} {{zoom: 1.3x}} {{annotate: check-open "Always check"}} Always check is open before using the stream. The file might not exist or might be locked.

{{zoom: 1x}} {{focus: none}} RAII means no manual close calls. The destructor handles it. If an exception fires between open and use, the file still closes.

```code:file-code lang=cpp
#include <fstream>
#include <string>

// Writing
{
    std::ofstream out("data.txt");
    if (!out.is_open()) { /* handle error */ }
    out << "line one\n";
    out << "line two\n";
}  // file closed automatically

// Reading
{
    std::ifstream in("data.txt");
    if (!in.is_open()) { /* handle error */ }
    std::string line;
    while (std::getline(in, line)) {
        // process each line
    }
}  // file closed automatically
---
write-file: 5-10
read-file: 13-19
check-open: 7, 15
```

# Stream parsing

{{clear: slide}}

{{show: parse-code typewriter 1.5s linear}} Streams can parse structured data. The extraction operator reads tokens separated by whitespace.

{{focus: sstream}} {{zoom: 1.2x}} {{annotate: sstream "String stream"}} istringstream wraps a string as an input stream. You read from it like a file.

{{zoom: 1x}} {{focus: extract}} {{zoom: 1.2x}} {{annotate: extract "Type-safe parsing"}} The extraction operator parses the next token into the variable's type. Int reads an integer. Double reads a float. String reads one word.

{{zoom: 1x}} {{focus: loop-parse}} {{zoom: 1.2x}} {{annotate: loop-parse "Token loop"}} Loop until extraction fails. The stream converts to false when it hits end-of-input or a parse error.

{{zoom: 1x}} {{focus: none}} String streams are the C++ way to parse structured text. No regex for simple tokenization. No manual substring slicing.

```code:parse-code lang=cpp
#include <sstream>
#include <string>
#include <vector>

std::istringstream input("42 3.14 hello");

int n;
double d;
std::string word;
input >> n >> d >> word;  // n=42, d=3.14, word="hello"

// Parse all words
std::istringstream words("apple banana cherry");
std::vector<std::string> tokens;
std::string token;
while (words >> token) {
    tokens.push_back(token);
}
// tokens = {"apple", "banana", "cherry"}
---
sstream: 5
extract: 10
loop-parse: 14-18
```
