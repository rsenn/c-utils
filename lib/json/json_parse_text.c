#include "../json_internal.h"
#include "../charbuf.h"
#include "../stralloc.h"

/* Parses the n bytes at x, which have to be one valid JSON value. Returns the
 * tree, to be released with json_free, or 0 if the text is not valid. */
jsonval*
json_parse_text(const char* x, size_t n) {
  stralloc copy;
  charbuf cb;
  jsonval* doc = 0;

  if(!json_valid(x, n))
    return 0;

  /* the reader works on a text that ends in a NUL */
  stralloc_init(&copy);
  if(stralloc_copyb(&copy, x, n) && stralloc_nul(&copy)) {
    charbuf_froms(&cb, copy.s, 4);
    doc = json_read_tree(&cb);
    charbuf_free(&cb);
  }
  stralloc_free(&copy);

  return doc;
}
