---
title: Pointers and the heap
---

# What is a pointer

{{show: ptr-diagram grow 0.5s spring}} A pointer is a variable that holds a memory address. That's it. An address. A number that says "look over there."

{{focus: var-node}} {{annotate: var-node "The variable"}} The variable x sits at some memory address. It holds the value 42.

{{focus: ptr-node}} {{annotate: ptr-node "Holds the address"}} The pointer p holds the address of x. Not the value 42. The address where 42 lives.

{{focus: arrow}} {{annotate: arrow "Points to"}} The arrow is the dereference. When you write star p, you follow the arrow and get the value at that address. Star p is 42.

{{focus: none}} A pointer is one level of indirection. It doesn't hold data. It holds the location of data.

```diagram:ptr-diagram
x [service]
p [service]
p --points to--> x
---
var-node: x
ptr-node: p
arrow: p, x
```

# Pointer syntax

{{clear: slide}}

{{show: ptr-code typewriter 1.5s linear}} Three operations. Declare. Take the address. Dereference.

{{focus: declare}} {{zoom: 1.2x}} {{annotate: declare "int* means pointer to int"}} The star in the declaration means "pointer to." int star p declares p as a pointer to an integer.

{{zoom: 1x}} {{focus: address-of}} {{zoom: 1.3x}} {{annotate: address-of "& gets the address"}} The ampersand operator takes the address of a variable. Ampersand x gives you the memory address where x lives.

{{zoom: 1x}} {{focus: deref}} {{zoom: 1.2x}} {{annotate: deref "* follows the pointer"}} The star operator dereferences. Star p follows the pointer and gives you the value it points to.

{{zoom: 1x}} {{focus: mutate}} {{zoom: 1.2x}} {{annotate: mutate "Modify through pointer"}} You can modify through a pointer. Star p equals 100 changes the value at the address p holds. Since p points to x, x is now 100.

{{zoom: 1x}} {{focus: nullptr-line}} {{zoom: 1.3x}} {{annotate: nullptr-line "Points nowhere"}} nullptr means "points to nothing." Always initialize pointers. An uninitialized pointer holds garbage — dereferencing it is undefined behavior.

{{zoom: 1x}} {{focus: none}} Three operators: star for declaration and dereference, ampersand for address-of. That's the whole syntax.

```code:ptr-code lang=cpp
int x = 42;
int* p = &x;       // p holds the address of x

int val = *p;       // val is 42 (dereference)
*p = 100;           // x is now 100 (modify through pointer)

int* q = nullptr;   // q points to nothing
// *q;              // CRASH: dereferencing null
---
declare: 2
address-of: 2
deref: 4
mutate: 5
nullptr-line: 7
```

# The heap

{{clear: slide}}

{{show: stack-heap grow 0.5s spring}} The stack handles function calls. The heap handles everything else.

{{focus: stack-side}} {{annotate: stack-side "Automatic"}} Stack memory is automatic. It's allocated when a function is called and freed when it returns. Fast. Predictable. Limited size.

{{focus: heap-side}} {{annotate: heap-side "Manual"}} Heap memory is manual. You allocate it with new. You free it with delete. It persists until you explicitly release it. No automatic cleanup.

{{focus: none}} The stack is a conveyor belt — things come and go in order. The heap is a warehouse — you rent space and return it when you're done.

```diagram:stack-heap
stack [service]
heap [service]
program [service]
program --auto--> stack
program --manual--> heap
{Memory: stack, heap}
---
stack-side: stack
heap-side: heap
```

# New and delete

{{clear: slide}}

{{show: new-delete typewriter 1.5s linear}} New allocates on the heap. Delete frees it. You must pair them.

{{focus: new-line}} {{zoom: 1.3x}} {{annotate: new-line "Allocate"}} New int parenthesis 42 allocates space for one int on the heap, initializes it to 42, and returns a pointer to it.

{{zoom: 1x}} {{focus: use-line}} {{zoom: 1.2x}} {{annotate: use-line "Use it"}} You use the pointer like before. Star p gives you the value.

{{zoom: 1x}} {{focus: delete-line}} {{zoom: 1.3x}} {{annotate: delete-line "Free it"}} Delete p frees the memory. After delete, p is a dangling pointer. Using it is undefined behavior.

{{zoom: 1x}} {{focus: array-new}} {{zoom: 1.2x}} {{annotate: array-new "Array allocation"}} For arrays, use new bracket and delete bracket. Note the brackets on delete. Miss them and you get undefined behavior.

{{zoom: 1x}} {{focus: none}} Every new needs a delete. Every new bracket needs a delete bracket. Miss one and you leak memory.

```code:new-delete lang=cpp
int* p = new int(42);       // allocate one int on the heap
int val = *p;                // use it: val is 42
delete p;                    // free it
// p is now dangling — don't use it

int* arr = new int[5];       // allocate array of 5 ints
arr[0] = 10;
arr[1] = 20;
delete[] arr;                // free array (note the [])
---
new-line: 1
use-line: 2
delete-line: 3
array-new: 6-9
```

# Memory leaks

{{clear: slide}}

{{show: leak-code typewriter 1.5s linear}} A memory leak happens when you allocate heap memory and lose the pointer to it. The memory stays allocated forever. Nobody can free it.

{{focus: leak-fn}} {{zoom: 1.2x}} {{annotate: leak-fn "Leak every call"}} This function allocates an int on the heap and never frees it. Every call leaks four bytes. Call it a million times — four megabytes gone.

{{zoom: 1x}} {{focus: overwrite-leak}} {{zoom: 1.3x}} {{annotate: overwrite-leak "Lost pointer"}} This is worse. You allocate, then point p at a new allocation. The first block is orphaned. No pointer reaches it. No way to free it. Leaked.

{{zoom: 1x}} {{focus: exception-leak}} {{zoom: 1.2x}} {{annotate: exception-leak "Exception path"}} Functions that throw can leak too. If an exception fires between new and delete, delete never runs. The memory leaks.

{{zoom: 1x}} {{focus: none}} Memory leaks are silent. No crash. No error. Just steadily growing memory usage until the system runs out. This is why smart pointers exist. Next lesson.

```code:leak-code lang=cpp
void leaky() {
    int* p = new int(42);
    // forgot delete — leaked!
}

void overwrite_leak() {
    int* p = new int(1);    // allocate first block
    p = new int(2);         // first block is orphaned — leaked!
    delete p;               // only frees the second block
}

void exception_leak() {
    int* p = new int(42);
    // if something throws here, delete never runs
    might_throw();
    delete p;
}
---
leak-fn: 1-4
overwrite-leak: 6-10
exception-leak: 12-17
```

# Double free and use after free

{{clear: slide}}

{{show: double-free typewriter 1.5s linear}} Two more bugs. Both are undefined behavior. Both can be exploited by attackers.

{{focus: double-delete}} {{zoom: 1.2x}} {{annotate: double-delete "Free twice"}} Delete p frees the memory. Delete p again tries to free already-freed memory. Undefined behavior. The heap's bookkeeping is corrupted.

{{zoom: 1x}} {{focus: use-after}} {{zoom: 1.3x}} {{annotate: use-after "Zombie pointer"}} Use after free. You delete p, then read through it. The memory might have been given to someone else. You're reading their data. Writing through a freed pointer can corrupt arbitrary memory.

{{zoom: 1x}} {{focus: none}} These bugs are why manual memory management is dangerous. Raw new and delete are tools of last resort.

```code:double-free lang=cpp
int* p = new int(42);
delete p;
delete p;       // UNDEFINED BEHAVIOR: double free

int* q = new int(99);
delete q;
int val = *q;   // UNDEFINED BEHAVIOR: use after free
---
double-delete: 1-3
use-after: 5-7
```

# Why raw pointers are dangerous

{{clear: slide}}

{{show: dangers grow 0.5s spring}} Five bugs, all from raw new and delete.

{{focus: leak-item}} {{annotate: leak-item "Forget delete"}} Memory leak. Allocate, forget to free.

{{focus: double-item}} {{annotate: double-item "Delete twice"}} Double free. Free the same memory twice.

{{focus: uaf-item}} {{annotate: uaf-item "Use dead memory"}} Use after free. Access freed memory.

{{focus: dangle-item}} {{annotate: dangle-item "Stale pointer"}} Dangling pointer. Pointer outlives the object.

{{focus: mismatch-item}} {{annotate: mismatch-item "Wrong delete"}} Mismatch. new with delete bracket, or new bracket with delete.

{{focus: none}} Five ways to crash or corrupt your program. All preventable. Next lesson: smart pointers make every one of these impossible.

```data:dangers type=array
["Memory leak", "Double free", "Use after free", "Dangling pointer", "new/delete mismatch"]
---
leak-item: 0
double-item: 1
uaf-item: 2
dangle-item: 3
mismatch-item: 4
```
