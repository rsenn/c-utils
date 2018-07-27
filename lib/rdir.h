#ifndef _RDIR_H__
#define _RDIR_H__

#include "dir.h"
#include "stralloc.h"

#ifdef __cplusplus
extern "C" {
#endif

struct rdir_s;

struct rdir_s {
  stralloc sa;
  union {
    struct dir_s dir;
    void* dir_int;
  };
  struct rdir_s* prev;
};
typedef struct rdir_s rdir_t;

int rdir_open(struct rdir_s* d, const char* p);
char* rdir_read(struct rdir_s* d);
void rdir_close(struct rdir_s* d);

/*char* rdir_name(struct rdir_s* d);
int rdir_type(struct rdir_s* d);
time_t rdir_time(struct rdir_s* d, int time_type);
*/
#ifdef __cplusplus
}
#endif

#endif // _RDIR_H__
