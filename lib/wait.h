/**
 * @defgroup   wait
 * @brief      WAIT module.
 * @{
 */
#ifndef WAIT_H
#define WAIT_H

#include "uint64.h"

#ifdef __cplusplus
extern "C" {
#endif

int wait_nohang(int*);
int wait_nointr(int*);
unsigned int wait_reap(void);

int wait_pid(int, int*);
int wait_pid_nohang(int, int*);

int wait_pids(int*, unsigned int);
int wait_pids_nohang(int const*, unsigned int, int*);
int wait_pids_reap(int*, unsigned int);

/* portable replacements for WIFEXITED()/WEXITSTATUS() -- wstat is not a
 * raw POSIX packed status on WINDOWS_NATIVE, so decode it via these
 * instead of <sys/wait.h>'s macros to stay portable */
int wait_ifexited(int wstat);
int wait_exitstatus(int wstat);

#ifdef __cplusplus
}
#endif

#endif // WAIT_H
/** @} */
