#include "../json_internal.h"
#include "../bool.h"
#include "../byte.h"
#include "../charbuf.h"
#include "../scan.h"
#include "../stralloc.h"

#include <assert.h>
#include <ctype.h>

int
json_parse(jsonval* j, charbuf* b) {
  int r = 0;
  charbuf_pred_skip(b, predicate_ctype, isspace);

  if((r = json_parse_object(j, b)) < 1)
    if((r = charbuf_pred_json_array_parse(j, b)) < 1)
      if((r = json_parse_null(j, b)) < 1)
        if((r = json_parse_boolean(j, b)) < 1)
          if((r = json_parse_number(j, b)) < 1)
            r = json_parse_string(j, b);
  return r;
}
