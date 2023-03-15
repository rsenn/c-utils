macro(show_result RESULT_VAR)
  if(${${RESULT_VAR}})
    set(RESULT_VALUE yes)
  else(${${RESULT_VAR}})
    set(RESULT_VALUE no)
  endif(${${RESULT_VAR}})
endmacro(show_result RESULT_VAR)

macro(check_compile RESULT_VAR SOURCE)
  set(RESULT "${${RESULT_VAR}}")
  # message("${RESULT_VAR} = ${RESULT}" )
  if(RESULT STREQUAL "")
    string(
      RANDOM LENGTH 6
      ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
               C_NAME)
    string(REPLACE SUPPORT_ "" NAME "${RESULT_VAR}")
    string(REPLACE _ - NAME "${NAME}")
    string(TOLOWER "${NAME}" C_NAME)
    set(C_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/try-${C_NAME}.c")
    string(REPLACE "\\" "\\\\" SOURCE "${SOURCE}")
    file(WRITE "${C_SOURCE}" "${SOURCE}")
    message(STATUS "Trying to compile try-${C_NAME}.c ... ")
    try_compile(COMPILE_RESULT "${CMAKE_CURRENT_BINARY_DIR}" "${C_SOURCE}"
                OUTPUT_VARIABLE "OUTPUT" LINK_LIBRARIES "${ARGN}")
    # file(REMOVE "${C_SOURCE}")

    if(COMPILE_RESULT)
      message(STATUS "Trying to compile try-${C_NAME}.c ... OK")
      # add_definitions(-D${RESULT_VAR})
    else(COMPILE_RESULT)
      set(COMPILE_LOG "${CMAKE_CURRENT_BINARY_DIR}/compile-${C_NAME}.log")
      message(
        STATUS "Trying to compile try-${C_NAME}.c ... FAIL: ${COMPILE_LOG}")
      file(WRITE "${COMPILE_LOG}" "${OUTPUT}")
      string(REPLACE "\n" ";" OUTPUT "${OUTPUT}")
      list(FILTER OUTPUT INCLUDE REGEX "error")
    endif(COMPILE_RESULT)

    set("${RESULT_VAR}" "${COMPILE_RESULT}" CACHE BOOL "Support ${NAME}")
  endif(RESULT STREQUAL "")
  show_result(${RESULT_VAR})
endmacro(check_compile RESULT_VAR SOURCE)

macro(check_run RESULT_VAR SOURCE)
  string(
    RANDOM LENGTH 6
    ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
             C_SOURCE)
  set(C_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/try-${C_SOURCE}.c")
  string(REPLACE "\\" "\\\\" SOURCE "${SOURCE}")
  file(WRITE "${C_SOURCE}" "${SOURCE}")
  try_run(RUN_RESULT COMPILE_RESULT "${CMAKE_CURRENT_BINARY_DIR}" "${C_SOURCE}"
          RUN_OUTPUT_VARIABLE RUN_OUTPUT COMPILE_OUTPUT_VARIABLE COMPILE_OUTPUT)

  if("${RUN_RESULT}" STREQUAL 0)
    set(RUN_OK TRUE)
  else("${RUN_RESULT}" STREQUAL 0)
    set(RUN_OK FALSE)
  endif("${RUN_RESULT}" STREQUAL 0)

  # message("Run result: ${RUN_OK} output: ${RUN_OUTPUT}") message("Compile result: ${COMPILE_RESULT}")

  file(REMOVE "${C_SOURCE}")

  if(NOT COMPILE_RESULT)
    set("${RESULT_VAR}" "${COMPILE_RESULT}")
    set(RUN_RESULT FALSE)
  else(NOT COMPILE_RESULT)
    set("${RESULT_VAR}" "${RUN_OK}")
  endif(NOT COMPILE_RESULT)

  if(RUN_OK)
    # add_definitions(-D${RESULT_VAR})
  else(RUN_OK)
    string(REPLACE "\n" ";" OUTPUT "${OUTPUT}")
    list(FILTER OUTPUT INCLUDE REGEX "error")
  endif(RUN_OK)

  if(ARGN)
    set("${ARGN}" "${RUN_OUTPUT}" PARENT_SCOPE)
  endif(ARGN)

  show_result(${RESULT_VAR})
endmacro(check_run RESULT_VAR SOURCE)
