message(STATUS "Writing ${CMAKE_CURRENT_BINARY_DIR}/tryalloca.c")
file(
  WRITE "${CMAKE_CURRENT_BINARY_DIR}/tryalloca.c"
  "#include <stdlib.h>\n#ifdef A\n#include <alloca.h>\n#else\n#include <malloc.h>\n#endif\n\nint main() {\nchar* c=alloca(23);\n(void)c;\nreturn 0;\n}"
)
try_compile(HAVE_ALLOCA ${CMAKE_CURRENT_BINARY_DIR} SOURCES "${CMAKE_CURRENT_BINARY_DIR}/tryalloca.c")
if(HAVE_ALLOCA)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/havealloca.h" "#include <malloc.h>\n")
endif(HAVE_ALLOCA)

message(STATUS "Writing ${CMAKE_CURRENT_BINARY_DIR}/trybsdsf.c")
file(
  WRITE "${CMAKE_CURRENT_BINARY_DIR}/trybsdsf.c"
  "/* for macos X, don't ask */\n#define SENDFILE 1\n\n#include <sys/types.h>\n#include <sys/socket.h>\n#include <sys/uio.h>\n#include <errno.h>\n#include <netinet/in.h>\n#include <netinet/tcp.h>\n\nint main() {\nstruct sf_hdtr hdr;\nstruct iovec v[17+23];\nint r,fd=1;\noff_t sbytes;\nhdr.headers=v; hdr.hdr_cnt=17;\nhdr.trailers=v+17; hdr.trl_cnt=23;\nr=sendfile(0,1,37,42,&hdr,&sbytes,0);\n(void)r;\nreturn 0;\n}"
)
try_compile(HAVE_BSDSF ${CMAKE_CURRENT_BINARY_DIR} SOURCES "${CMAKE_CURRENT_BINARY_DIR}/trybsdsf.c")
if(HAVE_BSDSF)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/havebsdsf.h" "#define HAVE_BSDSENDFILE\n")
endif(HAVE_BSDSF)

message(STATUS "Writing ${CMAKE_CURRENT_BINARY_DIR}/trydevpoll.c")
file(
  WRITE "${CMAKE_CURRENT_BINARY_DIR}/trydevpoll.c"
  "#include <stdio.h>\n#include <stdlib.h>\n#include <poll.h>\n#include <netdb.h>\n#include <unistd.h>\n#include <fcntl.h>\n#include <strings.h>\n#include <sys/types.h>\n#include <sys/socket.h>\n#include <sys/errno.h>\n#include <netinet/in.h>\n#include <sys/devpoll.h>\n\nmain() {\nint fd=open(\"/dev/poll\",O_RDWR);\nstruct pollfd p[100];\nint i,r;\ndvpoll_t timeout;\np[0].fd=0;\np[0].events=POLLIN;\nwrite(fd,p,sizeof(struct pollfd));\ntimeout.dp_timeout=100;	/* milliseconds? */\ntimeout.dp_nfds=1;\ntimeout.dp_fds=p;\nr=ioctl(fd,DP_POLL,&timeout);\nfor (i=0; i<r; ++i)\nprintf(\"event %d on fd #%d\n\",p[i].revents,p[i].fd);\n}"
)
try_compile(HAVE_DEVPOLL ${CMAKE_CURRENT_BINARY_DIR} SOURCES "${CMAKE_CURRENT_BINARY_DIR}/trydevpoll.c")
if(HAVE_DEVPOLL)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/havedevpoll.h" "#define HAVE_DEVPOLL\n")
endif(HAVE_DEVPOLL)

message(STATUS "Writing ${CMAKE_CURRENT_BINARY_DIR}/tryepoll.c")
file(
  WRITE "${CMAKE_CURRENT_BINARY_DIR}/tryepoll.c"
  "#include <inttypes.h>\n#include <sys/epoll.h>\n#include <stdio.h>\n\nint main() {\nint efd=epoll_create(10);\nstruct epoll_event x;\nif (efd==-1) return 111;\nx.events=EPOLLIN;\nx.data.fd=0;\nif (epoll_ctl(efd,EPOLL_CTL_ADD,0 /* fd */,&x)==-1) return 111;\n{\nint n;\nstruct epoll_event y[100];\nif ((n=epoll_wait(efd,y,100,1000))==-1) return 111;\nif (n>0)\nprintf(\"event %d on fd #%d\n\",y[0].events,y[0].data.fd);\n}\nreturn 0;\n}"
)
try_compile(HAVE_EPOLL ${CMAKE_CURRENT_BINARY_DIR} SOURCES "${CMAKE_CURRENT_BINARY_DIR}/tryepoll.c")
if(HAVE_EPOLL)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/haveepoll.h" "#define HAVE_EPOLL 1\n")
endif(HAVE_EPOLL)

message(STATUS "Writing ${CMAKE_CURRENT_BINARY_DIR}/tryinline.c")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/tryinline.c" "static inline int foo(int bar) { return bar+1; }")
try_compile(HAVE_INLINE ${CMAKE_CURRENT_BINARY_DIR} SOURCES "${CMAKE_CURRENT_BINARY_DIR}/tryinline.c")
if(NOT HAVE_INLINE)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/haveinline.h" "#define inline\n")
endif(NOT HAVE_INLINE)

message(STATUS "Writing ${CMAKE_CURRENT_BINARY_DIR}/tryip6.c")
file(
  WRITE "${CMAKE_CURRENT_BINARY_DIR}/tryip6.c"
  "#include <sys/types.h>\n#include <sys/socket.h>\n#include <netinet/in.h>\n\nint main() {\nstruct sockaddr_in6 sa;\nsa.sin6_family = PF_INET6;\n(void)sa;\nreturn 0;\n}"
)
try_compile(HAVE_IP6 ${CMAKE_CURRENT_BINARY_DIR} SOURCES "${CMAKE_CURRENT_BINARY_DIR}/tryip6.c")
if(HAVE_IP6)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/haveip6.h" "#define LIBC_HAS_IP6\n")
endif(HAVE_IP6)

message(STATUS "Writing ${CMAKE_CURRENT_BINARY_DIR}/trykqueue.c")
file(
  WRITE "${CMAKE_CURRENT_BINARY_DIR}/trykqueue.c"
  "#include <sys/types.h>\n#include <sys/event.h>\n#include <sys/time.h>\n\nint main() {\nint kq=kqueue();\nstruct kevent kev;\nstruct timespec ts;\nif (kq==-1) return 111;\nEV_SET(&kev, 0 /* fd */, EVFILT_READ, EV_ADD|EV_ENABLE, 0, 0, 0);\nts.tv_sec=0; ts.tv_nsec=0;\nif (kevent(kq,&kev,1,0,0,&ts)==-1) return 111;\n\n{\nstruct kevent events[100];\nint i,n;\nts.tv_sec=1; ts.tv_nsec=0;\nswitch (n=kevent(kq,0,0,events,100,&ts)) {\ncase -1: return 111;\ncase 0: puts(\"no data on fd #0\"); break;\n}\nfor (i=0; i<n; ++i) {\nprintf(\"ident %d, filter %d, flags %d, fflags %d, data %d\n\",\nevents[i].ident,events[i].filter,events[i].flags,\nevents[i].fflags,events[i].data);\n}\n}\nreturn 0;\n}"
)
try_compile(HAVE_KQUEUE ${CMAKE_CURRENT_BINARY_DIR} SOURCES "${CMAKE_CURRENT_BINARY_DIR}/trykqueue.c")
if(HAVE_KQUEUE)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/havekqueue.h" "#define HAVE_KQUEUE\n")
endif(HAVE_KQUEUE)

message(STATUS "Writing ${CMAKE_CURRENT_BINARY_DIR}/tryn2i.c")
file(
  WRITE "${CMAKE_CURRENT_BINARY_DIR}/tryn2i.c"
  "#include <sys/types.h>\n#include <sys/socket.h>\n#include <net/if.h>\n\nint main() {\nstatic char ifname[IFNAMSIZ];\nchar *tmp=if_indextoname(0,ifname);\n(void)tmp;\nreturn 0;\n}"
)
try_compile(HAVE_N2I ${CMAKE_CURRENT_BINARY_DIR} SOURCES "${CMAKE_CURRENT_BINARY_DIR}/tryn2i.c")
if(HAVE_N2I)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/haven2i.h" "#define HAVE_N2I\n")
endif(HAVE_N2I)

message(STATUS "Writing ${CMAKE_CURRENT_BINARY_DIR}/trypoll.c")
file(
  WRITE "${CMAKE_CURRENT_BINARY_DIR}/trypoll.c"
  "#include <sys/types.h>\n#include <fcntl.h>\n#include <poll.h>\n\nint main()\n{\nstruct pollfd x;\n\nx.fd = open(\"trypoll.c\",O_RDONLY);\nif (x.fd == -1) _exit(111);\nx.events = POLLIN;\nif (poll(&x,1,10) == -1) _exit(1);\nif (x.revents != POLLIN) _exit(1);\n\n/* XXX: try to detect and avoid poll() imitation libraries */\n\n_exit(0);\n}"
)
try_compile(HAVE_POLL ${CMAKE_CURRENT_BINARY_DIR} SOURCES "${CMAKE_CURRENT_BINARY_DIR}/trypoll.c")
if(HAVE_POLL)
  file(
    WRITE "${CMAKE_CURRENT_BINARY_DIR}/iopause.h"
    "/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */\n#ifndef IOPAUSE_H\n#define IOPAUSE_H\n\n/* sysdep: +poll */\n#define IOPAUSE_POLL\n\n#include <sys/types.h>\n#include <sys/poll.h>\n\ntypedef struct pollfd iopause_fd;\n#define IOPAUSE_READ POLLIN\n#define IOPAUSE_WRITE POLLOUT\n\n#include \"taia.h\"\n\nextern void iopause(iopause_fd *,unsigned int,struct taia *,struct taia *);\n\n#endif\n"
  )
else(HAVE_POLL)
  file(
    WRITE "${CMAKE_CURRENT_BINARY_DIR}/iopause.h"
    "/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */\n#ifndef IOPAUSE_H\n#define IOPAUSE_H\n\n/* sysdep: -poll */\n\ntypedef struct {\n  int fd;\n  short events;\n  short revents;\n} iopause_fd;\n\n#define IOPAUSE_READ 1\n#define IOPAUSE_WRITE 4\n\n#include \"taia.h\"\n\nextern void iopause(iopause_fd *,unsigned int,struct taia *,struct taia *);\n\n#endif\n"
  )
endif(HAVE_POLL)

message(STATUS "Writing ${CMAKE_CURRENT_BINARY_DIR}/trypread.c")
file(
  WRITE "${CMAKE_CURRENT_BINARY_DIR}/trypread.c"
  "#include <unistd.h>\n\nint main() {\nchar buf[100];\nif (pread(0,buf,sizeof(buf),0)==-1) return 111;\nreturn 0;\n}")
try_compile(HAVE_PREAD ${CMAKE_CURRENT_BINARY_DIR} SOURCES "${CMAKE_CURRENT_BINARY_DIR}/trypread.c")
if(HAVE_PREAD)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/havepread.h" "#define HAVE_PREAD\n")
endif(HAVE_PREAD)

message(STATUS "Writing ${CMAKE_CURRENT_BINARY_DIR}/tryscope.c")
file(
  WRITE "${CMAKE_CURRENT_BINARY_DIR}/tryscope.c"
  "#include <sys/types.h>\n#include <sys/socket.h>\n#include <netinet/in.h>\n\nint main() {\nstruct sockaddr_in6 sa;\nsa.sin6_family = PF_INET6;\nsa.sin6_scope_id = 23;\n(void)sa;\nreturn 0;\n}"
)
try_compile(HAVE_SCOPE ${CMAKE_CURRENT_BINARY_DIR} SOURCES "${CMAKE_CURRENT_BINARY_DIR}/tryscope.c")
if(HAVE_SCOPE)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/havescope.h" "#define LIBC_HAS_SCOPE_ID\n")
endif(HAVE_SCOPE)

message(STATUS "Writing ${CMAKE_CURRENT_BINARY_DIR}/trysendfile.c")
file(
  WRITE "${CMAKE_CURRENT_BINARY_DIR}/trysendfile.c"
  "#ifdef __hpux__\n#include <sys/types.h>\n#include <sys/uio.h>\n#include <sys/socket.h>\n#include <stdio.h>\n\nint main() {\n/*\nsbsize_t sendfile(int s, int fd, off_t offset, bsize_t nbytes,\nconst struct iovec *hdtrl, int flags);\n*/\nstruct iovec x[2];\nint fd=open(\"havesendfile.c\",0);\nx[0].iov_base=\"header\";\nx[0].iov_len=6;\nx[1].iov_base=\"footer\";\nx[1].iov_len=6;\nsendfile(1 /* dest socket */,fd /* src file */,\n0 /* offset */, 23 /* nbytes */,\nx, 0);\nperror(\"sendfile\");\nreturn 0;\n}\n#elif defined (__sun__) && defined(__svr4__)\n#include <sys/types.h>\n#include <sys/socket.h>\n#include <sys/sendfile.h>\n#include <stdio.h>\n\nint main() {\noff_t o;\no=0;\nsendfile(1 /* dest */, 0 /* src */,&o,23 /* nbytes */);\nperror(\"sendfile\");\nreturn 0;\n}\n#elif defined (_AIX)\n\n#define _FILE_OFFSET_BITS 64\n#include <sys/types.h>\n#include <sys/socket.h>\n#include <unistd.h>\n#include <stdio.h>\n\nint main() {\nint fd=open(\"havesendfile.c\",0);\nstruct sf_parms p;\nint destfd=1;\np.header_data=\"header\";\np.header_length=6;\np.file_descriptor=fd;\np.file_offset=0;\np.file_bytes=23;\np.trailer_data=\"footer\";\np.trailer_length=6;\nif (send_file(&destfd,&p,0)>=0)\nprintf(\"sent %lu bytes.\n\",p.bytes_sent);\nreturn 0;\n}\n#elif defined(__linux__)\n\n#define _FILE_OFFSET_BITS 64\n#include <sys/types.h>\n#include <unistd.h>\n#if defined(__GLIBC__)\n#include <sys/sendfile.h>\n#elif defined(__dietlibc__)\n#include <sys/sendfile.h>\n#else\n#include <linux/unistd.h>\n_syscall4(int,sendfile,int,out,int,in,long *,offset,unsigned long,count)\n#endif\n#include <stdio.h>\n\nint main() {\nint fd=open(\"havesendfile.c\",0);\noff_t o=0;\noff_t r=sendfile(1,fd,&o,23);\nif (r!=-1)\nprintf(\"sent %llu bytes.\n\",r);\nreturn 0;\n}\n\n#else\n#error unsupported architecture\n/* stupid fucking IRIX c99 does not signal #error via a non-zero exit\n* code!  ARGH!  So insert a parse error */\n#include rumpelstilzchen\n) \n#endif"
)
try_compile(HAVE_SENDFILE ${CMAKE_CURRENT_BINARY_DIR} SOURCES "${CMAKE_CURRENT_BINARY_DIR}/trysendfile.c")
if(HAVE_SENDFILE)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/havesendfile.h" "#define HAVE_SENDFILE\n")
endif(HAVE_SENDFILE)

message(STATUS "Writing ${CMAKE_CURRENT_BINARY_DIR}/trysigio.c")
file(
  WRITE "${CMAKE_CURRENT_BINARY_DIR}/trysigio.c"
  "#define _GNU_SOURCE\n#include <sys/types.h>\n#include <sys/poll.h>\n#include <signal.h>\n#include <fcntl.h>\n#include <stdio.h>\n\nint main() {\nint signum=SIGRTMIN+1;\nsigset_t ss;\nsigemptyset(&ss);\nsigaddset(&ss,signum);\nsigaddset(&ss,SIGIO);\nsigprocmask(SIG_BLOCK,&ss,0);\n\nfcntl(0 /* fd */,F_SETOWN,getpid());\nfcntl(0 /* fd */,F_SETSIG,signum);\n#if defined(O_ONESIGFD) && defined(F_SETAUXFL)\nfcntl(0 /* fd */, F_SETAUXFL, O_ONESIGFD);\n#endif\nfcntl(0 /* fd */,F_SETFL,fcntl(0 /* fd */,F_GETFL)|O_NONBLOCK|O_ASYNC);\n\n{\nsiginfo_t info;\nstruct timespec timeout;\nint r;\ntimeout.tv_sec=1; timeout.tv_nsec=0;\nswitch ((r=sigtimedwait(&ss,&info,&timeout))) {\ncase SIGIO:\n/* signal queue overflow */\nsignal(signum,SIG_DFL);\n/* do poll */\nbreak;\ndefault:\nif (r==signum) {\nprintf(\"event %c%c on fd #%d\n\",info.si_band&POLLIN?'r':'-',info.si_band&POLLOUT?'w':'-',info.si_fd);\n}\n}\n}\nreturn 0;\n}"
)
try_compile(HAVE_SIGIO ${CMAKE_CURRENT_BINARY_DIR} SOURCES "${CMAKE_CURRENT_BINARY_DIR}/trysigio.c")
if(HAVE_SIGIO)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/havesigio.h" "#define HAVE_SIGIO\n")
endif(HAVE_SIGIO)

message(STATUS "Writing ${CMAKE_CURRENT_BINARY_DIR}/trysl.c")
file(
  WRITE "${CMAKE_CURRENT_BINARY_DIR}/trysl.c"
  "#include <sys/types.h>\n#if defined(_WIN32) || defined(_WIN64)\n#include <winsock.h>\n#include <ws2tcpip.h>\n#else\n#include <sys/socket.h>\n#include <netinet/in.h>\n#endif\n\nint main() {\nsocklen_t t;\n(void)t;\nreturn 0;\n}"
)
try_compile(HAVE_SL ${CMAKE_CURRENT_BINARY_DIR} SOURCES "${CMAKE_CURRENT_BINARY_DIR}/trysl.c")
if(NOT HAVE_SL)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/havesl.h" "typedef int socklen_t;\n")
endif(NOT HAVE_SL)

message(STATUS "Writing ${CMAKE_CURRENT_BINARY_DIR}/trysocket.c")
file(
  WRITE "${CMAKE_CURRENT_BINARY_DIR}/trysocket.c"
  "#include <sys/types.h>\n#include <sys/socket.h>\n#include <netinet/in.h>\n\nint main() {\nint fd=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);\n(int)fd;\nreturn 0;\n}"
)
try_compile(HAVE_LIBSOCKET ${CMAKE_CURRENT_BINARY_DIR} SOURCES "${CMAKE_CURRENT_BINARY_DIR}/trysocket.c")
if(HAVE_LIBSOCKET)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/libsocket" "-lsocket -lnsl\n")
endif(HAVE_LIBSOCKET)

message(STATUS "Writing ${CMAKE_CURRENT_BINARY_DIR}/trysysel.c")
file(
  WRITE "${CMAKE_CURRENT_BINARY_DIR}/trysysel.c"
  "#include <sys/types.h>\n#include <sys/time.h>\n#include <sys/select.h> /* SVR4 silliness */\n\nvoid foo()\n{\n;\n}")
try_compile(HAVE_SELECT ${CMAKE_CURRENT_BINARY_DIR} SOURCES "${CMAKE_CURRENT_BINARY_DIR}/trysysel.c")
if(HAVE_SELECT)
  file(
    WRITE "${CMAKE_CURRENT_BINARY_DIR}/select.h"
    "#ifndef SELECT_H\n#define SELECT_H\n\n/* sysdep: +sysselect */\n\n#include <sys/types.h>\n#include <sys/time.h>\n#include <sys/select.h>\n\n/* braindead BSD uses bzero in FD_ZERO but doesn't #include string.h */\n#include <string.h>\n\nextern int select();\n\n#endif\n"
  )
else(HAVE_SELECT)
  file(
    WRITE "${CMAKE_CURRENT_BINARY_DIR}/select.h"
    "#ifndef SELECT_H\n#define SELECT_H\n\n/* sysdep: -sysselect */\n\n#include <sys/types.h>\n#include <sys/time.h>\nextern int select();\n\n#endif\n"
  )
endif(HAVE_SELECT)

message(STATUS "Writing ${CMAKE_CURRENT_BINARY_DIR}/tryuint128.c")
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/tryuint128.c" "__uint128_t tmp;")
try_compile(HAVE_UINT128 ${CMAKE_CURRENT_BINARY_DIR} SOURCES "${CMAKE_CURRENT_BINARY_DIR}/tryuint128.c")
if(HAVE_UINT128)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/haveuint128.h" "#define HAVE_UINT128\n")
endif(HAVE_UINT128)
