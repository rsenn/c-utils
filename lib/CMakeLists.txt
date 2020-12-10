message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message("CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message("CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")

set(iarray_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/cas.S)

set(MODSUBDIRS alloc array binfmt buffer byte case cb cbmap charbuf coff dir dns elf env errmsg fmt gpio hashmap hmap http iarray ihex io json list map mmap ndelay omf open path pe playlist process range rdir safemult scan seek set sig slist socket str stralloc strarray strlist tai taia textbuf textcode tls uint16 uint32 uint64 unix wait xml ucs utf8)
file(GLOB MODHEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

foreach(LIBNAME ${MODSUBDIRS})
  file(GLOB MODHEADERS ${CMAKE_CURRENT_SOURCE_DIR}/${LIBNAME}.h ${CMAKE_CURRENT_SOURCE_DIR}/${LIBNAME}_*.h)
  file(GLOB MODSOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${LIBNAME}/*.c)
  if(NOT MODSOURCES)
    file(GLOB MODSOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${LIBNAME}_*.c)
  endif()
  set(EXTRA_SOURCES ${${LIBNAME}_SOURCES})
  set(EXTRA_DEFS "${${LIBNAME}_DEFS}")

  # message("Library ${LIBNAME} EXTRA_DEFS = ${EXTRA_DEFS}")
  if(EXTRA_SOURCES)
    message(STATUS "${LIBNAME} extra sources: ${EXTRA_SOURCES}")
  endif(EXTRA_SOURCES)
  add_library("${LIBNAME}" STATIC ${MODSOURCES} ${MODHEADERS} ${EXTRA_SOURCES})
  if(BUILD_SHARED_LIBS)
    add_library("${LIBNAME}-shared" SHARED ${MODSOURCES} ${MODHEADERS} ${EXTRA_SOURCES})
    set_target_properties("${LIBNAME}-shared" PROPERTIES PREFIX "lib" COMPILE_FLAGS "${WARN_C_COMPILER_FLAGS} -fPIC" COMPILE_DEFINITIONS "${EXTRA_DEFS}")
  endif(BUILD_SHARED_LIBS)
  set_target_properties("${LIBNAME}" PROPERTIES PREFIX "" COMPILE_FLAGS "${WARN_C_COMPILER_FLAGS} ${${LIBNAME}_COMPILE_FLAGS}")

endforeach(LIBNAME)

set_target_properties(io PROPERTIES COMPILE_FLAGS "${MUX_DEFS}")

foreach(NETLIB dns http io ndelay socket)
  target_link_libraries(${NETLIB} ${LIBMSWSOCK})
endforeach()

target_link_libraries(unix stralloc buffer ndelay)
target_link_libraries(array safemult byte str)
target_link_libraries(binfmt mmap byte)
target_link_libraries(buffer alloc byte fmt mmap open str ${ZLIB_LIBRARY})
target_link_libraries(cb str)
target_link_libraries(cbmap str)
target_link_libraries(dns byte case fmt io open socket str taia uint16 ${LIBIPHLPAPI})
target_link_libraries(dir stralloc)
target_link_libraries(errmsg str)
target_link_libraries(elf byte str range mmap uint64 uint32 uint16)
target_link_libraries(fmt byte str uint64 uint32)
target_link_libraries(gpio mmap)
target_link_libraries(hmap str)
target_link_libraries(http fmt dns errmsg byte io scan socket stralloc str ${LIBSSL})
target_link_libraries(io buffer iarray array byte fmt open taia ${LIBMSWSOCK})
target_link_libraries(json charbuf hmap scan)
target_link_libraries(map str)
target_link_libraries(mmap seek open scan ${LIBPSAPI})
target_link_libraries(open str uint32)
target_link_libraries(path dir strlist)
target_link_libraries(pe uint64 uint32 uint16)
target_link_libraries(playlist byte fmt scan str xml)
target_link_libraries(rdir dir)
target_link_libraries(scan byte str)
target_link_libraries(socket byte fmt ndelay scan uint16 ${LIBMSWSOCK})
target_link_libraries(str byte)
target_link_libraries(stralloc buffer alloc open byte fmt str)
target_link_libraries(strarray array)
target_link_libraries(strlist stralloc buffer)
target_link_libraries(taia tai)
target_link_libraries(textbuf byte playlist)
target_link_libraries(uint64 uint32)
target_link_libraries(xml buffer array strlist hmap alloc)
target_link_libraries(json slist hmap stralloc buffer alloc)
target_link_libraries(ndelay socket)
target_link_libraries(process wait errmsg)
target_link_libraries(ihex scan byte)
target_link_libraries(uint32 taia tai)
target_link_libraries(tls io iarray buffer byte str ${LIBSSL})
target_link_libraries(http dns errmsg byte io scan tls socket stralloc str)