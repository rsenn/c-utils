message("Checking dynamic stack variable size")

file(
  WRITE "${CMAKE_CURRENT_BINARY_DIR}/try_dynstack.c"
  "void test_dynstack(size_t n) {\n  char x[n+1];\n  x[n] = 0;\n}\nint main(int argc, char* argv[]) {\n  test_dynstack(argc);\n}"
)
try_compile(HAVE_DYNSTACK "${CMAKE_CURRENT_BINARY_DIR}"
            SOURCES "${CMAKE_CURRENT_BINARY_DIR}/try_dynstack.c")

if(HAVE_DYNSTACK)
  message("Checking dynamic stack variable size - supported")
else(HAVE_DYNSTACK)
  message("Checking dynamic stack variable size - not supported")
endif(HAVE_DYNSTACK)

if(HAVE_DYNSTACK)
  add_definitions(-DHAVE_DYNSTACK=1)
endif(HAVE_DYNSTACK)
