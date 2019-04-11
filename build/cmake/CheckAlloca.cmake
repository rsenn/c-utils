include(CheckIncludeFile)
message("Checking for alloca")

file(WRITE "${CMAKE_BINARY_DIR}/try_alloca.c" "#include <alloca.h>
int main() {
  char* x = alloca(1024);
}")
try_compile(HAVE_ALLOCA "${CMAKE_BINARY_DIR}" SOURCES "${CMAKE_BINARY_DIR}/try_alloca.c")

if(HAVE_ALLOCA)
  message("Checking for alloca - found")
else(HAVE_ALLOCA)
  message("Checking for alloca - not found")
endif(HAVE_ALLOCA)

check_include_file(alloca.h HAVE_ALLOCA_H)

if(HAVE_ALLOCA AND HAVE_ALLOCA_H)
  add_definitions(-DHAVE_ALLOCA=1)
endif(HAVE_ALLOCA AND HAVE_ALLOCA_H)


