#include "../json_internal.h"
#include "../bool.h"
#include "../byte.h"
#include "../charbuf.h"
#include "../scan.h"
#include "../stralloc.h"

#include <assert.h>
#include <ctype.h>

/* Objects and arrays are read by recursion; a document that nests deeper than
 * this is refused instead of overflowing the stack. (The depth is kept in a
 * static, as the reader has no context to carry it in.) */
#define JSON_MAX_DEPTH 512

static int json_depth;

static int json_parse_value(jsonval* j, charbuf* b);

int
json_parse(jsonval* j, charbuf* b) {
  int r;

  if(json_depth >= JSON_MAX_DEPTH)
    return 0;

  json_depth++;
  r = json_parse_value(j, b);
  json_depth--;

  return r;
}

static int
json_parse_value(jsonval* j, charbuf* b) {
  int r = 0;
  charbuf_pred_skip(b, charbuf_is_ctype, isspace);

  if((r = json_parse_object(j, b)) < 1)

    if((r = json_parse_array(j, b)) < 1)

      if((r = json_parse_null(j, b)) < 1)

        if((r = json_parse_boolean(j, b)) < 1)

          if((r = json_parse_number(j, b)) < 1)
            r = json_parse_string(j, b);
  return r;
}
