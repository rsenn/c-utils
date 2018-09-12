#ifndef SELECT_H
#define SELECT_H

/* sysdep: +sysselect */

#include <sys/types.h>
#if !WINDOWS_NATIVE
#include <sys/time.h>
#endif
#include <sys/select.h>

/* braindead BSD uses bzero in FD_ZERO but doesn't #include string.h */
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int select();

#ifdef __cplusplus
}
#endif
#endif