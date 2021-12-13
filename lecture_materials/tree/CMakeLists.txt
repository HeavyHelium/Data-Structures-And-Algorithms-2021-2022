cmake_minimum_required(VERSION 3.21)
project(tree)

set(CMAKE_CXX_STANDARD 17)

add_executable(tree main.cpp cmake-build-debug/main.cpp cmake-build-debug/binary_tree.h)
