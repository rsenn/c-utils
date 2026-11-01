#include "../json.h"
#include "../byte.h"

/* parses the line that is complete, returns 1 if it gave a value */
static int
ndjson_line(ndjson* nd, ndjson_callback* cb, void* user) {
  size_t n = nd->line.len;
  jsonval* doc;

  /* a CR LF line end, and lines that are empty or only white space are not values */
  while(n && (nd->line.s[n - 1] == '\r' || nd->line.s[n - 1] == ' ' || nd->line.s[n - 1] == '\t'))
    n--;

  if(n == 0)
    return 0;

  if(!(doc = json_parse_text(nd->line.s, n))) {
    nd->errors++;
    return 0;
  }

  nd->lines++;
  if(cb)
    cb(doc, user);
  json_free(doc);
  return 1;
}

/* Parses n bytes of the stream, calls cb with every value a line completes
 * (the value is released when the callback returns) and returns how many
 * that were. A line that does not end before the stream does is not used. */
size_t
ndjson_feed(ndjson* nd, const char* x, size_t n, ndjson_callback* cb, void* user) {
  size_t values = 0;

  while(n) {
    size_t i = byte_chr(x, n, '\n');

    stralloc_catb(&nd->line, x, i);

    if(i == n)
      break;

    values += ndjson_line(nd, cb, user);
    nd->line.len = 0;

    x += i + 1;
    n -= i + 1;
  }
  return values;
}
