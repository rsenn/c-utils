function(check_flag_and_add FLAG VAR)
  set(RESULT)
  check_c_compiler_flag("${FLAG}" RESULT)
  if(RESULT)
    foreach(ARG ${ARGN})
      # message("Adding '${FLAG}' to ${ARG}")
      set("${ARG}" ${${ARG}} ${FLAG})
    endforeach(ARG ${ARGN})
    set("${VAR}" "${FLAG}" PARENT_SCOPE)
  else(RESULT)
    unset("${VAR}" PARENT_SCOPE)
  endif(RESULT)
  foreach(ARG ${ARGN})
    # message("${ARG} = ${${ARG}}")
    set("${ARG}" "${${ARG}}" PARENT_SCOPE)
  endforeach(ARG ${ARGN})
endfunction(check_flag_and_add FLAG VAR)

macro(check_enable_auto_import_flag VAR)
  if(NOT DEFINED ${VAR})
    check_flag_and_add("-Wl,--enable-auto-import" "${VAR}" AUTO_IMPORT_FLAG)
    set("${VAR}" "${AUTO_IMPORT_FLAG}" CACHE STRING "Linker flag for automatic importing from DLL")
    foreach(ARG CMAKE_EXE_LINKER_FLAGS CMAKE_MODULE_LINKER_FLAGS CMAKE_SHARED_LINKER_FLAGS)
      set("${ARG}" "${${ARG}} ${AUTO_IMPORT_FLAG}")
    endforeach()
  endif(NOT DEFINED ${VAR})
endmacro(check_enable_auto_import_flag VAR)

macro(check_msys_cygwin_mingw)
  file(
    WRITE
    "${CMAKE_BINARY_DIR}/cmake/try-msys.c  " # include <stdio.h>\n\n#ifndef __MSYS__\n#error \"Not MSYS\"\n#endif\n\nint\nmain() {\n  printf(\"OK\\n\");\n  return 0;\n}\nint WINAPI\nWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) \n__attribute__((stdcall)) \n{}\n"
    )
  file(
    WRITE "${CMAKE_BINARY_DIR}/cmake/try-mingw.c"
    "#include <stdio.h>\n\n#ifndef __MINGW32__\n#ifndef __MINGW64__\n#error \"Not MINGW\"\n#endif\n#endif\n\nint main() {\n\tprintf(\"OK\\n\");\n\treturn 0;\n}\n"
  )
  file(
    WRITE "${CMAKE_BINARY_DIR}/cmake/try-cygwin.c"
    "#include <stdio.h>\n\n#ifndef __CYGWIN__\n#error \"Not CYGWIN\"\n#endif\n\nint main() {\n\tprintf(\"OK\\n\");\n\treturn 0;\n}\n"
  )
  set(SYS "")

  unset(MSYS CACHE)
  unset(CYGWIN CACHE)
  unset(MINGW CACHE)
  unset(SYS CACHE)

  exec_program("${CMAKE_C_COMPILER}" "${CMAKE_BINARY_DIR}/cmake" ARGS -c ${CMAKE_BINARY_DIR}/cmake/try-msys.c
               OUTPUT_VARIABLE OUTPUT RETURN_VALUE TRY_MSYS)
  if(TRY_MSYS STREQUAL 0)
    set(MSYS TRUE CACHE BOOL "System is Msys")
    set(SYS msys)
  else(TRY_MSYS STREQUAL 0)
    exec_program("${CMAKE_C_COMPILER}" "${CMAKE_BINARY_DIR}/cmake" ARGS -c ${CMAKE_BINARY_DIR}/cmake/try-cygwin.c
                 OUTPUT_VARIABLE OUTPUT RETURN_VALUE TRY_CYGWIN)
    if(TRY_CYGWIN STREQUAL 0)
      set(CYGWIN TRUE CACHE BOOL "System is Cygwin")
      set(SYS cygwin)
    else(TRY_CYGWIN STREQUAL 0)
      exec_program("${CMAKE_C_COMPILER}" "${CMAKE_BINARY_DIR}/cmake" ARGS -c ${CMAKE_BINARY_DIR}/cmake/try-mingw.c
                   OUTPUT_VARIABLE OUTPUT RETURN_VALUE TRY_MINGW)
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

  # dump(MSYS CYGWIN MINGW)

  if(WIN32 OR MINGW)
    set(NATIVE TRUE)
  endif(WIN32 OR MINGW)

  set(WINDOWS_NATIVE "${NATIVE}")
  set(WINDOWS_NATIVE "${NATIVE}" CACHE BOOL "Native windows (msvcrt or mingw)")

  if(WIN32 OR CYGWIN OR MSYS OR MINGW)
    set(WINDOWS TRUE)
  endif(WIN32 OR CYGWIN OR MSYS OR MINGW)

  set(WINDOWS "${WINDOWS}")
  set(WINDOWS "${WINDOWS}" CACHE BOOL "Any windows (native/msys/cygwin)")

  set(SYSTEM "${SYSTEM}")

  if(WINDOWS)
    if("${SYSTEM}" STREQUAL "cygwin")
      add_definitions(-D__CRT__NO_INLINE)
    else("${SYSTEM}" STREQUAL "cygwin")
      check_enable_auto_import_flag(AUTO_IMPORT)
    endif("${SYSTEM}" STREQUAL "cygwin")

    set(LIBSHLWAPI shlwapi CACHE STRING "Windows API shlwapi library")
    set(LIBIPHLPAPI iphlpapi CACHE STRING "Windows API iphlpapi library")
    set(LIBWSOCK32 shlwapi CACHE STRING "Windows API wsock32 library")
  else(WINDOWS)
    unset(LIBSHLWAPI CACHE)
    unset(LIBIPHLPAPI CACHE)
    unset(LIBWSOCK32 CACHE)
  endif(WINDOWS)

endmacro(check_msys_cygwin_mingw)
