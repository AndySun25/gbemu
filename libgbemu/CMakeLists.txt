include_directories(src)
include_directories(include)

set(SOURCE_FILES main.c
    include/cpu.h
    include/memory.h
    include/registers.h
    src/cpu.c
    src/memory.c
    src/registers.c
    include/timers.h
    src/timers.c)

add_executable(gbemu ${SOURCE_FILES})
add_library(gbemulib
    src/cpu.c
    src/memory.c
    src/registers.c
    include/timers.h
    src/timers.c)
