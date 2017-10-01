cmake_minimum_required(VERSION 3.8)
project(gbemu)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

find_package(Check REQUIRED)
message(WARNING ${CHECK_FOUND})
message(WARNING ${CHECK_INCLUDE_DIRS})
message(WARNING ${CHECK_LIBRARIES})

enable_testing()

include_directories(include ${CHECK_INCLUDE_DIRS})

set(LIBS ${LIBS} ${CHECK_LIBRARIES} rt m c pthread subunit)

set(SOURCE_FILES main.c
    include/cpu.h
    include/memory.h
    include/registers.h
    src/cpu.c
    src/memory.c
    src/registers.c)

add_executable(gbemu ${SOURCE_FILES})

target_link_libraries(gbemu ${LIBS})
