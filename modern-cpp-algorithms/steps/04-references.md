---
title: References and the stack
---

# The stack

{{show: stack-frames grow 0.5s spring}} Every function call pushes a frame onto the stack. Every return pops it off.

{{focus: main-frame}} {{annotate: main-frame "Bottom"}} Main starts at the bottom. It allocates space for its local variables.

{{focus: foo-frame}} {{annotate: foo-frame "Called by main"}} When main calls foo, a new frame is pushed on top. Foo's local variables live here.

{{focus: bar-frame}} {{annotate: bar-frame "Called by foo"}} When foo calls bar, another frame. The stack grows upward. Each frame is isolated — bar can't see main's variables.

{{focus: none}} When bar returns, its frame is destroyed. Instantly. No garbage collector. No cleanup delay. The stack pointer moves down and that memory is gone.

```data:stack-frames type=stack
["main: x=5, y=10", "foo: a=3, b=7", "bar: result=21"]
^top=2
---
main-frame: 0
foo-frame: 1
bar-frame: 2
```

# Pass by value

{{clear: slide}}

{{show: by-value typewriter 1.5s linear}} When you pass an argument to a function, C++ makes a copy by default.

{{focus: caller}} {{zoom: 1.2x}} {{annotate: caller "Original"}} In main, x is 10. You pass x to double it.

{{zoom: 1x}} {{focus: copy}} {{zoom: 1.2x}} {{annotate: copy "Copy"}} The function receives n, a copy of x. Modifying n changes the copy. The original x in main is untouched.

{{zoom: 1x}} {{focus: unchanged}} {{zoom: 1.3x}} {{annotate: unchanged "Still 10"}} After the call, x is still 10. The function operated on its own copy. This is safe but wasteful for large objects — copying a million-element vector takes time.

{{zoom: 1x}} {{focus: none}} Pass by value is the default. It's safe but creates copies.

```code:by-value lang=cpp
void double_it(int n) {
    n = n * 2;  // modifies the copy
}

int main() {
    int x = 10;
    double_it(x);
    // x is still 10
    return 0;
}
---
copy: 1-3
caller: 6
unchanged: 8
```

# Lvalue references

{{clear: slide}}

{{show: ref-code typewriter 1.5s linear}} A reference is an alias. It doesn't copy — it refers to the original.

{{focus: ref-param}} {{zoom: 1.3x}} {{annotate: ref-param "Reference parameter"}} The ampersand after int makes n a reference to the argument. Not a copy. The actual variable from the caller.

{{zoom: 1x}} {{focus: mutates}} {{zoom: 1.2x}} {{annotate: mutates "Modifies original"}} When you modify n inside the function, you're modifying x in main. Same memory location. Same variable. Different name.

{{zoom: 1x}} {{focus: changed}} {{zoom: 1.3x}} {{annotate: changed "Now 20"}} After the call, x is 20. The function changed it through the reference.

{{zoom: 1x}} {{focus: none}} References give you direct access to the caller's variable. Powerful, but dangerous if misused.

```code:ref-code lang=cpp
void double_it(int& n) {  // n is a reference to the argument
    n = n * 2;             // modifies the original
}

int main() {
    int x = 10;
    double_it(x);
    // x is now 20
    return 0;
}
---
ref-param: 1
mutates: 2
changed: 8
```

# Const references

{{clear: slide}}

{{show: const-ref typewriter 1.5s linear}} What if you want to avoid copying but don't want to modify the original? Const reference.

{{focus: const-param}} {{zoom: 1.3x}} {{annotate: const-param "Read-only alias"}} Const ampersand. It's a reference — no copy. But the function can't modify it. The compiler enforces this.

{{zoom: 1x}} {{focus: efficient}} {{zoom: 1.2x}} {{annotate: efficient "Zero-copy read"}} For a string with a million characters, const reference passes a pointer-sized value instead of copying the entire string. Same for vectors, maps, any large object.

{{zoom: 1x}} {{focus: rule}} {{zoom: 1.2x}} {{annotate: rule "Default choice"}} The rule: pass small types like int and double by value. Pass everything else by const reference. Only use non-const reference when the function must modify the argument.

{{zoom: 1x}} {{focus: none}} Const reference is the workhorse of C++ parameter passing.

```code:const-ref lang=cpp
#include <string>

void print_length(const std::string& s) {  // no copy, no modify
    // s[0] = 'X';  // ERROR: s is const
    std::cout << s.length() << "\n";
}

int main() {
    std::string msg = "Hello, world!";
    print_length(msg);  // zero-copy, msg unchanged
    return 0;
}

// The rule:
// int, double, char, bool    → pass by value
// string, vector, map, ...   → pass by const reference
// must modify the argument?  → pass by non-const reference
---
const-param: 3
efficient: 10
rule: 14-17
```

# Dangling references

{{clear: slide}}

{{show: dangle-code typewriter 1.5s linear}} A reference to a destroyed variable is a dangling reference. Using it is undefined behavior.

{{focus: local-var}} {{zoom: 1.2x}} {{annotate: local-var "Lives on stack"}} The variable x lives in bad ref's stack frame. It exists for the lifetime of the function.

{{zoom: 1x}} {{focus: return-ref}} {{zoom: 1.3x}} {{annotate: return-ref "Disaster"}} The function returns a reference to x. But when the function returns, x's stack frame is destroyed. The reference points to dead memory.

{{zoom: 1x}} {{focus: kaboom}} {{zoom: 1.2x}} {{annotate: kaboom "Undefined behavior"}} The caller gets a reference to garbage. Reading it might give you a random number. Writing it might corrupt the stack. The compiler won't stop you.

{{zoom: 1x}} {{focus: none}} Never return a reference to a local variable. The variable dies when the function returns.

```code:dangle-code lang=cpp
int& bad_ref() {
    int x = 42;     // x lives on the stack
    return x;       // returning reference to local — DANGER
}  // x is destroyed here

int main() {
    int& ref = bad_ref();
    // ref is dangling — x no longer exists
    // using ref is undefined behavior
    return 0;
}
---
local-var: 2
return-ref: 3
kaboom: 8-9
```

# Rvalue reference preview

{{clear: slide}}

{{show: rvalue-preview typewriter 1.5s linear}} There's another kind of reference we'll cover in depth next lesson. A quick preview.

{{focus: lvalue}} {{zoom: 1.2x}} {{annotate: lvalue "Has an address"}} An lvalue is something with a name and an address. A variable. You can take its address with the ampersand operator.

{{zoom: 1x}} {{focus: rvalue}} {{zoom: 1.2x}} {{annotate: rvalue "Temporary"}} An rvalue is a temporary. The result of an expression. It doesn't have a name. It doesn't persist past the current statement.

{{zoom: 1x}} {{focus: double-amp}} {{zoom: 1.3x}} {{annotate: double-amp "Move semantics"}} A double ampersand binds to rvalues. It lets you "steal" the contents of a temporary instead of copying them. This is move semantics — the core of efficient modern C++.

{{zoom: 1x}} {{focus: none}} We'll build this up fully in the smart pointers lesson. For now, know that two ampersands exist and they're about efficiency.

```code:rvalue-preview lang=cpp
int x = 42;              // x is an lvalue (has a name)
int y = x + 1;           // (x + 1) is an rvalue (temporary)

int& ref = x;            // lvalue reference: binds to lvalue
// int& bad = 42;        // ERROR: can't bind lvalue ref to rvalue

int&& rref = 42;         // rvalue reference: binds to temporary
int&& moved = x + 1;    // binds to the temporary result

// Next lesson: move semantics and why this matters
---
lvalue: 1-2
rvalue: 2
double-amp: 7-8
```

# The full picture

{{clear: slide}}

{{show: ref-summary grow 0.5s spring}} Three kinds of references. Each has its job.

{{focus: value-row}} {{annotate: value-row "Copies"}} Pass by value. Safe, independent copy. Use for small types.

{{focus: const-ref-row}} {{annotate: const-ref-row "Read-only alias"}} Const reference. No copy, no mutation. The default for large objects.

{{focus: mut-ref-row}} {{annotate: mut-ref-row "Read-write alias"}} Mutable reference. No copy, allows mutation. Use when the function must modify the argument.

{{focus: rvalue-row}} {{annotate: rvalue-row "Steal contents"}} Rvalue reference. Binds to temporaries. Enables move semantics. More next lesson.

{{focus: none}} That's the reference system. The stack handles lifetime. References handle access. Together they give you control without a garbage collector.

```data:ref-summary type=array
["T: value (copy)", "const T&: read-only ref", "T&: mutable ref", "T&&: rvalue ref (move)"]
---
value-row: 0
const-ref-row: 1
mut-ref-row: 2
rvalue-row: 3
```
