---
title: Inheritance and composition
---

# When to inherit

{{show: inherit-viz grow 0.5s spring}} Inheritance models an "is-a" relationship. A Dog is an Animal. A Circle is a Shape. If the substitution holds — if callers can use the derived class everywhere they use the base — inheritance is appropriate.

{{focus: good-inherit}} {{annotate: good-inherit "Substitution holds"}} Good inheritance: Circle inherits Shape. Every function expecting a Shape works with a Circle. The interface contract is preserved. Callers don't know or care which concrete type they hold.

{{focus: bad-inherit}} {{annotate: bad-inherit "Substitution breaks"}} Bad inheritance: Stack inherits Vector. A stack is not a general vector. Callers could call insert, erase, or random access — operations that violate stack semantics. The contract is broken.

{{focus: none}} The test is Liskov substitution: can the derived type replace the base in every context without surprises? If yes, inherit. If no, compose.

```data:inherit-viz type=array
["Good: Circle → Shape (substitution holds)", "Bad: Stack → Vector (contract breaks)"]
---
good-inherit: 0
bad-inherit: 1
```

# Virtual functions and polymorphism

{{clear: slide}}

{{show: virtual-code typewriter 2s linear}} Virtual functions enable runtime polymorphism. The base class declares the interface. Derived classes provide implementations. Callers work through base pointers.

{{focus: base-class}} {{zoom: 1.2x}} {{annotate: base-class "The interface"}} The base class declares virtual methods. Pure virtual (equals zero) means no default — derived classes must implement. A class with any pure virtual is abstract.

{{zoom: 1x}} {{focus: derived}} {{zoom: 1.2x}} {{annotate: derived "The implementation"}} Override provides the concrete behavior. The override keyword tells the compiler to verify you're actually overriding a virtual method. Always use it.

{{zoom: 1x}} {{focus: dispatch}} {{zoom: 1.2x}} {{annotate: dispatch "Runtime selection"}} Call through a base pointer or reference. The correct derived implementation runs. This is dynamic dispatch — the vtable selects the right function at runtime.

{{zoom: 1x}} {{focus: none}} Virtual destructor is mandatory in any base class used polymorphically. Without it, deleting through a base pointer leaks derived resources.

```code:virtual-code lang=cpp
#include <memory>
#include <string>
#include <vector>

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual std::string name() const = 0;
};

class Circle : public Shape {
public:
    explicit Circle(double r) : radius_(r) {}
    double area() const override { return 3.14159 * radius_ * radius_; }
    std::string name() const override { return "Circle"; }
private:
    double radius_;
};

// Runtime dispatch
void print_area(const Shape& s) {
    // Calls the correct derived implementation
    std::cout << s.name() << ": " << s.area() << "\n";
}
---
base-class: 5-10
derived: 12-19
dispatch: 22-25
```

# Composition over inheritance

{{clear: slide}}

{{show: comp-code typewriter 2s linear}} Composition means a class contains another class as a member. It models "has-a" relationships and is almost always more flexible than inheritance.

{{focus: inheritance-ver}} {{zoom: 1.2x}} {{annotate: inheritance-ver "Coupled"}} The inheritance version: Robot inherits Animal. It gains walk, eat, sleep — methods that make no sense for a robot. You override them to do nothing. The base interface leaks into the derived class.

{{zoom: 1x}} {{focus: composition-ver}} {{zoom: 1.2x}} {{annotate: composition-ver "Decoupled"}} The composition version: Robot contains a Motor and a Sensor. It exposes only what makes sense. No inherited baggage. Swap Motor for a different implementation without changing Robot's interface.

{{zoom: 1x}} {{focus: none}} Inheritance creates tight coupling — the derived class depends on the base's entire interface. Composition creates loose coupling — the container depends only on the member's public interface. Default to composition. Use inheritance only when substitution genuinely applies.

```code:comp-code lang=cpp
// Inheritance: Robot IS an Animal? No.
class Animal {
public:
    virtual void walk() = 0;
    virtual void eat() = 0;   // Makes no sense for Robot
    virtual ~Animal() = default;
};

// Composition: Robot HAS a Motor and Sensor. Yes.
class Motor {
public:
    void move(double speed) { /* ... */ }
};

class Sensor {
public:
    double read() const { return 42.0; }
};

class Robot {
public:
    void advance(double speed) { motor_.move(speed); }
    double sense() const { return sensor_.read(); }
private:
    Motor motor_;
    Sensor sensor_;
};
---
inheritance-ver: 2-7
composition-ver: 10-25
```

# Abstract classes and interfaces

{{clear: slide}}

{{show: abstract-code typewriter 1.5s linear}} An abstract class defines a contract. It can't be instantiated. Derived classes must fulfill every pure virtual method.

{{focus: pure-interface}} {{zoom: 1.2x}} {{annotate: pure-interface "Pure contract"}} A class with only pure virtual methods and a virtual destructor is a pure interface. No data, no implementation. Just the contract.

{{zoom: 1x}} {{focus: partial-abstract}} {{zoom: 1.2x}} {{annotate: partial-abstract "Shared behavior"}} An abstract class with some implemented methods provides shared behavior. Derived classes override the abstract parts but inherit the concrete parts. Use this when multiple derived classes share logic.

{{zoom: 1x}} {{focus: none}} Prefer pure interfaces at boundaries. Use abstract classes with shared behavior only when the shared code is genuinely common and stable.

```code:abstract-code lang=cpp
// Pure interface — no implementation
class Serializable {
public:
    virtual ~Serializable() = default;
    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& data) = 0;
};

// Abstract class with shared behavior
class Container {
public:
    virtual ~Container() = default;
    virtual void add(int val) = 0;
    virtual int size() const = 0;

    bool empty() const { return size() == 0; }  // Shared
    void add_all(const std::vector<int>& vals) { // Shared
        for (int v : vals) add(v);
    }
};
---
pure-interface: 2-7
partial-abstract: 10-20
```

# The diamond problem

{{clear: slide}}

{{show: diamond-viz grow 0.5s spring}} Multiple inheritance creates ambiguity when two bases share a common ancestor. Class D inherits B and C, both inheriting A. Which copy of A does D get?

{{focus: ambiguous}} {{annotate: ambiguous "Two copies of A"}} Without virtual inheritance, D contains two separate copies of A's members. Calling an A method through D is ambiguous — the compiler doesn't know which copy you mean.

{{focus: virtual-fix}} {{annotate: virtual-fix "One shared copy"}} Virtual inheritance solves this. B and C virtually inherit A. Now D has a single shared copy of A. The most-derived class (D) is responsible for constructing A.

{{focus: none}}

{{show: diamond-code typewriter 1.5s linear}} In practice, avoid deep multiple inheritance hierarchies. They're fragile and hard to reason about. Prefer composition or multiple interfaces (pure abstract classes with no data).

{{focus: impl}} {{zoom: 1.2x}} {{annotate: impl "Virtual base"}} The virtual keyword on the inheritance declaration is the fix. It tells the compiler to share one base subobject across all paths.

{{zoom: 1x}} {{focus: none}} The diamond problem is a design smell. If you hit it, reconsider whether inheritance is the right tool.

```data:diamond-viz type=array
["A ← B, A ← C, B+C ← D: two copies of A", "virtual A ← B, virtual A ← C, B+C ← D: one copy"]
---
ambiguous: 0
virtual-fix: 1
```

```code:diamond-code lang=cpp
class A {
public:
    int value = 0;
};

class B : virtual public A {};
class C : virtual public A {};

class D : public B, public C {
public:
    void set(int v) { value = v; }  // Unambiguous — one copy of A
};
---
impl: 6-7
```

# CRTP: static polymorphism

{{clear: slide}}

{{show: crtp-code typewriter 2s linear}} The Curiously Recurring Template Pattern. A class inherits from a template instantiated with itself. Sounds circular. Works beautifully.

{{focus: base-crtp}} {{zoom: 1.2x}} {{annotate: base-crtp "Static dispatch"}} The base class casts this to the derived type at compile time. No virtual function, no vtable, no runtime overhead. The compiler resolves the call statically.

{{zoom: 1x}} {{focus: derived-crtp}} {{zoom: 1.2x}} {{annotate: derived-crtp "Provides implementation"}} The derived class implements the required method. The base calls it through the static cast. Same pattern as virtual dispatch, but resolved at compile time.

{{zoom: 1x}} {{focus: usage}} {{zoom: 1.2x}} {{annotate: usage "Zero overhead"}} Usage looks like regular polymorphism within templates. But there's no vtable pointer, no indirection, no runtime cost. The compiler inlines everything.

{{zoom: 1x}} {{focus: none}} CRTP is powerful for performance-critical code where you need polymorphic behavior without runtime dispatch. But prefer concepts (C++20) when the constraint is on types, not on implementation inheritance.

```code:crtp-code lang=cpp
template<typename Derived>
class Printable {
public:
    void print() const {
        // Static cast to derived — resolved at compile time
        const auto& self = static_cast<const Derived&>(*this);
        std::cout << self.to_string() << "\n";
    }
};

class Name : public Printable<Name> {
public:
    explicit Name(std::string n) : name_(std::move(n)) {}
    std::string to_string() const { return name_; }
private:
    std::string name_;
};

// Usage
Name n("Alice");
n.print();  // No virtual dispatch — fully inlined
---
base-crtp: 1-8
derived-crtp: 10-16
usage: 19-20
```

# Design synthesis

{{clear: slide}}

{{show: decision-viz grow 0.5s spring}} A decision framework for choosing between inheritance, composition, and templates.

{{focus: use-inherit}} {{annotate: use-inherit "Polymorphic dispatch"}} Use inheritance when: you need runtime polymorphism, Liskov substitution holds, and you're designing a closed set of types behind a stable interface.

{{focus: use-compose}} {{annotate: use-compose "Flexible assembly"}} Use composition when: the relationship is "has-a", you want to swap implementations, or you need multiple capabilities that don't form a hierarchy.

{{focus: use-template}} {{annotate: use-template "Compile-time"}} Use templates when: the behavior is known at compile time, you want zero overhead, or you're writing generic code that works across unrelated types.

{{focus: none}} Most real systems use all three. Virtual dispatch at API boundaries for extensibility. Composition for internal structure. Templates for generic algorithms. The art is knowing which tool fits each joint.

```data:decision-viz type=array
["Inherit: runtime polymorphism, substitution holds, stable interface", "Compose: has-a, swappable parts, multiple capabilities", "Template: compile-time, zero overhead, generic algorithms"]
---
use-inherit: 0
use-compose: 1
use-template: 2
```
