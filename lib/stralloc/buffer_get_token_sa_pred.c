#include "../buffer.h"
#include "../stralloc.h"

/* Like buffer_get_token_sa, but the token ends when the predicate says so: it
 * is given the stralloc as it is and returns 1 when the token is complete, 0 to
 * read on, -1 on an error. (libowfat's signature; see
 * buffer_get_token_sa_pred_arg for a predicate with an argument.) */
int
buffer_get_token_sa_pred(buffer* b, stralloc* sa, sa_predicate p) {
  for(;;) {
    char x;

    if(!stralloc_readyplus(sa, 1))
      return -1;

    switch(buffer_getc(b, &x)) {
      case -1: return -1;
      case 0: return 0;
    }

    stralloc_append(sa, &x);

    switch(p(sa)) {
      case -1: return -1;
      case 0: break;
      case 1: return 0;
    }
  }
}
