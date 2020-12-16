include(CheckCSourceCompiles)
foreach(KEYWORD "inline" "__inline__" "__inline")
  if(NOT INLINE_KEYWORD)
    set(CMAKE_REQUIRED_DEFINITIONS "-DTESTKEYWORD=${KEYWORD}")
    check_c_source_compiles(
      "typedef int foo_t;
static TESTKEYWORD foo_t static_foo(){return 0;}
foo_t foo(){return 0;}
int main(int argc, char *argv[]){return 0;}"
      HAVE_${KEYWORD})
    if(HAVE_${KEYWORD})
      set(INLINE_KEYWORD ${KEYWORD})
    endif(HAVE_${KEYWORD})
  endif(NOT INLINE_KEYWORD)
endforeach(KEYWORD)
