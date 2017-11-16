#ifndef SELECT_H
#define SELECT_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef IOPAUSE_POLL
extern int select();
/* sysdep: -sysselect */

#include <sys/types.h>
#include <sys/time.h>
extern int select();
#else
/* sysdep: +sysselect */

#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>

/* braindead BSD uses bzero in FD_ZERO but doesn't #include string.h */
#include <string.h>


#endif

#ifdef __cplusplus
}
#endif
#endif
