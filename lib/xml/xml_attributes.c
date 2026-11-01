#include "../xml.h"
#include "../str.h"
#include "../buffer.h"
#include "../hmap.h"
#include <stdarg.h>

HMAP_DB*
xml_attributes(const char* arg, ...) {
  HMAP_DB* ret;
  va_list args;
  va_start(args, arg);
  ret = xml_vattributes(arg, args);
  va_end(args);
  return ret;
}
