# third_party_fetch(NAME SRCVAR [GIT url] [TAG tag] [URL tarball] [OVERLAY CMakeLists.txt])
#
# Puts the source of NAME into third_party/NAME and sets SRCVAR to that
# directory. Nothing below third_party/ is tracked by git (see its .gitignore),
# the sources are cloned (GIT, shallow, at TAG) or unpacked (URL) on demand.
function(third_party_fetch NAME SRCVAR)
  cmake_parse_arguments(TP "" "GIT;TAG;URL;OVERLAY" "" ${ARGN})

  set(_dir "${CMAKE_CURRENT_SOURCE_DIR}/third_party/${NAME}")

  if(NOT IS_DIRECTORY "${_dir}")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/third_party")

    if(TP_GIT)
      find_package(Git REQUIRED)
      set(_tag)
      if(TP_TAG)
        set(_tag --branch "${TP_TAG}")
      endif(TP_TAG)
      message(STATUS "Cloning ${NAME} from ${TP_GIT} into ${_dir}")
      execute_process(COMMAND "${GIT_EXECUTABLE}" clone --depth 1 ${_tag} "${TP_GIT}" "${_dir}"
                      RESULT_VARIABLE _result OUTPUT_QUIET ERROR_QUIET)
      if(NOT _result EQUAL 0)
        file(REMOVE_RECURSE "${_dir}")
      endif(NOT _result EQUAL 0)
    elseif(TP_URL)
      get_filename_component(_file "${TP_URL}" NAME)
      set(_tmp "${CMAKE_CURRENT_BINARY_DIR}/third_party-${NAME}")
      file(MAKE_DIRECTORY "${_tmp}")
      message(STATUS "Downloading ${NAME} from ${TP_URL}")
      file(DOWNLOAD "${TP_URL}" "${_tmp}/${_file}" STATUS _status)
      list(GET _status 0 _result)
      if(_result EQUAL 0)
        file(ARCHIVE_EXTRACT INPUT "${_tmp}/${_file}" DESTINATION "${_tmp}/x")
        file(GLOB _top LIST_DIRECTORIES TRUE "${_tmp}/x/*")
        list(GET _top 0 _top)
        file(RENAME "${_top}" "${_dir}")
      endif(_result EQUAL 0)
      file(REMOVE_RECURSE "${_tmp}")
    endif(TP_GIT)

    if(NOT IS_DIRECTORY "${_dir}")
      message(FATAL_ERROR "Could not fetch ${NAME} into ${_dir} (no network? put the source there by hand)")
    endif(NOT IS_DIRECTORY "${_dir}")
  endif(NOT IS_DIRECTORY "${_dir}")

  # projects without a cmake build get ours
  if(TP_OVERLAY AND NOT EXISTS "${_dir}/CMakeLists.txt")
    configure_file("${TP_OVERLAY}" "${_dir}/CMakeLists.txt" COPYONLY)
  endif(TP_OVERLAY AND NOT EXISTS "${_dir}/CMakeLists.txt")

  set(${SRCVAR} "${_dir}" PARENT_SCOPE)
endfunction(third_party_fetch)

# third_party_build(NAME SRC PREFIX PIC BYPRODUCT [CMAKE_ARGS ...])
#
# Configures, builds and installs the cmake project in SRC into PREFIX with the
# toolchain of this project, unless BYPRODUCT (a file below PREFIX) is there
# already. This runs at configure time so that headers and libraries exist
# when the programs are configured.
function(third_party_build NAME SRC PREFIX PIC BYPRODUCT)
  cmake_parse_arguments(TP "" "" "CMAKE_ARGS" ${ARGN})

  if(EXISTS "${BYPRODUCT}")
    return()
  endif(EXISTS "${BYPRODUCT}")

  set(_build "${CMAKE_CURRENT_BINARY_DIR}/third_party-${NAME}")
  set(_args "-DCMAKE_INSTALL_PREFIX=${PREFIX}" -DCMAKE_INSTALL_LIBDIR=lib -DCMAKE_C_FLAGS=-w
            "-DCMAKE_BUILD_TYPE=Release" "-DCMAKE_POSITION_INDEPENDENT_CODE=${PIC}" -DBUILD_SHARED_LIBS=OFF)

  foreach(_var CMAKE_TOOLCHAIN_FILE CMAKE_C_COMPILER CMAKE_SYSROOT CMAKE_SYSTEM_NAME CMAKE_SYSTEM_PROCESSOR)
    if(${_var})
      list(APPEND _args "-D${_var}=${${_var}}")
    endif(${_var})
  endforeach(_var)

  message(STATUS "Building ${NAME} into ${PREFIX}")

  foreach(_step "-S;${SRC};-B;${_build};${_args};${TP_CMAKE_ARGS}" "--build;${_build};--parallel" "--install;${_build}")
    execute_process(COMMAND "${CMAKE_COMMAND}" ${_step} RESULT_VARIABLE _result OUTPUT_VARIABLE _out ERROR_VARIABLE _out)
    if(NOT _result EQUAL 0)
      message(FATAL_ERROR "Building ${NAME} failed (${_build}):\n${_out}")
    endif(NOT _result EQUAL 0)
  endforeach(_step)

  if(NOT EXISTS "${BYPRODUCT}")
    message(FATAL_ERROR "Building ${NAME} did not produce ${BYPRODUCT}")
  endif(NOT EXISTS "${BYPRODUCT}")
endfunction(third_party_build)

# the one prefix every third_party library installs into, per build directory
set(THIRD_PARTY_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/deps" CACHE PATH "install prefix of the libraries built from third_party/")
