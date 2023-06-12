include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/Functions.cmake)

#[[message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message("CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message("CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")]]

include_directories(${BROTLI_INCLUDE_DIR} ${BZIP2_INCLUDE_DIR} ${LIBLZMA_INCLUDE_DIR} ${ZLIB_INCLUDE_DIR})

if(CMAKE_COMPILER_IS_GNUCC OR "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
  if(NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    set(iarray_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/cas.S)
  endif(NOT "${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
endif(CMAKE_COMPILER_IS_GNUCC OR "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")

set(MODSUBDIRS
    alloc
    array
    binfmt
    bmap
    buffer
    byte
    case
    cb
    cbmap
    charbuf
    coff
    dir
    dlist
    dns
    elf
    env
    errmsg
    fmt
    gpio
    hashmap
    hmap
    http
    iarray
    ihex
    io
    json
    mmap
    ndelay
    omf
    open
    path
    pe
    playlist
    cpp
    process
    range
    rdir
    safemult
    scan
    seek
    set
    sig
    slist
    socket
    str
    stralloc
    strarray
    strlist
    tai
    taia
    textbuf
    textcode
    tls
    tokenizer
    ucs
    uint16
    uint32
    uint64
    unix
    utf8
    wait
    xml)
file(GLOB MODHEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(LIB_SOURCES)

foreach(LIBNAME ${MODSUBDIRS})
  file(GLOB MODHEADERS ${CMAKE_CURRENT_SOURCE_DIR}/${LIBNAME}.h ${CMAKE_CURRENT_SOURCE_DIR}/${LIBNAME}_*.h)
  file(GLOB MODSOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${LIBNAME}/*.c)
  if(NOT MODSOURCES)
    file(GLOB MODSOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${LIBNAME}_*.c)
  endif()
  set(EXTRA_SOURCES ${${LIBNAME}_SOURCES})
  set(EXTRA_DEFS "${${LIBNAME}_DEFS}")
  set(EXTRA_LIBS "${${LIBNAME}_LIBS}")

  set(ALL_SOURCES ${LIB_SOURCES})
  list(APPEND ALL_SOURCES ${LIB_SOURCES} ${MODSOURCES} ${MODHEADERS} ${EXTRA_SOURCES})
  set(LIB_SOURCES "${ALL_SOURCES}" PARENT_SCOPE)

  # message("Library ${LIBNAME} EXTRA_DEFS = ${EXTRA_DEFS}") if(EXTRA_SOURCES) message(STATUS "${LIBNAME} extra sources: ${EXTRA_SOURCES}") endif(EXTRA_SOURCES)

  add_library("${LIBNAME}" STATIC ${MODSOURCES} ${MODHEADERS} ${EXTRA_SOURCES})
  if(BUILD_SHARED_LIBS)
    add_library("${LIBNAME}-shared" SHARED ${MODSOURCES} ${MODHEADERS} ${EXTRA_SOURCES})
    set_target_properties("${LIBNAME}-shared" PROPERTIES PREFIX "lib" COMPILE_FLAGS "${WARN_C_COMPILER_FLAGS} -fPIC"
                                                         COMPILE_DEFINITIONS "${EXTRA_DEFS}")
  endif(BUILD_SHARED_LIBS)
  set_target_properties("${LIBNAME}" PROPERTIES PREFIX "" COMPILE_FLAGS
                                                          "${WARN_C_COMPILER_FLAGS} ${${LIBNAME}_COMPILE_FLAGS}")
  if(EXTRA_LIBS)
    # message("Library ${LIBNAME} EXTRA_LIBS = ${EXTRA_LIBS}")
    target_link_libraries("${LIBNAME}" PRIVATE ${EXTRA_LIBS})
  endif(EXTRA_LIBS)
endforeach(LIBNAME)

set_target_properties(io PROPERTIES COMPILE_FLAGS "${MUX_DEFS}")

foreach(NETLIB dns http io ndelay socket)
  target_link_libraries(${NETLIB} PRIVATE ${LIBMSWSOCK})
endforeach()

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/CheckWinSock.cmake)
if(WINDOWS)
  check_winsock()
endif(WINDOWS)

target_link_libraries(alloc PRIVATE byte)
target_link_libraries(unix PRIVATE stralloc buffer ndelay utf8)
target_link_libraries(array PRIVATE safemult byte str)
target_link_libraries(binfmt PRIVATE mmap byte)
target_link_libraries(buffer PRIVATE alloc byte fmt mmap open str ${ZLIB_LIBRARIES})
target_link_libraries(cb PRIVATE str)
target_link_libraries(cbmap PRIVATE str)
target_link_libraries(dns PRIVATE stralloc byte case fmt io open socket str taia uint16 ${LIBIPHLPAPI})
target_link_libraries(dir PRIVATE stralloc)
target_link_libraries(errmsg PRIVATE str)
target_link_libraries(elf PRIVATE byte str range mmap uint64 uint32 uint16)
target_link_libraries(fmt PRIVATE byte str uint64 uint32)
target_link_libraries(gpio PRIVATE mmap)
target_link_libraries(hmap PRIVATE str)
target_link_libraries(http PRIVATE fmt dns errmsg byte io scan socket stralloc str ${TLS_LIBRARIES})
target_link_libraries(io PRIVATE buffer iarray array byte fmt open taia ${LIBMSWSOCK})
target_link_libraries(json PRIVATE hashmap fmt scan)
target_link_libraries(bmap PRIVATE str)
target_link_libraries(mmap PRIVATE seek open scan ${LIBPSAPI})
target_link_libraries(open PRIVATE str uint32)
target_link_libraries(path PRIVATE unix dir strlist ${LIBSHLWAPI})
target_link_libraries(pe PRIVATE uint64 uint32 uint16)
target_link_libraries(playlist PRIVATE byte fmt scan str xml)
target_link_libraries(rdir PRIVATE dir)
target_link_libraries(scan PRIVATE byte str)
target_link_libraries(socket PRIVATE io iarray byte fmt ndelay scan uint16 ${LIBMSWSOCK})
target_link_libraries(str PRIVATE byte)
target_link_libraries(stralloc PRIVATE buffer alloc open byte fmt str)
target_link_libraries(strarray PRIVATE array unix)
target_link_libraries(strlist PRIVATE stralloc buffer)
target_link_libraries(taia PRIVATE tai)
target_link_libraries(textbuf PRIVATE byte playlist)
target_link_libraries(uint64 PRIVATE uint32)
target_link_libraries(xml PRIVATE buffer array strlist hmap alloc)
target_link_libraries(json PRIVATE slist hmap stralloc buffer alloc)
target_link_libraries(ndelay PRIVATE socket)
target_link_libraries(process PRIVATE wait errmsg)
target_link_libraries(ihex PRIVATE scan byte)
target_link_libraries(uint32 PRIVATE taia tai)
target_link_libraries(tls PRIVATE io iarray buffer byte str ${TLS_LIBRARIES})
target_link_libraries(http PRIVATE dns errmsg byte io scan tls socket stralloc str)
target_link_libraries(tokenizer PRIVATE buffer byte str)
target_link_libraries(cpp PRIVATE errmsg tokenizer strlist hashmap hmap)

if(BUILD_SHARED)
  file(GLOB LIB_SOURCES */*.c)

  add_library(c-utils SHARED ${LIB_SOURCES})
  if(MSYS)
    set(LIBPREFIX "msys-")
  else(MSYS)
    if(CYGWIN)
      set(LIBPREFIX "cyg")
    else(CYGWIN)
      if(MINGW)
        set(LIBPREFIX "lib")
      else(MINGW)
        if(WIN32)
          set(LIBPREFIX "")
        else(WIN32)

        endif(WIN32)
      endif(MINGW)
    endif(CYGWIN)
  endif(MSYS)
  set_target_properties(c-utils PROPERTIES SOVERSION 1.0 PREFIX "${LIBPREFIX}")

  target_link_libraries(c-utils PRIVATE ${TLS_LIBRARIES} ${COMPRESSION_LIBRARIES} ${ZLIB_LIBRARY} ${LIBMSWSOCK})

  install(TARGETS c-utils DESTINATION lib)
endif(BUILD_SHARED)
