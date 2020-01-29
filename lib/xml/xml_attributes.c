#include "../xml.h"
#include "../str.h"
#include "../hmap.h"
#include <stdarg.h>

HMAP_DB*
xml_vattributes(const char* arg, va_list args) {
  HMAP_DB* db;
  const char* value;
  size_t len;
  hmap_init(1024, &db);
  do {
    if(arg[len = str_chr(arg, '=')]) {
      value = &arg[len + 1];
    } else {
      len = str_len(arg);
      value = va_arg(args, const char*);
    }

    hmap_set(db, arg, len, value, str_len(value) + 1);

  } while((arg = va_arg(args, const char*)));
  return db;
}

HMAP_DB*
xml_attributes(const char* arg, ...) {
  HMAP_DB* ret;
  va_list args;
  va_start(args, arg);
  ret = xml_vattributes(arg, args);
  va_end(args);
  return ret;
}