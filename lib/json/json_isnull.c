#include "../json_internal.h"

int
json_isnull(jsonval v) {
  return v.type == JSON_NULL; // v.type == JSON_OBJECT && MAP_ISNULL(v.dictv);
}