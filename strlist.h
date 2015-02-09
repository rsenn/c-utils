#ifndef STRLIST_H
#define STRLIST_H

#include <sys/types.h>
#include "stralloc.h"

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

size_t
strlist_count(strlist* sl);

const char*
strlist_at(strlist* sl, size_t pos);

ssize_t
strlist_index_of(strlist* sl, const char* str);

int
strlist_append(strlist* sl, const char* str);

int
strlist_append_unique(strlist *sl, const char *str);


#ifdef __cplusplus
}
#endif
#endif
