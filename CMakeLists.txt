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
add_compile_options(-Wall -Wextra -Werror -g) # -fsanitize=address



# Recursively find all .cpp files
file(GLOB_RECURSE SRC
${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Library
add_library(ftpp STATIC ${SRC})
target_include_directories(ftpp PUBLIC ${PROJECT_SOURCE_DIR}/includes)

# ---- Executables ----

# Main
add_executable(program main.cpp)
target_link_libraries(program PRIVATE ftpp atomic)

# Server binary
add_executable(server main_server.cpp)
target_link_libraries(server PRIVATE ftpp atomic)

# Client binary
add_executable(client main_client.cpp)
target_link_libraries(client PRIVATE ftpp atomic)
