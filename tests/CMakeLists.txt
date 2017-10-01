find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})

set(LIBS ${LIBS} ${CHECK_LIBRARIES} rt m c pthread subunit gbemulib)
enable_testing()

include_directories(. ../libgbemu/include)
add_executable(tests tests.c)
target_link_libraries(tests ${LIBS})
