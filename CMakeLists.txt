cmake_minimum_required(VERSION 3.10)

# Set the project name
project(oberon_compiler)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(compiler main.cpp LexicalAnalyzer.cpp Parser.cpp SymTable.cpp TAC.cpp TempMap.cpp LiteralTable.cpp Asm.cpp)

# Include directories
target_include_directories(compiler PUBLIC "${PROJECT_BINARY_DIR}")
