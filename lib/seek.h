/* Public domain. */

#ifndef SEEK_H
#define SEEK_H

#include "typedefs.h"
#include "uint64.h"

typedef unsigned long seek_pos;

extern seek_pos seek_cur(fd_t);

extern int seek_set(fd_t, seek_pos);
extern int seek_end(fd_t);

extern int seek_trunc(fd_t, seek_pos);

#define seek_begin(fd) (seek_set((fd), (seek_pos)0))

void     seek_by_jump(fd_t fd, int64 amount);
void     seek_by_read(fd_t fd, int64 amount);

#endif
