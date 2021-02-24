#include "../uint64.h"
#include "../fmt.h"
#include "../json_internal.h"

jsonval*
js_property_set_int(jsonval* obj, int64 index, jsonval value) {
  char buf[128];
  jsonval* ret;
  size_t i = 0;
  i = fmt_longlong(buf, index);
  buf[i] = '\0';
  ret = js_property_set_str(obj, buf, value);

  if(ret)
    *ret = value;

  return ret;
}
