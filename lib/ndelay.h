/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef NDELAY_H
#define NDELAY_H

#include "typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

int ndelay_on(fd_t);
int ndelay_off(fd_t);

#ifdef __cplusplus
}
#endif

#endif