#include_directories("${CMAKE_SOURCE_DIR}")
include_directories(${CMAKE_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

link_directories(${CMAKE_BINARY_DIR})

link_libraries(strlist path taia tai buffer mmap open uint32)

file(GLOB TESTS test_*.c)

add_executable(all_tests unit_test_runner.c unit_test.h unit_test.c ${TESTS})
