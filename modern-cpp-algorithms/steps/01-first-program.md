---
title: Your first C++ program
---

# Where C++ lives

{{show: domains grow 0.5s spring}} C++ is everywhere you don't see it. {{focus: games-node}} {{annotate: games-node "Unreal Engine"}} Game engines. {{focus: os-node}} {{annotate: os-node "Windows, Linux"}} Operating systems. {{focus: hft-node}} {{annotate: hft-node "Nanoseconds matter"}} High-frequency trading. {{focus: embedded-node}} {{annotate: embedded-node "Mars rovers"}} Embedded systems.

{{focus: none}} Why? One reason. {{zoom: 1.1x}} You control exactly what the machine does. No garbage collector. No runtime. You talk to the hardware, and it listens.

{{zoom: 1x}} This course takes you from zero to interview-ready. C++ from first principles, the full standard library, and every major algorithm pattern. Let's start.

```diagram:domains
games [service]
os [service]
hft [service]
embedded [service]
cpp [service]
cpp --powers--> games
cpp --powers--> os
cpp --powers--> hft
cpp --powers--> embedded
{C++ Powers: games, os, hft, embedded}
---
games-node: games
os-node: os
hft-node: hft
embedded-node: embedded
```

# The compilation model

{{clear: slide}}

{{show: pipeline grow 0.5s spring}} You don't run C++ code directly. You compile it. The compiler turns your text file into a binary the operating system can execute.

{{focus: source-step}} {{annotate: source-step "Your code"}} You write source code. A plain text file ending in dot cpp.

{{focus: preprocess-step}} {{annotate: preprocess-step "Copy-paste machine"}} The preprocessor runs first. It handles include directives — literally copy-pasting header files into your source. No magic.

{{focus: compile-step}} {{annotate: compile-step "Text to machine code"}} The compiler reads the preprocessed text and produces an object file. Machine instructions, but not yet a runnable program.

{{focus: link-step}} {{annotate: link-step "Assembly"}} The linker takes your object file, combines it with library code, and produces the final executable binary.

{{focus: binary-step}} {{annotate: binary-step "Run this"}} That binary is what you run. No interpreter. No virtual machine. Native code, on your processor.

{{focus: none}} Four stages. Source, preprocess, compile, link. Every C++ build follows this path.

```diagram:pipeline
source [service]
preprocessor [service]
compiler [service]
linker [service]
binary [service]
source --#include--> preprocessor
preprocessor --expanded--> compiler
compiler --.o file--> linker
linker --executable--> binary
{Build Pipeline: source, preprocessor, compiler, linker, binary}
---
source-step: source
preprocess-step: preprocessor
compile-step: compiler
link-step: linker
binary-step: binary
```

# Hello world

{{clear: slide}}

{{show: hello typewriter 1.5s linear}} Here's the smallest useful C++ program.

{{focus: include-line}} {{zoom: 1.3x}} {{annotate: include-line "Pull in I/O"}} The include directive. It brings in the iostream header — the standard library's input/output machinery. The angle brackets mean "look in the system include path."

{{zoom: 1x}} {{focus: main-fn}} {{zoom: 1.2x}} {{annotate: main-fn "Entry point"}} Every C++ program starts at main. The int return type tells the operating system whether the program succeeded. Zero means success.

{{zoom: 1x}} {{focus: cout-line}} {{zoom: 1.3x}} {{annotate: cout-line "Standard output"}} std colon colon cout is the standard output stream. The double less-than operator pushes data into it. The endl flushes the buffer and adds a newline.

{{zoom: 1x}} {{focus: return-line}} {{annotate: return-line "Success"}} Return zero. The program ran. Nothing went wrong. The operating system knows.

{{zoom: 1x}} {{focus: none}} Five lines. That's a complete program.

```code:hello lang=cpp
#include <iostream>

int main() {
    std::cout << "Hello, world!" << std::endl;
    return 0;
}
---
include-line: 1
main-fn: 3-6
cout-line: 4
return-line: 5
```

# Compiling and running

{{clear: slide}}

{{show: compile-cmd typewriter 1s linear}} To compile, you invoke the compiler from the terminal.

{{focus: compiler-name}} {{zoom: 1.3x}} {{annotate: compiler-name "GNU or Clang"}} g plus plus is the GNU compiler. clang plus plus is the LLVM compiler. Both work. Pick one.

{{zoom: 1x}} {{focus: std-flag}} {{zoom: 1.2x}} {{annotate: std-flag "Use modern C++"}} The standard flag tells the compiler which version of C++ to use. We use C++ 23. Always specify this. Without it, you get an older default.

{{zoom: 1x}} {{focus: warn-flags}} {{zoom: 1.2x}} {{annotate: warn-flags "Catch mistakes"}} Wall and W extra turn on warnings. The compiler will tell you about suspicious code — unused variables, implicit conversions, missing return statements. Always use these.

{{zoom: 1x}} {{focus: output-flag}} {{annotate: output-flag "Name the binary"}} The o flag names the output binary. Without it, you get a file called a dot out.

{{zoom: 1x}} {{focus: source-file}} {{annotate: source-file "Your code"}} The source file. The thing you wrote.

{{zoom: 1x}} {{focus: none}} One command. Source in, binary out. Run it with dot slash hello.

{{clear: slide}}

{{show: run-cmd typewriter 0.5s linear}} Run the binary. It prints the message to the terminal. That's it. You wrote C++, compiled it, and ran it.

```code:compile-cmd lang=bash
g++ -std=c++23 -Wall -Wextra -o hello hello.cpp
---
compiler-name: 1
std-flag: 1
warn-flags: 1
output-flag: 1
source-file: 1
```

```code:run-cmd lang=bash
$ ./hello
Hello, world!
---
output: 2
```

# Installing the toolchain

{{clear: slide}}

{{show: install-macos typewriter 1.5s linear}} Three platforms, three setups. Let's get the tools installed.

{{focus: xcode-line}} {{zoom: 1.2x}} {{annotate: xcode-line "Apple's toolchain"}} On macOS, xcode select install gives you clang plus plus and the standard library. One command.

{{zoom: 1x}} {{focus: cmake-line}} {{zoom: 1.2x}} {{annotate: cmake-line "Build system"}} CMake is a build system generator. It writes the actual build commands for you. We'll use it from lesson two onward. Install it through Homebrew.

{{zoom: 1x}} {{focus: none}} That's macOS. Two commands and you're ready.

{{clear: slide}}

{{show: install-linux typewriter 1s linear}} On Linux, build essential gives you g plus plus. {{focus: apt-line}} {{zoom: 1.2x}} {{annotate: apt-line "GCC + tools"}} One apt install.

{{zoom: 1x}} {{focus: cmake-linux}} {{annotate: cmake-linux "Same CMake"}} CMake installs the same way.

{{zoom: 1x}} {{focus: none}} Linux is done.

{{clear: slide}}

{{show: install-windows typewriter 1s linear}} On Windows, install the MSVC Build Tools from Visual Studio. {{focus: msvc-line}} {{zoom: 1.2x}} {{annotate: msvc-line "Microsoft's compiler"}} The free Build Tools package includes the compiler and standard library.

{{zoom: 1x}} {{focus: cmake-windows}} {{annotate: cmake-windows "CMake for Windows"}} CMake installs from its website or through winget.

{{zoom: 1x}} {{focus: none}} Every platform has a compiler and CMake. That's all you need. Open your editor, and let's write some code.

```code:install-macos lang=bash
# macOS
xcode-select --install
brew install cmake
---
xcode-line: 2
cmake-line: 3
```

```code:install-linux lang=bash
# Linux (Debian/Ubuntu)
sudo apt install build-essential
sudo apt install cmake
---
apt-line: 2
cmake-linux: 3
```

```code:install-windows lang=bash
# Windows
# Install "Build Tools for Visual Studio" from visualstudio.microsoft.com
winget install Kitware.CMake
---
msvc-line: 2
cmake-windows: 3
```
