cmake_minimum_required(VERSION 3.10)

# Set the project name
project(LexicalAnalyzer)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(hw1 main.cpp LexicalAnalyzer.cpp Parser.cpp SymTable.cpp)

# Include directories
target_include_directories(hw1 PUBLIC "${PROJECT_BINARY_DIR}")
