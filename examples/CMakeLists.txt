#include_directories("${CMAKE_SOURCE_DIR}")
include_directories(${CMAKE_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

link_directories(${CMAKE_BINARY_DIR})

link_libraries(strlist errmsg path stralloc mmap str taia tai buffer open uint32)

set(EXAMPLES
  array buffer_1 buffer_fromsa buffer_mmap buffer_tosa byte_copy cas dnsip fmt fmt_httpdate fmt_human fmt_ip6 fmt_long fmt_longlong fmt_strm_alloca io io2 io5 json marshal mult netstring protobuf proxy range readhttp scan scan_long stralloc_buffer stralloc_chomp uint utf8
  )
foreach(EXAMPLE ${EXAMPLES})
  message("Example: ${EXAMPLE}")
  add_executable("example_${EXAMPLE}" "${EXAMPLE}.c")
endforeach(EXAMPLE ${EXAMPLES})
