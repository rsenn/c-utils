#include <stdlib.h>
#include "../byte.h"
#include "../str.h"
#include "../rdir.h"
#include "../dir_internal.h"
#include "../buffer.h"

void rdir_read_r(rdir_t* d);

char*
rdir_read(rdir_t* pd) {
  size_t len;
  char* s;
  rdir_t* d = pd;


  if(d->sa.s && strchr(&d->sa.s[d->sa.len], '/')
      //dir_type(&d->dir) == D_DIRECTORY && d->sa.s
      ) {
  //if(dir_type(&d->dir) == D_DIRECTORY && d->sa.s) {

    buffer_puts(buffer_2, "&d->sa.s[d->sa.len] = ");
    buffer_puts(buffer_2, &d->sa.s[d->sa.len]);
    buffer_puts(buffer_2, " // ");
    buffer_puts(buffer_2, strchr(&d->sa.s[d->sa.len], '/'));
    buffer_putnlflush(buffer_2);

    //if(str_diff(&d->sa.s[d->sa.len], "./") && str_diff(&d->sa.s[d->sa.len], "../"))
         rdir_read_r(d);
  }

  for(;;) {
    if((s = dir_read(&d->dir))) {
      if(!(str_diff(s, "..") && str_diff(s, ".")))
        continue;

      if(d->sa.s[d->sa.len-1] != '/') {
        stralloc_catc(&d->sa, '/');
      }
      len = d->sa.len;
      stralloc_cats(&d->sa, s);

      if(dir_type(&d->dir) == D_DIRECTORY)
        stralloc_catc(&d->sa, '/');

      stralloc_0(&d->sa);
      d->sa.len = len;
      return d->sa.s;

    } else if(d->prev) {
      //d->prev->sa.s = d->sa.s;
      //d->prev->sa.a = d->sa.a;
      stralloc_free(&d->sa);
      dir_close(&d->dir);
      byte_copy(d, sizeof(rdir_t), d->prev);
      free(d->prev);
      //byte_copy(d, sizeof(rdir_t), d->prev);

      //stralloc_0(&d->sa);
      //*d = *d->prev;

      continue;
    }
  }

  return NULL;
}

void
rdir_read_r(rdir_t* d) {
  rdir_t* rdn = malloc(sizeof(rdir_t));
  byte_copy(rdn, sizeof(rdir_t), d);
  byte_zero(d, sizeof(rdir_t));
  d->prev = rdn;

  stralloc_copys(&d->sa, rdn->sa.s);
  stralloc_0(&d->sa);

  if(dir_open(&d->dir, d->sa.s)) {
    byte_copy(d, sizeof(rdir_t), rdn);
    free(rdn);
    return;
  }


  d->sa.len = str_len(d->sa.s);



    buffer_puts(buffer_2, "entering ");
    buffer_puts(buffer_2, d->sa.s);
    buffer_putnlflush(buffer_2);

  //return rdir_read(rdn);
}

