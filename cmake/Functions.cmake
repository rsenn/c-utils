# link_directories() without the system library directories of the host. A
# -L/usr/lib/x86_64-linux-gnu in front of those of a musl toolchain makes the
# linker take the glibc libc.a.
macro(link_directories_nosys)
  set(_LIBDIRS "")
  foreach(_DIR ${ARGN})
    if(_DIR AND NOT "${_DIR}" STREQUAL "BEFORE" AND NOT "${_DIR}" STREQUAL "AFTER")
      get_filename_component(_ABS "${_DIR}" ABSOLUTE)
      list(FIND CMAKE_C_IMPLICIT_LINK_DIRECTORIES "${_ABS}" _IMPLICIT)
      if(_IMPLICIT EQUAL -1 AND NOT "${_ABS}" MATCHES "^/(usr/)?lib(64)?(/[^/]+-linux-[^/]+)?/?$")
        list(APPEND _LIBDIRS "${_DIR}")
      endif()
    endif()
  endforeach()
  if(_LIBDIRS)
    link_directories(${_LIBDIRS})
  endif()
endmacro(link_directories_nosys)

# include_directories() without the directories the compiler searches anyway.
# A -I/usr/include puts the host (glibc) headers in front of those of a musl
# or cross toolchain, and the build then fails in the system headers.
macro(include_directories_nosys)
  set(_INCDIRS "")
  foreach(_DIR ${ARGN})
    if(_DIR)
      get_filename_component(_ABS "${_DIR}" ABSOLUTE)
      list(FIND CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES "${_ABS}" _IMPLICIT)
      if(_IMPLICIT EQUAL -1 AND NOT "${_ABS}" STREQUAL "/usr/include")
        list(APPEND _INCDIRS "${_DIR}")
      elseif(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        # headers such as zlib.h that no toolchain directory has, searched last
        add_compile_options("SHELL:-idirafter ${_ABS}")
        # Debian puts some headers, such as openssl/opensslconf.h, below the
        # architecture directory
        execute_process(COMMAND gcc -print-multiarch OUTPUT_VARIABLE _MULTIARCH OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
        if(_MULTIARCH AND IS_DIRECTORY "${_ABS}/${_MULTIARCH}")
          add_compile_options("SHELL:-idirafter ${_ABS}/${_MULTIARCH}")
        endif()
      endif()
    endif()
  endforeach()
  if(_INCDIRS)
    include_directories(${_INCDIRS})
  endif()
endmacro(include_directories_nosys)

function(DUMP VAR)
  string(REGEX REPLACE "[;\n]" " " A "${ARGV}")
  message("\nVariable dump of: ${A}\n")

  foreach(VAR ${ARGV})
    message("  ${VAR} = ${${VAR}}")
  endforeach(VAR ${ARGV})
  message("")
endfunction(DUMP VAR)

function(DUMP_LIST VAR)
  foreach(VAR ${ARGV})
    message("List dump of: ${VAR}")
    foreach(LINE ${${VAR}})
      file(RELATIVE_PATH LINE "${CMAKE_SOURCE_DIR}" "${LINE}")
      message("${VAR}: ${LINE}")
    endforeach(LINE ${${VAR}})
  endforeach(VAR ${ARGV})
endfunction(DUMP_LIST VAR)

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
  endif(DEBUG_${NAME})
  if(${${MODULE}_LIBS})
    set(${MODULE}_LIBS "${${MODULE}_LIBS}" PARENT_SCOPE)
    #message("${MODULE}_LIBS ${${MODULE}_LIBS} PARENT_SCOPE")
  endif(${${MODULE}_LIBS})
endfunction(debug_option NAME DESC)

#
# var2define <NAME> [DEFINED_VALUE] [VAR_NAME]
#
# Add the compile definition -DNAME: =1 or =0 by the truth of variable NAME,
# or =DEFINED_VALUE if VAR_NAME is true.
#
function(var2define NAME)
  if(${ARGC} GREATER_EQUAL 3)
    set(VAR_NAME "${ARGV1}")
  else()
    set(VAR_NAME "${NAME}")
  endif()

  set(VALUE "${${VAR_NAME}}")
  if(${ARGC} LESS_EQUAL 1 AND ${ARGC} GREATER_EQUAL 0)
    if(VALUE)
      add_definitions(-D${NAME}=1)
    else()
      add_definitions(-D${NAME}=0)
    endif()
  else()
    if(VALUE)
      add_definitions(-D${NAME}=${ARGV1})
    endif()
  endif()
endfunction()

#
# clean_name <STRING> <OUTPUT-VAR>
#
function(clean_name STR OUTPUT_VAR)
  string(TOUPPER "${STR}" STR)
  string(REGEX REPLACE "[^A-Za-z0-9_]" "_" STR "${STR}")
  set("${OUTPUT_VAR}" "${STR}" PARENT_SCOPE)
endfunction()

#
# check_function_def <FUNCTION NAME> [RESULT VARIABLE] [PREPROC_DEF]
#
macro(check_function_def FUNC)
  if(${ARGC} GREATER 1)
    set(RESULT_VAR "${ARGV1}")
  else()
    string(TOUPPER "HAVE_${FUNC}" RESULT_VAR)
  endif()

  if(${ARGC} GREATER 2)
    set(PREPROC_DEF "${ARGV2}")
  else()
    string(TOUPPER "HAVE_${FUNC}" PREPROC_DEF)
  endif()

  if(NOT DEFINED ${RESULT_VAR})
    check_function_exists("${FUNC}" "_${RESULT_VAR}")

    if(${_${RESULT_VAR}})
      set("${RESULT_VAR}" TRUE CACHE INTERNAL "Define this if you have the '${FUNC}' function")
    else()
      set("${RESULT_VAR}" FALSE CACHE INTERNAL "Define this if you have the '${FUNC}' function")
    endif()
  endif()

  if(${${RESULT_VAR}})
    if(NOT "${PREPROC_DEF}" STREQUAL "")
      set("${PREPROC_DEF}" "1")
      var2define("${PREPROC_DEF}" 1)
    endif()
  endif()
endmacro(check_function_def FUNC)
macro(check_functions)
  foreach(FUNC ${ARGN})
    string(TOUPPER "HAVE_${FUNC}" RESULT_VAR)
    check_function_def("${FUNC}" "${RESULT_VAR}")
  endforeach(FUNC ${ARGN})
endmacro(check_functions)

#
# check_include_def <INCLUDE> [RESULT VARIABLE] [PREPROC_DEF]
#
macro(check_include_def INC)
  if(ARGC GREATER_EQUAL 2)
    set(RESULT_VAR "${ARGV1}")
    set(PREPROC_DEF "${ARGV2}")
  else()
    clean_name("HAVE_${INC}" RESULT_VAR)
    set(PREPROC_DEF "${RESULT_VAR}")
  endif()

  check_include_file("${INC}" "${RESULT_VAR}")

  if(${${RESULT_VAR}})
    set("${RESULT_VAR}" TRUE CACHE INTERNAL "Define this if you have the '${INC}' header file")

    if(NOT "${PREPROC_DEF}" STREQUAL "")
      var2define("${PREPROC_DEF}" 1)
    endif()
  endif()
endmacro(check_include_def INC)

#
# check_includes <INCLUDE FILES...>
#
macro(check_includes)
  foreach(INC ${ARGN})
    clean_name("HAVE_${INC}" RESULT_VAR)
    check_include_def("${INC}" "${RESULT_VAR}")
  endforeach(INC ${ARGN})
endmacro(check_includes)

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
  unset(DEPEND_TARGETS)

  list_contains("${BUILD_PROGRAMS}" "${PROG}" BUILD)

  if(NOT BUILD_PROGRAMS OR BUILD)
    set(PLIST "${PROGRAMS}")
    list(APPEND PLIST "${PROG}")
    set(PROGRAMS "${PLIST}" PARENT_SCOPE)
    string(REGEX REPLACE "[-/]" "_" PFX "${PROG}")
    foreach(DEP ${ARGN})
      if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}"
         AND NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}")
        list(APPEND "${PFX}_SOURCES" "${DEP}")
      else(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}"
           AND NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}")
        list(FIND MODSUBDIRS "${DEP}" I)
        if(I GREATER -1 OR IS_DIRECTORY
                           "${CMAKE_CURRENT_SOURCE_DIR}/lib/${DEP}")
          list(APPEND INCLUDES "lib/${DEP}.h")
        endif(I GREATER -1 OR IS_DIRECTORY
                              "${CMAKE_CURRENT_SOURCE_DIR}/lib/${DEP}")
        if("${DEP}" STREQUAL "debug")
          # "debug" is a keyword of target_link_libraries(), link the archive by path
          list(APPEND DEPS "$<TARGET_FILE:debug>")
          list(APPEND DEPEND_TARGETS debug)
        else()
          list(APPEND DEPS ${DEP})
        endif()
      endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}"
            AND NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${DEP}")
    endforeach(DEP ${ARGN})

    if(OVERRIDE_DEPS)
      set(DEPS ${OVERRIDE_DEPS})
    endif(OVERRIDE_DEPS)

    # message("Program '${PROG}' INCLUDES: ${INCLUDES}") message("add_executable(${PROG} ${PROG}.c ${${PROG}_SOURCES} ${INCLUDES}")
    # the main source is ${PROG}.c in the top directory or in a src/ subdirectory
    set(PROG_SOURCE ${PROG}.c)
    foreach(SRCDIR dnsutils binfmt toolchain ftp tcp eagle media xmljson serial filedata dirlist winreg intercept genmakefile text system)
      if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${PROG_SOURCE}"
         AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/${SRCDIR}/${PROG}.c")
        set(PROG_SOURCE src/${SRCDIR}/${PROG}.c)
      endif()
    endforeach(SRCDIR)
    add_executable(${PROG} ${PROG_SOURCE} ${${PFX}_SOURCES} ${INCLUDES})
    target_link_libraries(${PROG} ${DEPS} ${ELECTRICFENCE_LIBRARY}
                          ${EXTRA_LIBRARIES})
    if(DEPEND_TARGETS)
      add_dependencies(${PROG} ${DEPEND_TARGETS})
    endif(DEPEND_TARGETS)
    target_compile_definitions(${PROG} PRIVATE CONFIG_PREFIX="${CONFIG_PREFIX}")
    install(TARGETS ${PROG} DESTINATION bin)
    if(NOT EMSCRIPTEN AND OPENSSL_FOUND AND USE_SSL)
      if(CMAKE_COMPILER_IS_GNUCC OR "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang"
         OR "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
        set_target_properties(
          ${PROG}
          PROPERTIES LINK_FLAGS "-L${OPENSSL_LIB_DIR} ${EXE_LINKER_FLAGS}"
                     COMPILE_FLAGS "")
      endif(CMAKE_COMPILER_IS_GNUCC OR "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang"
            OR "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    endif(NOT EMSCRIPTEN AND OPENSSL_FOUND AND USE_SSL)
    if(EMSCRIPTEN)
      set_target_properties(${PROG} PROPERTIES SUFFIX
                                               ".${EMSCRIPTEN_EXE_SUFFIX}")
    endif(EMSCRIPTEN)
  endif(NOT BUILD_PROGRAMS OR BUILD)
endfunction(add_program PROG)

function(link_program PROG)
  list_contains(BUILD_PROGRAMS "${PROG}" BUILD)
  if(NOT BUILD_PROGRAMS OR BUILD)
    target_link_libraries("${PROG}" ${ARGN})
  endif(NOT BUILD_PROGRAMS OR BUILD)
endfunction(link_program PROG)

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
      if(CMAKE_COMPILER_IS_GNUCC OR "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang"
         OR "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
        set_target_properties(${MOD} PROPERTIES EXE_LINKER_FLAGS
                                                "-L${OPENSSL_LIB_DIR} -shared")
      endif(CMAKE_COMPILER_IS_GNUCC OR "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang"
            OR "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    endif(BUILD_SHARED_LIBS)
    target_compile_options(${MOD} PUBLIC -fPIC)
  endif(NOT EMSCRIPTEN)
endfunction(add_module MOD)

function(CHECK_FLAG FLAG VAR)
  if(NOT VAR OR VAR STREQUAL "")
    string(TOUPPER "${FLAG}" TMP)
    string(REGEX REPLACE "[^0-9A-Za-z]" _ VAR "${TMP}")
  endif(NOT VAR OR VAR STREQUAL "")
  set(CMAKE_REQUIRED_QUIET ON)
  check_c_compiler_flag("${FLAG}" "${VAR}")
  set(CMAKE_REQUIRED_QUIET OFF)

  set(RESULT "${${VAR}}")
  if(RESULT)
    set("${VAR}" "${FLAG}" PARENT_SCOPE)

    message(STATUS "Compiler flag ${FLAG} ... supported")
  else(RESULT)
    set("${VAR}" "" PARENT_SCOPE)
  endif(RESULT)
endfunction(CHECK_FLAG FLAG VAR)
