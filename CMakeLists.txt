cmake_minimum_required(VERSION 3.0)
project(avx_mean)

set(CMAKE_CXX_STANDARD 20)
add_compile_options("-mavx")
add_executable(avx_mean main.cpp)


