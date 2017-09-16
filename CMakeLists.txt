cmake_minimum_required(VERSION 3.8)
project(gbemu)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
set(SOURCE_FILES main.c)
add_executable(gbemu
    ${SOURCE_FILES}
    include/cpu.h
    include/memory.h
    include/registers.h
)