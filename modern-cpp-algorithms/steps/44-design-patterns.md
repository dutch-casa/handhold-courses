---
title: Design patterns in modern C++
---

# Strategy with std::function

{{show: strategy-code typewriter 2s linear}} The strategy pattern: swap algorithms at runtime. In classic OOP, this requires an interface hierarchy. In modern C++, std::function does it in one line.

{{focus: old-way}} {{zoom: 1.2x}} {{annotate: old-way "Inheritance tax"}} The old way: define a Comparator interface, create LessThan and GreaterThan classes, pass pointers to the sorter. Three classes for what should be a single callable.

{{zoom: 1x}} {{focus: modern}} {{zoom: 1.2x}} {{annotate: modern "Just a function"}} The modern way: accept a std::function. Pass a lambda, a function pointer, or any callable. No interface, no hierarchy, no boilerplate. The strategy IS the function.

{{zoom: 1x}} {{focus: usage}} {{zoom: 1.2x}} {{annotate: usage "Swap at call site"}} At the call site, the strategy is a lambda. Change behavior by passing a different lambda. No new classes, no registration, no factory.

{{zoom: 1x}} {{focus: none}} std::function erases the callable's type. It can hold lambdas, function pointers, bound methods, functors — anything callable with the right signature. That's strategy without the ceremony.

```code:strategy-code lang=cpp
#include <functional>
#include <vector>
#include <algorithm>

// Old way: interface hierarchy
class Comparator {
public:
    virtual ~Comparator() = default;
    virtual bool compare(int a, int b) const = 0;
};

// Modern way: std::function IS the strategy
class Sorter {
public:
    using Strategy = std::function<bool(int, int)>;

    explicit Sorter(Strategy cmp) : compare_(std::move(cmp)) {}

    void sort(std::vector<int>& data) {
        std::sort(data.begin(), data.end(), compare_);
    }

private:
    Strategy compare_;
};

// Usage — strategy is a lambda
Sorter ascending([](int a, int b) { return a < b; });
Sorter descending([](int a, int b) { return a > b; });
---
old-way: 6-10
modern: 13-24
usage: 27-28
```

# Observer with callbacks

{{clear: slide}}

{{show: observer-code typewriter 2s linear}} The observer pattern: notify interested parties when something changes. Classic observer uses an interface with an update method. Modern C++ uses callbacks.

{{focus: subscribe}} {{zoom: 1.2x}} {{annotate: subscribe "Register interest"}} Subscribe takes a callback — any callable matching the signature. Returns an ID for later unsubscription. No Observer interface, no inheritance.

{{zoom: 1x}} {{focus: notify}} {{zoom: 1.2x}} {{annotate: notify "Broadcast"}} Notify iterates through callbacks and invokes each one. The signal class doesn't know or care what the callbacks do. Complete decoupling.

{{zoom: 1x}} {{focus: connect}} {{zoom: 1.2x}} {{annotate: connect "Wire it up"}} Connection is a single line. A lambda captures whatever context it needs. Disconnection uses the returned ID.

{{zoom: 1x}} {{focus: none}} This is the observer pattern reduced to its essence: a list of callbacks. No abstract base, no multiple inheritance, no shared state.

```code:observer-code lang=cpp
#include <functional>
#include <unordered_map>

template<typename... Args>
class Signal {
public:
    using Callback = std::function<void(Args...)>;

    int connect(Callback cb) {
        int id = next_id_++;
        slots_[id] = std::move(cb);
        return id;
    }

    void disconnect(int id) { slots_.erase(id); }

    void emit(Args... args) const {
        for (const auto& [id, cb] : slots_) {
            cb(args...);
        }
    }

private:
    int next_id_ = 0;
    std::unordered_map<int, Callback> slots_;
};

// Usage
Signal<int> on_value_changed;
int id = on_value_changed.connect([](int val) {
    std::cout << "Value: " << val << "\n";
});
on_value_changed.emit(42);
on_value_changed.disconnect(id);
---
subscribe: 8-12
notify: 16-20
connect: 28-31
```

# RAII as a pattern

{{clear: slide}}

{{show: raii-code typewriter 2s linear}} RAII — Resource Acquisition Is Initialization — is the most important pattern in C++. Acquire in the constructor, release in the destructor. The compiler guarantees cleanup.

{{focus: lock-guard}} {{zoom: 1.2x}} {{annotate: lock-guard "Automatic unlock"}} A lock guard acquires a mutex in its constructor and releases it in its destructor. When the guard goes out of scope — normally or via exception — the mutex is released. No manual unlock. No forgotten cleanup.

{{zoom: 1x}} {{focus: scope-guard}} {{zoom: 1.2x}} {{annotate: scope-guard "General cleanup"}} A scope guard runs arbitrary cleanup code on destruction. Pass a lambda. It runs when the scope exits, no matter how. This is RAII generalized to any cleanup action.

{{zoom: 1x}} {{focus: timer}} {{zoom: 1.2x}} {{annotate: timer "Measure scope"}} A scope timer records the start time on construction and logs the elapsed duration on destruction. RAII for instrumentation.

{{zoom: 1x}} {{focus: none}} RAII is why C++ doesn't need try/finally. The destructor IS the finally block. Build every resource around it.

```code:raii-code lang=cpp
#include <mutex>
#include <functional>
#include <chrono>

// Lock guard — classic RAII
std::mutex mtx;
{
    std::lock_guard<std::mutex> lock(mtx);  // Acquires
    // Critical section
}   // Releases — even if exception thrown

// Scope guard — general RAII
class ScopeGuard {
public:
    explicit ScopeGuard(std::function<void()> fn) : fn_(std::move(fn)) {}
    ~ScopeGuard() { if (fn_) fn_(); }

    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
private:
    std::function<void()> fn_;
};

// Usage
auto guard = ScopeGuard([] { std::cout << "cleanup\n"; });

// Scope timer — RAII for measurement
class ScopeTimer {
public:
    ScopeTimer() : start_(std::chrono::steady_clock::now()) {}
    ~ScopeTimer() {
        auto elapsed = std::chrono::steady_clock::now() - start_;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
        std::cout << ms.count() << "ms\n";
    }
private:
    std::chrono::steady_clock::time_point start_;
};
---
lock-guard: 6-10
scope-guard: 13-23
timer: 28-37
```

# Type erasure

{{clear: slide}}

{{show: erase-code typewriter 2s linear}} Type erasure hides concrete types behind a uniform interface without requiring inheritance from the user's types. The erased type owns a concept and a model.

{{focus: concept-model}} {{zoom: 1.2x}} {{annotate: concept-model "Hidden hierarchy"}} The concept is a private abstract base. The model is a private template that wraps any concrete type implementing the required operations. Users never see either.

{{zoom: 1x}} {{focus: public-api}} {{zoom: 1.2x}} {{annotate: public-api "Simple surface"}} The public class holds a unique pointer to the concept. Its interface delegates to the model. Callers see a simple value type with no templates.

{{zoom: 1x}} {{focus: usage}} {{zoom: 1.2x}} {{annotate: usage "Any type fits"}} Any type with a draw method works. No base class needed. No inheritance. The model template wraps it automatically. This is runtime polymorphism without coupling.

{{zoom: 1x}} {{focus: none}} Type erasure is how std::function, std::any, and std::move only iterator work. It's the pattern for when you need polymorphism but can't modify the types.

```code:erase-code lang=cpp
#include <memory>
#include <iostream>

class Drawable {
    // Hidden concept
    struct Concept {
        virtual ~Concept() = default;
        virtual void do_draw() const = 0;
        virtual std::unique_ptr<Concept> clone() const = 0;
    };

    // Hidden model — wraps any type with draw()
    template<typename T>
    struct Model : Concept {
        T obj;
        explicit Model(T o) : obj(std::move(o)) {}
        void do_draw() const override { obj.draw(); }
        std::unique_ptr<Concept> clone() const override {
            return std::make_unique<Model>(obj);
        }
    };

    std::unique_ptr<Concept> self_;

public:
    template<typename T>
    Drawable(T obj) : self_(std::make_unique<Model<T>>(std::move(obj))) {}
    Drawable(const Drawable& o) : self_(o.self_->clone()) {}
    Drawable& operator=(const Drawable& o) { self_ = o.self_->clone(); return *this; }

    void draw() const { self_->do_draw(); }
};

// Any type with draw() works — no inheritance required
struct Square { void draw() const { std::cout << "Square\n"; } };
struct Text   { void draw() const { std::cout << "Text\n"; } };

std::vector<Drawable> scene;
scene.push_back(Square{});
scene.push_back(Text{});
---
concept-model: 6-21
public-api: 25-30
usage: 34-39
```

# Builder with method chaining

{{clear: slide}}

{{show: builder-code typewriter 1.5s linear}} The builder pattern constructs complex objects step by step. Method chaining makes the construction read like a sentence.

{{focus: builder-class}} {{zoom: 1.2x}} {{annotate: builder-class "Fluent interface"}} Each setter returns a reference to the builder itself. Calls chain: builder dot host dot port dot timeout dot build. Left to right, one field per call.

{{zoom: 1x}} {{focus: build-method}} {{zoom: 1.2x}} {{annotate: build-method "Finalize"}} The build method validates and constructs the final object. It can enforce invariants that individual setters can't — like requiring both host and port.

{{zoom: 1x}} {{focus: usage}} {{zoom: 1.2x}} {{annotate: usage "Reads like prose"}} The call site is self-documenting. Each field is named. Order doesn't matter. Optional fields have defaults. Compare this to a constructor with six positional arguments.

{{zoom: 1x}} {{focus: none}} Builders are for objects with many optional parameters and cross-field validation. For simple structs with defaults, designated initializers are simpler.

```code:builder-code lang=cpp
#include <string>
#include <stdexcept>

struct Connection {
    std::string host;
    int port;
    int timeout_ms;
    bool use_tls;
};

class ConnectionBuilder {
public:
    ConnectionBuilder& host(std::string h) { host_ = std::move(h); return *this; }
    ConnectionBuilder& port(int p) { port_ = p; return *this; }
    ConnectionBuilder& timeout(int ms) { timeout_ = ms; return *this; }
    ConnectionBuilder& tls(bool t) { tls_ = t; return *this; }

    Connection build() {
        if (host_.empty()) throw std::invalid_argument("host required");
        if (port_ <= 0) throw std::invalid_argument("port required");
        return {host_, port_, timeout_, tls_};
    }

private:
    std::string host_;
    int port_ = 0;
    int timeout_ = 5000;
    bool tls_ = false;
};

// Usage
auto conn = ConnectionBuilder{}
    .host("api.example.com")
    .port(443)
    .tls(true)
    .build();
---
builder-class: 12-16
build-method: 18-22
usage: 31-35
```

# Factory with unique_ptr

{{clear: slide}}

{{show: factory-code typewriter 1.5s linear}} A factory function returns a unique pointer to a base class. The caller gets ownership of a polymorphic object without knowing the concrete type.

{{focus: factory-func}} {{zoom: 1.2x}} {{annotate: factory-func "Create by name"}} The factory takes a descriptor — a string, an enum, a config — and returns the right concrete type. The return type is always the base. make unique constructs the derived object directly on the heap.

{{zoom: 1x}} {{focus: caller}} {{zoom: 1.2x}} {{annotate: caller "Type-blind"}} The caller holds a unique pointer to the base. It calls virtual methods. It doesn't know or care whether it got a Circle, a Rectangle, or something else. Ownership is clear — the unique pointer manages the lifetime.

{{zoom: 1x}} {{focus: none}} Factory plus unique pointer is the modern replacement for raw new. The factory decides what to create. The smart pointer decides when to destroy. No manual memory management.

```code:factory-code lang=cpp
#include <memory>
#include <string>
#include <stdexcept>

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
};

class Circle : public Shape {
    double r_;
public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return 3.14159 * r_ * r_; }
};

class Rectangle : public Shape {
    double w_, h_;
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
};

// Factory function
std::unique_ptr<Shape> make_shape(const std::string& type, double a, double b = 0) {
    if (type == "circle") return std::make_unique<Circle>(a);
    if (type == "rect")   return std::make_unique<Rectangle>(a, b);
    throw std::invalid_argument("unknown shape: " + type);
}

// Caller is type-blind
auto s = make_shape("circle", 5.0);
double a = s->area();  // Virtual dispatch
---
factory-func: 26-30
caller: 33-34
```

# Synthesis

{{clear: slide}}

{{show: synth-viz grow 0.5s spring}} Modern C++ design combines three pillars. Each operates at a different level. Together they cover every design decision.

{{focus: runtime}} {{annotate: runtime "Virtual dispatch"}} Runtime polymorphism — virtual functions, type erasure, std::function. Use at API boundaries where types are decided at runtime. Pay the vtable cost only where you need extensibility.

{{focus: structure}} {{annotate: structure "Composition"}} Composition — objects containing other objects. The default relationship. Flexible, testable, replaceable. Motor inside Robot. Strategy inside Sorter. Signal inside EventBus.

{{focus: compile}} {{annotate: compile "Templates + concepts"}} Compile-time polymorphism — templates, concepts, constexpr. Use for algorithms that work across types. Zero runtime cost. The compiler generates exactly the code you need.

{{focus: none}} The pattern that emerges: deep modules with simple interfaces. Hide the complexity — whether it's a vtable, a template instantiation, or a composed subsystem — behind a surface that callers can use without understanding the internals. That's the design goal. Everything else is technique.

```data:synth-viz type=array
["Runtime: virtual, type erasure, std::function — API boundaries", "Structure: composition, RAII, value semantics — internal wiring", "Compile-time: templates, concepts, constexpr — generic algorithms"]
---
runtime: 0
structure: 1
compile: 2
```
