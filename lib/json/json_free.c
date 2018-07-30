#include "../json.h"
#include <stdlib.h>
static size_t
json_free_val(jsonval* val) {
  switch(val->type) {
    case JSON_OBJECT:
      break;
    case JSON_ARRAY:
      break;
    default:
      free(val);
      return 1;
  }
  return 0;
}

void
json_free(jsonval* val) {
  size_t num = json_free_val(val);
#ifdef DEBUG
  buffer_puts(buffer_2, "vals free'd: ");
  buffer_putulong(buffer_2, num);
  buffer_putnlflush(buffer_2);
#endif
}
