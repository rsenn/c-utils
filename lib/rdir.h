#ifndef _RDIR_H__
#define _RDIR_H__

#include "dir.h"
#include "dir_internal.h"
#include "stralloc.h"

#ifdef __cplusplus
extern "C" {
#endif

struct rdir_s;

struct rdir_s {
  struct rdir_s* prev;
  stralloc sa;
  dir_t dir;
};
typedef struct rdir_s rdir_t;

int rdir_open(rdir_t* d, const char* p);
char* rdir_read(rdir_t* d);
void rdir_close(rdir_t* d);

#ifdef __cplusplus
}
#endif

#endif /* _RDIR_H__ */