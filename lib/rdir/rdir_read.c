#include "../rdir.h"
#include "../buffer.h"
#include "../byte.h"
#include "../dir_internal.h"
#include "../stralloc.h"

#include <assert.h>
#include <stdlib.h>

void rdir_read_r(rdir_t* d);
static void rdir_push(rdir_t* d);
static void rdir_pop(rdir_t* d);

char*
rdir_read(rdir_t* d) {
  size_t len;
  char* s;

  for(;;) {
    if((s = dir_read(&d->dir))) {
      int type = dir_type(&d->dir);
      if(!(str_diff(s, "..") && str_diff(s, "."))) continue;

      if(d->sa.s[d->sa.len - 1] != DIRSEP_C) {
        stralloc_catc(&d->sa, DIRSEP_C);
      }
      len = d->sa.len;

      if(type == D_DIRECTORY) rdir_push(d);

      stralloc_cats(&d->sa, s);
      stralloc_nul(&d->sa);

      if(type == D_DIRECTORY) {
#ifdef DEBUG_OUTPUT
        buffer_puts(buffer_2, "entering ");
        buffer_puts(buffer_2, d->sa.s);
        buffer_putnlflush(buffer_2);
#endif

        dir_open(&d->dir, d->sa.s);
      } else {
        d->sa.len = len;
      }

      return d->sa.s;

    } else {
      dir_close(&d->dir);

      if(d->prev) {
        rdir_pop(d);

        continue;
      } else {
        break;
      }
    }
  }

  return NULL;
}

static void
rdir_push(rdir_t* d) {
  rdir_t* rdn = malloc(sizeof(rdir_t));
  byte_copy(rdn, sizeof(rdir_t), d);
  byte_zero(d, sizeof(rdir_t));
  d->prev = rdn;

  d->sa = rdn->sa;
}

static void
rdir_pop(rdir_t* d) {
  stralloc sa;
  rdir_t* prev = d->prev;
  assert(prev);

  sa = d->sa;
  sa.len = d->prev->sa.len;

  byte_copy(d, sizeof(rdir_t), prev);
  d->sa = sa;
  free(prev);
}
