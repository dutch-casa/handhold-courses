---
title: Classes and deep modules
---

# What makes a good interface

{{show: deep-shallow grow 0.5s spring}} A class is an interface over hidden complexity. The best classes have simple surfaces and significant depth. That's the design goal.

{{focus: deep}} {{annotate: deep "Deep module"}} A deep module: few public methods, each doing substantial work. The interface is small. The implementation is rich. Callers don't need to understand internals.

{{focus: shallow}} {{annotate: shallow "Shallow module"}} A shallow module: many small methods, each trivially thin. The interface is as complex as the implementation. Nothing is hidden. Using it requires understanding everything.

{{focus: none}} Every class you write is a choice on this spectrum. Aim deep. Hide the complexity. Expose only what callers need.

```data:deep-shallow type=array
["Deep: TextBuffer → insert, erase, text (3 methods, gap buffer inside)", "Shallow: GapBuffer → move_gap, expand_gap, left_size, right_start, ... (10 methods)"]
---
deep: 0
shallow: 1
```

# Classes in C++

{{clear: slide}}

{{show: class-code typewriter 2s linear}} A class bundles data and the operations on that data. Public for the interface. Private for the implementation.

{{focus: constructor}} {{zoom: 1.2x}} {{annotate: constructor "Initialize"}} The constructor initializes state. Use the member initializer list — it's more efficient than assignment in the body and required for const and reference members.

{{zoom: 1x}} {{focus: methods}} {{zoom: 1.2x}} {{annotate: methods "The interface"}} Public methods define what the class does. Keep them few and meaningful. Each method should do real work, not just expose a data field.

{{zoom: 1x}} {{focus: private-data}} {{zoom: 1.2x}} {{annotate: private-data "Hidden state"}} Private members are the implementation. Callers never see them. You can change the internals without breaking the interface.

{{zoom: 1x}} {{focus: none}}

```code:class-code lang=cpp
class Counter {
public:
    explicit Counter(int start = 0) : value_(start) {}

    void increment() { value_++; }
    void reset() { value_ = 0; }
    int value() const { return value_; }

private:
    int value_;
};
---
constructor: 3
methods: 5-7
private-data: 9-10
```

# Rule of Zero

{{clear: slide}}

{{show: zero-code typewriter 1.5s linear}} If your class doesn't manage a resource directly, don't write a destructor, copy constructor, move constructor, copy assignment, or move assignment. Let the compiler generate them.

{{focus: good}} {{zoom: 1.2x}} {{annotate: good "Rule of Zero"}} This class owns a vector and a string. Both manage their own memory. The compiler-generated destructor, copy, and move do the right thing automatically.

{{zoom: 1x}} {{focus: no-dtor}} {{zoom: 1.2x}} {{annotate: no-dtor "Nothing to write"}} No destructor. No copy constructor. No move constructor. No assignment operators. The compiler handles it all because the members handle themselves.

{{zoom: 1x}} {{focus: none}} Rule of Zero is the default. Smart pointers and standard containers are self-managing. Build on them and you never write resource management code.

```code:zero-code lang=cpp
class Document {
public:
    Document(std::string title) : title_(std::move(title)) {}

    void add_line(std::string line) { lines_.push_back(std::move(line)); }
    const std::string& title() const { return title_; }
    int line_count() const { return static_cast<int>(lines_.size()); }

private:
    std::string title_;
    std::vector<std::string> lines_;
    // No destructor, no copy/move — compiler generates them correctly
};
---
good: 1-12
no-dtor: 12
```

# Rule of Five

{{clear: slide}}

{{show: five-code typewriter 2s linear}} When you do manage a resource directly — a file handle, a raw allocation, a system resource — you need all five: destructor, copy constructor, copy assignment, move constructor, move assignment.

{{focus: dtor}} {{zoom: 1.2x}} {{annotate: dtor "Release resource"}} The destructor releases the resource. This is RAII — the resource's lifetime is tied to the object's lifetime.

{{zoom: 1x}} {{focus: copy}} {{zoom: 1.2x}} {{annotate: copy "Deep copy"}} Copy operations create independent copies of the resource. Copy constructor for initialization, copy assignment for reassignment.

{{zoom: 1x}} {{focus: move}} {{zoom: 1.2x}} {{annotate: move "Transfer ownership"}} Move operations transfer the resource without copying. The source is left in a valid but empty state.

{{zoom: 1x}} {{focus: none}} If you write any of the five, you likely need all five. But prefer Rule of Zero — wrap the resource in a smart pointer and let the compiler do the rest.

```code:five-code lang=cpp
class FileHandle {
public:
    explicit FileHandle(const char* path) : fp_(std::fopen(path, "r")) {}
    ~FileHandle() { if (fp_) std::fclose(fp_); }

    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;

    FileHandle(FileHandle&& other) noexcept : fp_(other.fp_) { other.fp_ = nullptr; }
    FileHandle& operator=(FileHandle&& other) noexcept {
        if (this != &other) { if (fp_) std::fclose(fp_); fp_ = other.fp_; other.fp_ = nullptr; }
        return *this;
    }

private:
    std::FILE* fp_;
};
---
dtor: 4
copy: 6-7
move: 9-13
```

# Encapsulation done right

{{clear: slide}}

{{show: encap-viz grow 0.5s spring}} Encapsulation isn't about hiding data fields behind getters and setters. That's busywork that hides nothing. Real encapsulation hides complexity behind meaningful operations.

{{focus: bad}} {{annotate: bad "Fake encapsulation"}} A class with get x, set x, get y, set y for every field isn't encapsulated. The interface mirrors the implementation. Changing one requires changing the other.

{{focus: good}} {{annotate: good "Real encapsulation"}} A class with move to, distance to, and rotate hides how coordinates are stored. Polar? Cartesian? The caller doesn't care. You can switch representations without breaking anything.

{{focus: none}} Ask: if I change the private members, does the public interface survive? If yes, you have encapsulation. If no, you have decoration.

```data:encap-viz type=array
["Bad: get_x(), set_x(), get_y(), set_y() — mirrors struct", "Good: move_to(), distance_to(), rotate() — hides representation"]
---
bad: 0
good: 1
```

# const member functions

{{clear: slide}}

{{show: const-code typewriter 1.5s linear}} Mark every method that doesn't modify state as const. It's not optional — it's how you communicate intent and enable const references.

{{focus: const-method}} {{zoom: 1.2x}} {{annotate: const-method "Read-only guarantee"}} A const member function promises not to modify the object. The compiler enforces it. Callers holding a const reference can only call const methods.

{{zoom: 1x}} {{focus: mutable-field}} {{zoom: 1.2x}} {{annotate: mutable-field "Cache exception"}} Mutable allows modification inside a const method. Use it for caching — the logical state doesn't change, but a cached result gets updated. It's the exception, not the rule.

{{zoom: 1x}} {{focus: none}} Default to const. Drop it only when the method genuinely modifies state.

```code:const-code lang=cpp
class Circle {
public:
    explicit Circle(double r) : radius_(r) {}

    double area() const { return 3.14159 * radius_ * radius_; }

    double cached_area() const {
        if (!cached_) { cache_ = area(); cached_ = true; }
        return cache_;
    }

private:
    double radius_;
    mutable double cache_ = 0;
    mutable bool cached_ = false;
};
---
const-method: 5
mutable-field: 7-9
```

# static members

{{clear: slide}}

{{show: static-code typewriter 1.5s linear}} Static members belong to the class, not to any instance. Use them for shared state and factory methods.

{{focus: static-field}} {{zoom: 1.2x}} {{annotate: static-field "Class-level state"}} A static data member is shared across all instances. Declare in the class, define outside (or use inline in C++17+).

{{zoom: 1x}} {{focus: factory}} {{zoom: 1.2x}} {{annotate: factory "Named constructor"}} Static methods can serve as factories — named constructors that return instances. They can have descriptive names and can return different types.

{{zoom: 1x}} {{focus: none}} Static is for class-level concerns. Instance methods for instance-level concerns. Don't mix them.

```code:static-code lang=cpp
class Logger {
public:
    static Logger& instance() {
        static Logger logger;
        return logger;
    }

    void log(const std::string& msg) {
        messages_.push_back(msg);
        count_++;
    }

    static int total_count() { return count_; }

private:
    Logger() = default;
    std::vector<std::string> messages_;
    inline static int count_ = 0;
};
---
static-field: 18
factory: 3-6
```

# Operator overloading

{{clear: slide}}

{{show: spaceship-code typewriter 2s linear}} C++20 introduced the spaceship operator. One declaration generates all six comparison operators.

{{focus: spaceship}} {{zoom: 1.2x}} {{annotate: spaceship "One operator"}} The spaceship operator, less than equals greater than, returns a comparison category. The compiler generates less than, greater than, less or equal, greater or equal, equal, and not equal from it.

{{zoom: 1x}} {{focus: default-impl}} {{zoom: 1.2x}} {{annotate: default-impl "= default"}} Defaulting the spaceship performs member-wise comparison in declaration order. For most value types, this is exactly right.

{{zoom: 1x}} {{focus: custom}} {{zoom: 1.2x}} {{annotate: custom "Custom logic"}} For custom comparison logic, implement the operator yourself. Return strong ordering for types with a single canonical ordering, partial ordering for types where some values are incomparable.

{{zoom: 1x}} {{focus: none}} The spaceship operator eliminates boilerplate. One line replaces six operator overloads.

```code:spaceship-code lang=cpp
#include <compare>
#include <string>

struct Point {
    int x, y;
    auto operator<=>(const Point&) const = default;
};

struct Student {
    std::string name;
    double gpa;

    // Custom: compare by GPA descending, then name ascending
    std::strong_ordering operator<=>(const Student& other) const {
        if (auto cmp = other.gpa <=> gpa; cmp != 0) return cmp;
        return name <=> other.name;
    }
    bool operator==(const Student&) const = default;
};
---
spaceship: 6
default-impl: 6
custom: 14-18
```
