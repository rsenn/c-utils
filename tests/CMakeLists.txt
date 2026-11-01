# include_directories("${CMAKE_SOURCE_DIR}")
include_directories(${CMAKE_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

link_directories(${CMAKE_BINARY_DIR})

if(OVERRIDE_DEPS)
  link_libraries(${OVERRIDE_DEPS})
else(OVERRIDE_DEPS)
  link_libraries(cpp ini sse utf8 range slist dlist cb cbmap cdb critbit set elf pe uri textcode binfmt rdir xml tokenizer json charbuf ihex http hmap hashmap iarray strlist errmsg path stralloc mmap str taia tai buffer open uint32 fmt iob io iom
                  scan unix socket env dir safemult wait uint64 uint16 seek ndelay process)
endif(OVERRIDE_DEPS)

file(GLOB TESTS test_*.c)

string(REGEX REPLACE "([^ ;]*/tests/test_|\\.c)" "" TEST_NAMES "${TESTS}")

#message("Tests: ${TEST_NAMES}")
#foreach(TEST ${TEST_NAMES})
#  add_test(NAME test_${TEST} COMMAND test_${TEST})
#endforeach()

add_executable(all_tests all_tests.c unit_test_runner.c unit_test.h unit_test.c ${TESTS})
target_compile_definitions(all_tests PRIVATE ${COMPRESS_LIBRARY_DEFINES})
target_link_libraries(all_tests ${ZLIB_LIBRARY} ${COMPRESSION_LIBRARIES})

if(DO_TESTS)
  include(CTest)
  add_test(NAME all_tests COMMAND all_tests WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    #BUILD_DEPENDS all_tests
    )
endif()

