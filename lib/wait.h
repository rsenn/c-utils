/**
 * @defgroup   WAIT wait
 *
 * @brief      This file implements wait.
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

#ifdef __cplusplus
}
#endif

#endif // WAIT_H
/** @} */
