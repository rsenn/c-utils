#ifndef STRLIST_H
# define STRLIST_H

# include <sys/types.h>

# include "stralloc.h"
# include "uint64.h"
# include "buffer.h"

# ifdef __cplusplus
extern "C"
{
# endif

/* strlist is the internal data structure all functions are working on.
 */

  typedef struct strlist
  {
    stralloc sa;
  } strlist;

/* strlist_init will initialize a strlist. */
# define strlist_zero(l) stralloc_zero(&(l)->sa)
# define strlist_init(l) stralloc_init(&(l)->sa)
# define strlist_free(l) stralloc_free(&(l)->sa)

    size_t strlist_count (const strlist * sl);

   char *strlist_at (const strlist * sl, size_t pos);

    int64 strlist_index_of (strlist * sl, const char *str);

  int strlist_push (strlist * sl, const char *str);
  
  int strlist_pushm (strlist * sl, ...);

  int strlist_pushb (strlist * sl, const char *s, size_t n);

  int strlist_push_sa (strlist * sl, const stralloc * sa);

  int strlist_copy (strlist * sl, const strlist* l);
  
  void strlist_dump (buffer * out, const strlist * sl);

  char** strlist_to_argv(const strlist* sl);

  int strlist_unshift(strlist * sl, const char *s);
  int strlist_shift(strlist * sl, const char** s);

#define strlist_pushm(sa, ...) strlist_pushm_internal(sa, __VA_ARGS__, (char *)0)
int strlist_pushm_internal(strlist* sl, ...);

# ifdef __cplusplus
}
# endif
#endif /* defined(STRLIST_H) */
