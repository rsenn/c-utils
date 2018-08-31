#ifndef _PATH_H__
#define _PATH_H__

#include "stralloc.H"

#ifdef __cplusplus
extern "C" {
#endif

int path_readlink(const char* path, stralloc* sa);

#ifdef __cplusplus
}
#endif

#endif // _PATH_H__
/* path_time.c */
