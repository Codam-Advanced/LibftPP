cmake_minimum_required(VERSION 3.16)

project(program)

# compiler options for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use your compiler
set(CMAKE_CXX_COMPILER /usr/bin/g++-12)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags (like your Makefile)
add_compile_options(-Wall -Wextra -mcx16 -g -fsanitize=thread) # -fsanitize=address
add_link_options(-fsanitize=thread)



# Recursively find all .cpp files
file(GLOB_RECURSE SRC
${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Create static library (libftpp.a)
add_library(ftpp STATIC ${SRC})

target_include_directories(ftpp PUBLIC ${PROJECT_SOURCE_DIR}/includes)

# Main executable
add_executable(program main.cpp)

# Link library to executable
target_link_libraries(program PRIVATE ftpp atomic)

