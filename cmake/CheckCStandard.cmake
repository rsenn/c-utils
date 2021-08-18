macro(check_c_standard)
  foreach(C_STANDARD c2x c18 c17 c1x c11 c99)
    string(REPLACE "c" "CPLUSPLUS" C_STANDARD_NUM "${C_STANDARD}")
    set(CMAKE_REQUIRED_QUIET TRUE)
    check_c_compiler_flag("-std=${C_STANDARD}" C_STANDARD_${C_STANDARD_NUM})
    set(CMAKE_REQUIRED_QUIET FALSE)
    if(C_STANDARD_${C_STANDARD_NUM})
      set(C_STANDARD_VALUE "${C_STANDARD}" CACHE STRING "C standard")
      set(C_STANDARD_FLAG "-std=${C_STANDARD}" CACHE STRING "C standard argument")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_STANDARD_FLAG}")
      break()
    endif(C_STANDARD_${C_STANDARD_NUM})
  endforeach(C_STANDARD c2x c18 c17 c1x c11 c99)

  message(STATUS "C standard: ${C_STANDARD_VALUE}")
endmacro(check_c_standard)