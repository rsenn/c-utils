# include_directories("${CMAKE_SOURCE_DIR}")
include_directories(${CMAKE_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

link_directories(${CMAKE_BINARY_DIR})

if(OVERRIDE_DEPS)
  link_libraries(${OVERRIDE_DEPS})
else(OVERRIDE_DEPS)
  link_libraries(cpp hashmap iarray strlist errmsg path stralloc mmap str taia tai buffer open uint32)
endif(OVERRIDE_DEPS)

file(GLOB TESTS test_*.c)

add_executable(all_tests all_tests.c unit_test_runner.c unit_test.h unit_test.c ${TESTS})
