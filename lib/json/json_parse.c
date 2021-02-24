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
  charbuf_skip_pred(b, &isspace);

  if((r = json_object_parse(j, b)) < 1)
    if((r = json_array_parse(j, b)) < 1)
      if((r = json_null_parse(j, b)) < 1)
        if((r = json_boolean_parse(j, b)) < 1)
          if((r = json_number_parse(j, b)) < 1)
            r = json_string_parse(j, b);
  return r;
}
