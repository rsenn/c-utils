#ifndef ENV_H
#define ENV_H

#include <stdlib.h>
#include "typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

extern char** environ;

void env_clear(void);
const char* env_get2(const char* const* envp, const char* s);
const char* env_get(const char* s);
size_t env_init(void);
size_t env_len(const char* const* e);
int env_make(const char** v, size_t argc, const char* s, size_t len);
size_t
env_merge(const char** v, size_t vmax, const char* const* envp, size_t envlen, const char* modifs, size_t modiflen);
char* env_pick(void);
int env_put2(const char* s, const char* t);
int env_put2b(const char* s, const char* t, size_t n);
int env_putb(const char*, size_t);
int env_put(const char* s);
int env_set(const char* name, const char* value);
int env_unset(const char* name);

#ifdef STRALLOC_H
int env_addmodif(stralloc* sa, const char* s, const char* t);
int env_string(stralloc* sa, const char* const* envp, size_t envlen);
#endif

#ifdef __cplusplus
}
#endif

#endif
