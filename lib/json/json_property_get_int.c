#include "../fmt.h"
#include "../json_internal.h"

jsonval
json_property_get_int(jsonval obj, int64 i) {
  jsonval ret = {JSON_UNDEFINED};

  if(obj.type == JSON_ARRAY) {
    struct slink* p;

    for(p = obj.listv; p; p = p->next) {
      if(i-- == 0) {
        ret = *(jsonval*)slist_data(p);
        break;
      }
    }
  }
  else if(obj.type == JSON_OBJECT) {
    /* an object has no indices: the number is the key, as json_property_set_int stores it */
    char buf[FMT_LONG];

    buf[fmt_longlong(buf, i)] = '\0';
    ret = json_property_get_str(obj, buf);
  }
  return ret;
}
