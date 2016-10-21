#ifndef STRLIST_H
#define STRLIST_H

#include <sys/types.h>


#include "stralloc.h"
#include "uint64.h"

#ifdef __cplusplus
extern "C" {
#endif

/* strlist is the internal data structure all functions are working on.
 */

typedef struct strlist {
  stralloc sa;
} strlist;

/* strlist_init will initialize a strlist. */
#define strlist_init stralloc_init
#define strlist_free stralloc_free
#define strlist_zero stralloc_zero

size_t
strlist_count(strlist* sl);

const char*
strlist_at(strlist* sl, size_t pos);

int64
strlist_index_of(strlist* sl, const char* str);

int
strlist_push(strlist* sl, const char* str);

int
strlist_pushb(strlist* sl, const char* s, size_t n);

int
strlist_pushsa(strlist* sl, const stralloc *sa);

int
strlist_pushunique(strlist* sl, const char* str);


#ifdef __cplusplus
}
#endif
#endif
