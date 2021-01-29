function(DUMP VAR)
  string(REGEX REPLACE "[;\n]" " " A "${ARGV}")
  message("\nVariable dump of: ${A}\n")

  foreach(VAR ${ARGV})
    message("  ${VAR} = ${${VAR}}")
  endforeach(VAR ${ARGV})
  message("")
endfunction(DUMP VAR)

function(debug_option NAME DESC)
  option(DEBUG_${NAME} "${DESC}" OFF)
  string(TOLOWER "${NAME}" MODULE)
  set(DEFS "${${MODULE}_DEFS}")
  if("${DEFS}" STREQUAL "")
    set(DEFS "MODULE_${NAME}=1")
  endif()
  if(DEBUG_${NAME})
    set("${MODULE}_DEFS" "DEBUG_${NAME}=1;${DEFS}" PARENT_SCOPE)
    add_definitions(-DDEBUG_${NAME}=1)
    set(APPEND "${MODULE}_LIBS" buffer)
    message("APPEND ${MODULE}_LIBS buffer")
  endif(DEBUG_${NAME})
  if(${${MODULE}_LIBS})
    set(${MODULE}_LIBS "${${MODULE}_LIBS}" PARENT_SCOPE)
    message("${MODULE}_LIBS ${${MODULE}_LIBS} PARENT_SCOPE")
  endif(${${MODULE}_LIBS})
endfunction(debug_option NAME DESC)

macro(check_function_def FUNC)
  if(ARGC EQUAL 1)
    string(TOUPPER "HAVE_${FUNC}" RESULT_VAR)
  else(ARGC EQUAL 1)
    set(RESULT_VAR "${ARGN}")
  endif(ARGC EQUAL 1)
  # message("FUNC: ${FUNC} RESULT_VAR: ${RESULT_VAR}")
  check_function_exists("${FUNC}" "${RESULT_VAR}")
  if(${${RESULT_VAR}})
    # add_definitions(-D${RESULT_VAR}) message("${RESULT_VAR} ${${RESULT_VAR}}")
  endif(${${RESULT_VAR}})
endmacro(check_function_def FUNC RESULT_VAR)
function(check_functions)
  foreach(FUNC ${ARGN})
    string(TOUPPER "HAVE_${FUNC}" RESULT_VAR)
    check_function_def("${FUNC}" "${RESULT_VAR}")
  endforeach(FUNC ${ARGN})
endfunction(check_functions)

function(list_contains LIST ITEM OUTPUT_VAR)
  set(RESULT FALSE)
  foreach(IT ${LIST})
    if("${IT}" STREQUAL "${ITEM}")
      set(RESULT TRUE)
    endif("${IT}" STREQUAL "${ITEM}")
  endforeach(IT ${LIST})
  set("${OUTPUT_VAR}" ${RESULT} PARENT_SCOPE)
endfunction(list_contains LIST ITEM OUTPUT_VAR)

function(DEFINE_HAVE OUTVAR)
  set(OUT "")
  foreach(DEF ${ARGN})
    # message("Check definition ${DEF}: ${${DEF}}")
    if(${DEF} STREQUAL 1)
      list(APPEND OUT "${DEF}=1")
    endif(${DEF} STREQUAL 1)
  endforeach(DEF ${ARGN})
  set("${OUTVAR}" "${OUT}" PARENT_SCOPE)

endfunction(DEFINE_HAVE)

function(add_program PROG)
  unset(INCLUDES)
  unset(LIBS)
  string(REGEX REPLACE "[-/]" "_" PFX "${PROG}")
  foreach(DEP ${ARGN})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}" AND NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}")
      list(APPEND "${PFX}_SOURCES" "${DEP}")
    else(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}" AND NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}")
      list(FIND MODSUBDIRS "${DEP}" I)
      if(I GREATER -1 OR IS_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${DEP}")
        list(APPEND INCLUDES "lib/${DEP}.h")
      endif(I GREATER -1 OR IS_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${DEP}")
      list(APPEND DEPS ${DEP})
    endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}" AND NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}")
  endforeach(DEP ${ARGN})
  # message("Program '${PROG}' INCLUDES: ${INCLUDES}") message("add_executable(${PROG} ${PROG}.c ${${PROG}_SOURCES} ${INCLUDES}")
  add_executable(${PROG} ${PROG}.c ${${PFX}_SOURCES} ${INCLUDES})
  target_link_libraries(${PROG} ${DEPS} ${ELECTRICFENCE_LIBRARY} ${EXTRA_LIBRARIES})
  target_compile_definitions(${PROG} PRIVATE CONFIG_PREFIX="${CONFIG_PREFIX}")
  install(TARGETS ${PROG} DESTINATION bin)
  if(NOT EMSCRIPTEN AND OPENSSL_FOUND AND USE_SSL)
    set_target_properties(${PROG} PROPERTIES LINK_FLAGS "-L${OPENSSL_LIB_DIR} ${EXE_LINKER_FLAGS}" COMPILE_FLAGS "")
  endif(NOT EMSCRIPTEN AND OPENSSL_FOUND AND USE_SSL)
  if(EMSCRIPTEN)
    set_target_properties(${PROG} PROPERTIES SUFFIX ".${EMSCRIPTEN_EXE_SUFFIX}")
  endif(EMSCRIPTEN)
endfunction(add_program PROG)

function(add_module MOD SRC)
  unset(INCLUDES)
  #[[ foreach(DEP ${ARGN})
    list(FIND MODSUBDIRS "${DEP}" I)
    if(I GREATER -1)
    list(APPEND INCLUDES "lib/${DEP}.h")
    endif(I GREATER -1)
    endforeach(DEP ${ARGN})]]
  # message("Program '${MOD}' dependencies: ${INCLUDES}")
  set(LIBS "")
  foreach(LIB ${ARGN})
    if(BUILD_SHARED_LIBS)
      list(APPEND LIBS "${LIB}-shared")
    endif(BUILD_SHARED_LIBS)
  endforeach(LIB ${ARGN})
  if(NOT EMSCRIPTEN)
    add_library(${MOD} SHARED ${SRC})
    target_link_libraries(${MOD} ${LIBS} ${EXTRA_LIBRARIES})
    install(TARGETS ${MOD} DESTINATION libexec)
    if(BUILD_SHARED_LIBS)
      set_target_properties(${MOD} PROPERTIES EXE_LINKER_FLAGS "-L${OPENSSL_LIB_DIR} -shared")
    endif(BUILD_SHARED_LIBS)
    target_compile_options(${MOD} PUBLIC -fPIC)
  endif(NOT EMSCRIPTEN)
endfunction(add_module MOD)
