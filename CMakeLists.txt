cmake_minimum_required(VERSION 3.8)
project(gbemu)

set(CMAKE_CXX_STANDARD 11)

enable_testing()
add_subdirectory(libgbemu)
add_subdirectory(tests)
