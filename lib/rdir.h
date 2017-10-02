#ifndef _RDIR_H__
#define _RDIR_H__

#include "dir.h"
#include "stralloc.h"

#ifdef __cplusplus
extern "C" {
#endif

struct rdir_s;

typedef struct rdir_s {
  union {
    struct dir_s dir;
    void* dir_int;
  };
  struct rdir_s* prev;
  stralloc sa;
} rdir_t;

int rdir_open(rdir_t* d, const char* p);
char* rdir_read(rdir_t* d);
void rdir_close(rdir_t* d);

/*char* rdir_name(rdir_t* d);
int rdir_type(rdir_t* d);
time_t rdir_time(rdir_t* d, int time_type);
*/
#ifdef __cplusplus
}
#endif

#endif // _RDIR_H__
