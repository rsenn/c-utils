#include "../json_internal.h"

static const char identifier_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789$_";

int
json_null_parse(jsonval* j, charbuf* b) {
  uint8* chars;
  if((chars = charbuf_peekn(b, 5))) {
    if(byte_equal(chars, 4, "null")) {
      if(identifier_chars[str_chr(identifier_chars, chars[4])] == '\0') {
        charbuf_skipn(b, 4);
        j->type = JSON_NULL;
        return 1;
      }
    }
  }
  return 0;
}