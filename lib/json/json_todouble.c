#include "../stralloc.h"
#include "../scan.h"
#include "../json.h"
#include <float.h>

double
json_todouble(jsonval val) {
  double ret;
  stralloc sa;
  stralloc_init(&sa);
  json_tostring(val, &sa);
  stralloc_nul(&sa);
  if(!scan_double(sa.s, &ret))
    ret = DBL_MAX;
  stralloc_free(&sa);
  return ret;
}
