#ifndef _PATH_H__
#define _PATH_H__

#include "stralloc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PATH_FNM_NOMATCH     1
#define PATH_FNM_PATHNAME    (1 << 0) /* No wildcard can ever match /'.  */
#define PATH_FNM_NOESCAPE    (1 << 1) /* Backslashes don't quote special chars.  */
#define PATH_FNM_PERIOD      (1 << 2) /* Leading .' is matched only explicitly.  */

char* path_basename(char* path);
int   path_canonicalize(const char* path, stralloc* sa, int symbolic);
char* path_dirname(char* path);
int   path_fnmatch(const char* pattern, unsigned int plen, const char* string, unsigned int slen, int flags);
void  path_getcwd(stralloc* sa, unsigned long start);
char* path_gethome(int uid);
int   path_readlink(const char* path, stralloc* sa);
int   path_realpath(const char* path, stralloc* sa, int symbolic, stralloc* cwd);

#define path_isabs(p) ((p)[0] == '/') 
#define path_isrel(p) ((p)[0] != '/') 
#define path_isname(p) ((p)[str_chr((p), '/')] != '\0')

#ifdef __cplusplus
}
#endif

#endif /* _PATH_H__ */
/* path_time.c */
