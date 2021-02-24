#include "../stralloc.h"
#include "../json_internal.h"

/*static const char identifier_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789$_";

static int
is_identifier_char(int c, size_t pos, void* ptr) {
  return !!identifier_chars[str_chr(identifier_chars, c)];
}
*/
int
json_parse_null(jsonval* j, charbuf* b) {
  ssize_t n;

  n = charbuf_pred_lookahead(b, &tok, predicate_string, "null");

  if(n == 4)  {
    charbuf_skipn(b, 4);
    j->type = JSON_NULL;
    return 1;
  }

  return 0;
}