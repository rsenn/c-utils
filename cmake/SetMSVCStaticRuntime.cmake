set(CompilerFlags CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_RELWITHDEBINFO
                  CMAKE_C_FLAGS_MINSIZEREL)
foreach(CompilerFlag ${CompilerFlags})
  unset(FLAG)
  string(REPLACE "/MD" "/MT" FLAG "${${CompilerFlag}}")
  if("${CompilerFlag}" STREQUAL "CMAKE_C_FLAGS_DEBUG")
    set(FLAG "${FLAG} -DDEBUG=1")
  endif()
  unset(${CompilerFlag} CACHE)
  set(${CompilerFlag} "${FLAG}" CACHE STRING "Compiler flags ${CompilerFlag}")
  message("${CompilerFlag}: ${${CompilerFlag}}")
endforeach()
