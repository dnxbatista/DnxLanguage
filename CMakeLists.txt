cmake_minimum_required(VERSION 3.16) # Cmake minimum version required
project (dnxlang LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17) # C++ version
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler exe
add_executable(dnxlang
        src/main.cpp
        src/Lexer.cpp
        src/Parser.cpp
        src/CodeGen.cpp
)

target_include_directories(dnxlang PRIVATE # H Files
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)