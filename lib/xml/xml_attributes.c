#include "../xml.h"
#include "../str.h"
#include "../buffer.h"
#include "../hmap.h"
#include <stdarg.h>

HMAP_DB*
xml_vattributes(const char* arg, va_list args) {
  HMAP_DB* db = 0;
  const char* value;
  size_t i, nlen, vlen;

  do {
    nlen = str_len(arg);
    if(arg[i = str_chr(arg, '=')]) {
      vlen = nlen - (i + 1);
      nlen = i;
      value = &arg[i + 1];
    } else {
      nlen = str_len(arg);
      if((value = va_arg(args, const char*)) == 0)
        break;
      vlen = str_len(value);
    }
    if(value == 0)
      break;

    if(db == 0)
      hmap_init(XML_HMAP_BUCKETS, &db);
    hmap_set(&db, arg, nlen, (char*)value, vlen + 1);

  } while((arg = va_arg(args, const char*)));

  hmap_dump(db, buffer_2);
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
