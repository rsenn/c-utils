/**
 * @defgroup   ndelay
 * @brief      NDELAY module.
 * @{
 */
/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef NDELAY_H
#define NDELAY_H

#include "typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

int ndelay_on(fd_type);
int ndelay_off(fd_type);

#ifdef __cplusplus
}
#endif

#endif
/** @} */
