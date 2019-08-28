cmake_minimum_required(VERSION 3.14)
project(Matrix)

set(CMAKE_CXX_STANDARD 14)

include_directories(Matrix includes)

add_executable(Matrix main.cpp includes/static_matrix.hpp)