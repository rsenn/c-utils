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

#define strlist_pushm(sa, ...) strlist_pushm_internal(sa, __VA_ARGS__, (char *)0)

char**  strlist_to_argv(const strlist*);
char*   strlist_at(const strlist*, size_t i);
int     strlist_cat(strlist*, const strlist* l);
int     strlist_contains(strlist*, const char* s);
size_t  strlist_count(const strlist*);
void    strlist_dump(buffer*, const strlist* sl);
void    strlist_froms(strlist*, const char* s, char delim);
int64   strlist_index_of(strlist*, const char* str);
void    strlist_join(const strlist*, stralloc* sa, char delim);
int     strlist_pushb(strlist*, const char* s, size_t n);
int     strlist_pushm_internal(strlist*, ...);
int     strlist_pushsa(strlist*, const stralloc* sa);
int     strlist_push_sa(strlist*, const stralloc* sa);
void    strlist_push_tokens(strlist*, const char* s, const char* delim);
int     strlist_push_unique(strlist*, const char* s);
int     strlist_push(strlist*, const char* s);
strlist strlist_range(const strlist*, size_t from, size_t to);
int     strlist_shift(strlist*, const char** strp);
size_t  strlist_sort(strlist*);
int     strlist_unshift(strlist*, const char* s);

# ifdef __cplusplus
}
# endif
#endif /* defined(STRLIST_H) */
