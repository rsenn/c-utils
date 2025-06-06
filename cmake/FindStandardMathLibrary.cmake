# * Try to find how to link to the standard math library, if anything at all is needed to do. On most platforms this is automatic, but for example it's not automatic on QNX.
#
# Once done this will define
#
# LIBMATH_FOUND - we found how to successfully link to the standard math library LIBMATH - the name of the standard library that one has to link to. -- this will be left empty if it's automatic (most platforms). -- this will be set to "m" on platforms where one must explicitly pass the "-lm" linker flag.
#
# Copyright (c) 2010 Benoit Jacob <jacob.benoit.1@gmail.com> Redistribution and use is allowed according to the terms of the 2-clause BSD license.

include(CheckCSourceCompiles)

# a little test program for c++ math functions. notice the std:: is required on some platforms such as QNX

set(find_standard_math_library_test_program "#include <math.h>
int main() { sin(0.0); log(0.0f); }")

# first try compiling/linking the test program without any linker flags

set(CMAKE_REQUIRED_FLAGS "")
set(CMAKE_REQUIRED_LIBRARIES "")
check_c_source_compiles("${find_standard_math_library_test_program}" standard_math_library_linked_to_automatically)

if(NOT DEFINED CACHE{LIBMATH})
  if(standard_math_library_linked_to_automatically)

    # the test program linked successfully without any linker flag.
    set(LIBMATH "" CACHE STRING "C standard math library")
    set(LIBMATH_FOUND TRUE)

  else()
    # the test program did not link successfully without any linker flag. This is a very uncommon case that so far we only saw on QNX. The next try is the standard name 'm' for the standard math library.

    set(CMAKE_REQUIRED_LIBRARIES "m")
    check_c_source_compiles("${find_standard_math_library_test_program}" standard_math_library_linked_to_as_m)

    if(standard_math_library_linked_to_as_m)

      # the test program linked successfully when linking to the 'm' library
      set(LIBMATH "m" CACHE STRING "C standard math library")
      set(LIBMATH_FOUND TRUE)

    else()

      # the test program still doesn't link successfully
      set(LIBMATH_FOUND FALSE)

    endif()
  endif()
endif(NOT DEFINED CACHE{LIBMATH})
