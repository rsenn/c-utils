#ifndef _RDIR_INTERNAL_H__
#define _RDIR_INTERNAL_H__

#include "rdir.h"
#include "dir_internal.h"

#define _POSIX_ 1

struct rdir_internal_s {
  struct dir_internal_s dir;
  struct rdir_internal_s *prev;
};


#endif // _RDIR_INTERNAL_H__
