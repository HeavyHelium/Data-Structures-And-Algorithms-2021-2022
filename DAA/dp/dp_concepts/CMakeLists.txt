cmake_minimum_required(VERSION 3.21)
project(dp_concepts)

set(CMAKE_CXX_STANDARD 20)

add_executable(kadane kadane.cpp)
add_executable(longest_increasing_subsequence longest_increasing.cpp)
add_executable(coins_change coins_change.cpp)
