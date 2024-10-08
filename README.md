# Hydrogen

Hydrogen is a simple, educational programming language implemented in C++. Designed to help beginners understand the fundamentals of compiler construction, Hydrogen covers key concepts such as tokenization, parsing, and code generation. This project demonstrates how to build a basic compiler that translates Hydrogen code into assembly, assembles it, and links it into an executable. It is made for Mac ARM64 architecture.

## Table of Contents
- [Features](#features)
- [Language Syntax](#language-syntax)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Usage](#usage)
  - [Writing Hydrogen Code](#writing-hydrogen-code)
  - [Compiling Hydrogen Code](#compiling-hydrogen-code)
- [Project Structure](#project-structure)
- [Example](#example)
- [Building from Source](#building-from-source)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgements](#acknowledgements)

## Features
- **Simple Syntax:** Easy-to-read and write syntax suitable for learning compiler basics.
- **Compiler Components:**
  - **Tokenizer:** Breaks down the source code into tokens. 
  - **Parser:** Constructs an Abstract Syntax Tree (AST) from tokens. 
  - **Generator:** Translates AST into assembly code.
- **CMake Integration**: Simplifies the build process across different platforms.
- **Example Program:** Includes a sample Hydrogen program to demonstrate functionality.

## Language Syntax
  Hydrogen is a minimalist language with a focus on understanding the compilation process. Below are the core constructs supported by Hydrogen:

### Variable Declaration
Declare variables using the let keyword followed by an identifier, an equals sign, and an integer literal.

```hydrogen
let x = 10;
let y = 20;
```
### Exit Statement
Terminate the program and optionally return a value using the exit keyword.

```hydrogen
exit(x);
```
### Complete Example
```hydrogen
let a = 5;
let b = 15;
let c = 25;
exit(c);
```

## Getting Started
### Prerequisites
- **C++ Compiler:** Ensure you have a C++23 compatible compiler installed (e.g., clang, gcc).
- **CMake:** Version 3.26 or higher.
- **Make:** For building the project (on Unix-like systems).
### Installation
1. Clone the Repository:
    ```bash
    git clone https://github.com/yourusername/hydrogen.git
    cd hydrogen
    ```
2. Build the Project:
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```
    This will generate the executable Hydrogen in the build directory.

## Usage
### Writing Hydrogen Code
Create a Hydrogen source file with the .hy extension. For example, program.hy:
```hydrogen
let x = 10;
let y = 20;
let z = 30;
exit(z);
```

### Compiling Hydrogen Code
Use the compiled Hydrogen executable to compile your Hydrogen source code.

```bash
./Hydrogen path/to/your/program.hy
```
This process will:
1. **Tokenize:** Convert the source code into tokens.
2. **Parse:** Build an Abstract Syntax Tree (AST) from the tokens.
3. **Generate Assembly:** Translate the AST into ARM64 assembly code (out.asm).
4. **Assemble and Link:** Assemble out.asm into an object file (out.o) and link it into an executable (out).

### Running the Executable
After compilation, run the generated executable:

```bash
./out
```
The exit code will correspond to the value provided in the exit statement.

```bash
echo $?
# Output: 30
```
### Project Structure
```css
hydrogen/
├── src/
│   ├── main.cpp
│   ├── tokenization.h
│   ├── parser.h
│   └── generation.h
├── test.hy
├── out.asm
├── CMakeLists.txt
└── README.md
```
- **src/main.cpp:** Entry point of the compiler.
- **src/tokenization.h:** Defines the tokenizer component.
- **src/parser.h:** Defines the parser and AST structures.
- **src/generation.h:** Defines the code generator.
- **test.hy:** Sample Hydrogen program.
- **out.asm:** Generated assembly code from test.hy.
- **CMakeLists.txt:** CMake build configuration.
- **README.md:** Project documentation.
## Example
### Sample Hydrogen Program (```test.hy```)
```hydrogen
let variable = 25;
let y = 15;
let z = 30;
exit(variable);
```
### Generated Assembly (```out.asm```)
```assembly
.global _main
.align 2
_main:
    sub sp, sp, #16
    mov w16, 25
    str w16, [sp, #0]
    mov w16, 15
    str w16, [sp, #8]
    mov w16, 30
    str w16, [sp, #16]
    ldr w16, [sp, #0]
    str w16, [sp, #24]
    mov w16, 1
    ldr w0, [sp, #24]
    add sp, sp, #24
    svc 0
    add sp, sp, #16
    mov w16, 1
    mov w0, 0
    svc 0
```
### Explanation
1. **Variable Allocation:**
    - Allocates space on the stack for variables ```variable```, ```y```, and ```z```.
    - Moves the integer literals into registers and stores them on the stack.
2. **Exit Statement:**
   - Loads the value of ```variable``` from the stack.
   - Performs a system call to exit the program, returning the value of ```variable```.

## Building from Source
Ensure you have the prerequisites installed, then follow these steps:

1. **Clone the Repository:**
    ```bash
    git clone https://github.com/yourusername/hydrogen.git
    cd hydrogen
    ```
2. **Create Build Directory and Navigate:**
    ```bash
    mkdir build
    cd build
    ```
3. **Run CMake to Configure the Project:**
    ```bash
    cmake ..
    ```
4. **Build the Project:**
    ```bash
    make
    ```
5. **Run the Compiler:**
    ```bash
    ./Hydrogen ../test.hy
    ```
    This will generate out.asm, out.o, and the executable out.

## Contributing
Contributions are welcome! Whether you're reporting bugs, suggesting features, or submitting pull requests, your help is appreciated. Please ensure your code adheres to the project's coding standards and includes appropriate tests.

## License
This project is licensed under the MIT License.

## Acknowledgements
Inspired by educational compiler projects and resources.
Utilizes CMake for build configuration.
Thanks to the open-source community for providing invaluable resources and support.
