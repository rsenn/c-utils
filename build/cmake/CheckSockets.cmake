include(CheckIncludeFile)
include(CheckFunctionExists)
include(CheckTypeSize)

check_include_file(sys/socket.h HAVE_SYS_SOCKET_H)
check_include_file(sys/select.h HAVE_SYS_SELECT_H)
check_include_file(sys/poll.h HAVE_SYS_POLL_H)
check_include_file(fcntl.h HAVE_FCNTL_H)
check_include_file(io.h HAVE_IO_H)
check_include_file(netinet/in.h HAVE_NETINET_IN_H)
check_include_file(net/bpf.h HAVE_NET_BPF_H)
check_include_file(net/ethernet.h HAVE_NET_ETHERNET_H)
check_include_file(cygwin/in.h HAVE_CYGWIN_IN_H)
check_include_file(linux/filter.h HAVE_LINUX_FILTER_H)
check_include_file(linux/types.h HAVE_LINUX_TYPES_H)


if(CMAKE_HOST_WIN32)
#  check_include_file(winsock.h HAVE_WINSOCK2_H)
  check_include_file(winsock2.h HAVE_WINSOCK2_H)
  check_include_file(ws2tcpip.h HAVE_WS2TCPIP_H)
endif(CMAKE_HOST_WIN32)


if(HAVE_SYS_SOCKET_H)
  set(CMAKE_EXTRA_INCLUDE_FILES ${CMAKE_EXTRA_INCLUDE_FILES} sys/socket.h)
endif(HAVE_SYS_SOCKET_H)

if(HAVE_WINSOCK2_H)
  set(CMAKE_EXTRA_INCLUDE_FILES ${CMAKE_EXTRA_INCLUDE_FILES} winsock2.h)
endif(HAVE_WINSOCK2_H)

if(HAVE_WS2TCPIP_H)
  set(CMAKE_EXTRA_INCLUDE_FILES ${CMAKE_EXTRA_INCLUDE_FILES} ws2tcpip.h)
endif(HAVE_WS2TCPIP_H)

check_type_size(socklen_t SOCKLEN_T)


function(check_sockets OUT_VAR LIB INC)

  set(TRY_SOCK_PROG "
/*#define _WINSOCKAPI_
#define _WINSOCK2API_
*/")
set(TRY_SOCK_PROG "${TRY_SOCK_PROG}
#undef _WINSOCKAPI_
#undef _WINSOCK2API_
")
  foreach(I ${INC})
    set(TRY_SOCK_PROG "${TRY_SOCK_PROG}
    #include <${I}>")
  endforeach(I ${INC})
  set(TRY_SOCK_PROG "${TRY_SOCK_PROG}

  int main() {
  SOCKET l,a;
  fd_set w,r,e;
  struct timeval tv;
  select(10, &r, &w, &e, &tv);
  AcceptEx(l, a, NULL, 0,  0, 0, NULL, NULL);
  }")
  message("Program:"    "${TRY_SOCK_PROG}")
  set(SRC "${CMAKE_CURRENT_BINARY_DIR}/try_sockets.c")
  file(WRITE "${SRC}" "${TRY_SOCK_PROG}")

  try_compile("${OUT_VAR}" "${CMAKE_CURRENT_BINARY_DIR}" "${SRC}"
    LINK_LIBRARIES "${LIB}"
    OUTPUT_VARIABLE  OUTPUT
    )
#  message("check_sockets(${INC} ${LIB}) ${OUT_VAR}='${${OUT_VAR}}'")
  if(NOT "${${OUT_VAR}}")
    message("check_sockets ${OUT_VAR} ${LIB} ${INC} failed")
		unset(LIBMSWSOCK CACHE)
  endif(NOT "${${OUT_VAR}}")

  if("${${OUT_VAR}}")
    message("Got socket library: ${LIB}")
    set(LIBMSWSOCK "${LIB}" CACHE STRING "Windows socket library")
  endif("${${OUT_VAR}}")
endfunction(check_sockets OUT_VAR LIB INC)

#check_sockets(mswsock.h wsock32 HAVE_MSWSOCK_WSOCK32_LIB)
#check_sockets(mswsock.h mswsock HAVE_MSWSOCK_MWSOCK_LIB)
check_sockets(HAVE_WINSOCK2_WSOCK32_LIB wsock32 "windows.h;winsock2.h" )
check_sockets(HAVE_WINSOCK_WSOCK32_LIB wsock32 "windows.h;winsock.h" )
check_sockets(HAVE_WINSOCK2_WS2_32_LIB ws2_32 "windows.h;winsock2.h" )
check_sockets(HAVE_WINSOCK2_WS2_32_LIB ws2_32 "winsock2.h;windows.h" )


