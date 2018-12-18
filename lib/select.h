#ifndef SELECT_H
#define SELECT_H

#include "socket.h"

/* sysdep: +sysselect */

#include <sys/types.h>

#if !WINDOWS_NATIVE
#include <sys/time.h>
#include <sys/select.h>
#endif

/* braindead BSD uses bzero in FD_ZERO but doesn't #include string.h */
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(_WINSOCKAPI_) && !defined(_WINSOCK2API_)
extern int select();
#endif

#ifdef __cplusplus
}
#endif
#endif
