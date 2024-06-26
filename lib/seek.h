/**
 * @defgroup   seek
 * @brief      SEEK module.
 * @{
 */
/* Public domain. */

#ifndef SEEK_H
#define SEEK_H

#include "typedefs.h"
#include "uint64.h"

typedef int64 seek_pos;

void seek_by_jump(fd_type, int64 amount);
void seek_by_read(fd_type, int64 amount);
seek_pos seek_cur(fd_type);
seek_pos seek_end(fd_type);
int seek_set(fd_type, seek_pos pos);

#define seek_begin(fd) (seek_set((fd), (seek_pos)0))

#endif
/** @} */
