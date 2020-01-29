option(WARN_WERROR "Halt the compilation with an error on compiler warnings." OFF)

include(CheckCCompilerFlag)

check_c_compiler_flag("-Wno-unused-variable" WARN_NO_UNUSED_VARIABLE)
if(WARN_NO_UNUSED_VARIABLE)
    set(WERROR_FLAG "${WERROR_FLAG} -Wno-unused-variable")
endif()
check_c_compiler_flag("-Wno-unused-function" WARN_NO_UNUSED_FUNCTION)
if(WARN_NO_UNUSED_FUNCTION)
    set(WERROR_FLAG "${WERROR_FLAG} -Wno-unused-function")
endif()

#message(INFO "Compiler Id: ${CMAKE_C_COMPILER_ID}")

if(CMAKE_C_COMPILER_ID MATCHES "GNU")
  set(PEDANTIC_COMPILE_FLAGS -pedantic-errors -Wall -Wextra -pedantic
      -Wold-style-cast -Wundef
      -Wredundant-decls -Wwrite-strings -Wpointer-arith
      -Wcast-qual -Wformat=2 -Wmissing-include-dirs
      -Wcast-align -Wnon-virtual-dtor
      -Wctor-dtor-privacy -Wdisabled-optimization
      -Winvalid-pch -Woverloaded-virtual
      -Wconversion
      -Wno-ctor-dtor-privacy -Wno-format-nonliteral -Wno-shadow)
  if(NOT CMAKE_C_COMPILER_VERSION VERSION_LESS 4.6)
      set(PEDANTIC_COMPILE_FLAGS ${PEDANTIC_COMPILE_FLAGS} -Wnoexcept
         -Wno-dangling-else -Wno-unused-local-typedefs)
  endif()
  if(NOT CMAKE_C_COMPILER_VERSION VERSION_LESS 5.0)
      set(PEDANTIC_COMPILE_FLAGS ${PEDANTIC_COMPILE_FLAGS} -Wdouble-promotion
          -Wtrampolines -Wzero-as-null-pointer-constant -Wuseless-cast
          -Wvector-operation-performance -Wsized-deallocation)
  endif()
  if(NOT CMAKE_C_COMPILER_VERSION VERSION_LESS 6.0)
      set(PEDANTIC_COMPILE_FLAGS ${PEDANTIC_COMPILE_FLAGS} -Wshift-overflow=2
          -Wnull-dereference -Wduplicated-cond)
  endif()
  set(WERROR_FLAG "${WERROR_FLAG} -Werror")
endif()

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
  set(PEDANTIC_COMPILE_FLAGS -Wall -Wextra -pedantic -Wconversion -Wno-sign-conversion)
#  check_c_compiler_flag(-Wzero-as-null-pointer-constant HAS_NULLPTR_WARNING)
#  if(HAS_NULLPTR_WARNING)
#    set(PEDANTIC_COMPILE_FLAGS ${PEDANTIC_COMPILE_FLAGS}
#        -Wzero-as-null-pointer-constant)
#  endif()
  set(WERROR_FLAG "${WERROR_FLAG} -Werror")
endif()

if(MSVC)
  set(PEDANTIC_COMPILE_FLAGS /W3)
  set(WERROR_FLAG /WX)
endif()


if(WARN_WERROR)
  #SET(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${WERROR_FLAG})
  SET(WARN_C_COMPILER_FLAGS "-Wall ${WERROR_FLAG}")
endif()
if(WARN_PEDANTIC)
  #SET(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${PEDANTIC_COMPILE_FLAGS})
  SET(WARN_C_COMPILER_FLAGS "-Wall ${PEDANTIC_COMPILE_FLAGS}")
endif()
