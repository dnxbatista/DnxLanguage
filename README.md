# DnxLanguage

DnxLanguage is a simple programming language implemented in C++.

The compiler reads `.dnx` files, generates C++ code, compiles it with `g++`, and runs the result.

## Requirements

- CMake 3.16 or newer
- C++17 compiler
- `g++`

## Build

```bash
cmake -S . -B build
cmake --build build
```

## Run
On Windows
```bash
.\dnxlang.exe .\main.dnx
```

On Linux or macOS:
```bash
./build/dnxlang main.dnx
```

## Options
- ```-l``` keeps generated files in the local folder.
- ```-s``` logs the compilation steps.
- ```-ls``` enables both options.

Example:
```bash
.\dnxlang.exe .\main.dnx -s
```

## Dnx Code Example
```
# This is a comment
string message = "Hello World";
print message;
```
A complete example is available in example/main.dnx