#include "../alloc.h"
#include "../windoze.h"
#include "../typedefs.h"

/* env.c, envread.c, env.h: environ library
Daniel J. Bernstein, djb@silverton.berkeley.edu.
Requires environ.
19960113: rewrite. warning: interface is different.
No known patent problems.
*/

#include "../env.h"
#include "../byte.h"
#include "../str.h"

#include <stdlib.h>

#if WINDOWS_NATIVE

#if defined(_MSC_VER) && !defined(__POCC__)
int
env_put2(const char* name, const char* value) {
  return !!_putenv_s(name, value);
}

int
env_putb(const char* x, size_t n) {
  size_t namelen = byte_chr(x, n, '=');
  char* name;
  const char* value;
  int ret;

  if(namelen == n)
    return 0;
  name = str_ndup(x, namelen);
  value = str_ndup(&x[namelen + 1], n - (namelen + 1));
  ret = env_put2(name, value);
  free((void*)name);
  free((void*)value);
  return ret;
}

int
env_put(const char* s) {
  return env_putb(s, str_len(s));
}
#else

int
env_put(const char* s) {
  return !putenv(s);
}

int
env_putb(const char* x, size_t n) {
  char* tmp = str_ndup(x, n);
  int ret = env_put(tmp);
  free(tmp);
  return ret;
}

int
env_put2(const char* name, const char* value) {
  size_t len = str_len(name) + str_len(value) + 2;
  char* tmp;
  int ret = 0;

  if((tmp = malloc(len))) {
    str_copy(tmp, name);
    str_cat(tmp, "=");
    str_cat(tmp, value);
    ret = env_put(tmp);
    free(tmp);
  }
  return ret;
}

#endif

#else
static size_t env_isinit = 0; /* if env_isinit: */
static size_t ea;             /* environ is a pointer to ea+1 char*'s. */
static size_t en;             /* the first en of those are ALLOCATED. environ[en] is 0. */

static inline void
env_goodbye(size_t i) {
  free(environ[i]);
  environ[i] = environ[--en];
  environ[en] = 0;
}

static char* null = 0;

void
env_clear(void) {
  if(env_isinit) {
    while(en) {
      env_goodbye(0);
    }
  } else {
    environ = &null;
  }
}

static void
env_unsetlen(const char* s, size_t len) {
  int i;

  for(i = (int)en - 1; i >= 0; --i) {
    if(environ[i] && !str_diffn(s, environ[i], len)) {
      if(environ[i][len] == '=') {
        env_goodbye((size_t)i);
      }
    }
  }
}

static size_t
_env_unset(const char* s) {
  if(!env_isinit) {
    if(!env_init())
      return 0;
  }
  env_unsetlen(s, str_len(s));
  return 1;
}

#define EXPAND 30

static size_t
env_expand(void) {
  char** newenviron;

  if(en != ea)
    return 1;

  newenviron = (char**)malloc((size_t)((ea + EXPAND + 1) * sizeof(char*)));

  if(!newenviron)
    return 0;
  ea += 30;

  byte_copy((char*)newenviron, (size_t)((en + 1) * sizeof(char*)), (char*)environ);
  free((void*)environ);
  environ = newenviron;
  return 1;
}

static const char*
env_findeq(const char* s) {
  while(*s) {
    if(*s == '=')
      return s;
    ++s;
  }
  return 0;
}

static size_t
env_add(char* s) {
  const char* t;
  t = env_findeq(s);

  if(t) {
    env_unsetlen(s, (size_t)(t - s));
  }

  if(!env_expand())
    return 0;
  environ[en++] = s;
  environ[en] = 0;
  return 1;
}

int
env_putb(const char* x, size_t n) {
  char* u;

  if(!env_isinit) {
    if(!env_init())
      return 0;
  }
  u = (char*)alloc(n + 1);

  if(!u)
    return 0;
  str_copyn(u, x, n);

  if(!env_add(u)) {
    free(u);
    return 0;
  }
  return 1;
}

int
env_put(const char* s) {
  return env_putb(s, str_len(s));
}

int
env_put2b(const char* s, const char* t, size_t n) {
  char* u;
  size_t slen;

  if(!env_isinit) {
    if(!env_init())
      return 0;
  }
  slen = str_len(s);
  u = (char*)alloc(slen + n + 2);

  if(!u)
    return 0;
  str_copy(u, s);
  u[slen] = '=';
  byte_copy(u + slen + 1, n, t);
  u[slen + 1 + n] = '\0';

  if(!env_add(u)) {
    free(u);
    return 0;
  }
  return 1;
}

int
env_put2(const char* s, const char* t) {
  return env_put2b(s, t, str_len(t));
}

size_t
env_init(void) {
  char** newenviron;
  size_t i;

  for(en = 0; environ[en]; ++en) {
    ;
  }
  ea = en + 10;
  newenviron = (char**)malloc((size_t)((ea + 1) * sizeof(char*)));

  if(!newenviron)
    return 0;

  for(en = 0; environ[en]; ++en) {
    newenviron[en] = (char*)alloc(str_len(environ[en]) + 1);

    if(!newenviron[en]) {
      for(i = 0; i < en; ++i) {
        free(newenviron[i]);
      }
      free((char*)newenviron);
      return 0;
    }
    str_copy(newenviron[en], environ[en]);
  }
  newenviron[en] = 0;
  environ = newenviron;
  env_isinit = 1;
  return 1;
}

#endif
