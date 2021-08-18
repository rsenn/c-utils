macro(check_msys_cygwin_mingw)
  file(WRITE "${CMAKE_BINARY_DIR}/cmake/try-msys.c  " # include <stdio.h>\n\n#ifndef __MSYS__\n#error \"Not MSYS\"\n#endif\n\nint\nmain() {\n  printf(\"OK\\n\");\n  return 0;\n}\nint WINAPI\nWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) \n__attribute__((stdcall)) \n{}\n"
       )
  file(WRITE "${CMAKE_BINARY_DIR}/cmake/try-mingw.c" "#include <stdio.h>\n\n#ifndef __MINGW32__\n#ifndef __MINGW64__\n#error \"Not MINGW\"\n#endif\n#endif\n\nint main() {\n\tprintf(\"OK\\n\");\n\treturn 0;\n}\n")
  file(WRITE "${CMAKE_BINARY_DIR}/cmake/try-cygwin.c" "#include <stdio.h>\n\n#ifndef __CYGWIN__\n#error \"Not CYGWIN\"\n#endif\n\nint main() {\n\tprintf(\"OK\\n\");\n\treturn 0;\n}\n")
  set(SYS "")

  # try_compile(TRY_MSYS "${CMAKE_BINARY_DIR}/cmake" ${CMAKE_BINARY_DIR}/cmake/try-msys.c COMPILE_DEFINITIONS "-c" OUTPUT_VARIABLE OUTPUT)

  exec_program("${CMAKE_C_COMPILER}" ARGS -c ${CMAKE_BINARY_DIR}/cmake/try-msys.c OUTPUT_VARIABLE OUTPUT RETURN_VALUE TRY_MSYS)
  message("OUTPUT: ${OUTPUT}")
  message("TRY_MSYS: ${TRY_MSYS}")

  unset(MSYS CACHE)
  unset(CYGWIN CACHE)
  unset(MINGW CACHE)
  unset(SYS CACHE)

  if(TRY_MSYS STREQUAL 0)
    set(MSYS TRUE CACHE BOOL "System is Msys")
    set(SYS msys)
  else(TRY_MSYS STREQUAL 0)
    exec_program("${CMAKE_C_COMPILER}" ARGS -c ${CMAKE_BINARY_DIR}/cmake/try-cygwin.c OUTPUT_VARIABLE OUTPUT RETURN_VALUE TRY_CYGWIN)
    if(TRY_CYGWIN STREQUAL 0)
      set(CYGWIN TRUE CACHE BOOL "System is Cygwin")
      set(SYS cygwin)
    else(TRY_CYGWIN STREQUAL 0)
      exec_program("${CMAKE_C_COMPILER}" ARGS -c ${CMAKE_BINARY_DIR}/cmake/try-mingw.c OUTPUT_VARIABLE OUTPUT RETURN_VALUE TRY_MINGW)
      if(TRY_MINGW STREQUAL 0)
        set(MINGW TRUE CACHE BOOL "System is Cygwin")
        set(SYS mingw)
      else(TRY_MINGW STREQUAL 0)

      endif(TRY_MINGW STREQUAL 0)
    endif(TRY_CYGWIN STREQUAL 0)
  endif(TRY_MSYS STREQUAL 0)

  if(NOT "${SYS}" STREQUAL "")
    set(SYSTEM "${SYS}" CACHE STRING "Host system")
    message(STATUS "System detected: ${SYSTEM}")
  endif(NOT "${SYS}" STREQUAL "")

  dump(MSYS CYGWIN MINGW)

  set(SYSTEM "${SYSTEM}" PARENT_SCOPE)
  try_compile(TRY_MINGW "${CMAKE_BINARY_DIR}/cmake" ${CMAKE_BINARY_DIR}/cmake/try-mingw.c)
endmacro(check_msys_cygwin_mingw)
