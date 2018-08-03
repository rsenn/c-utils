#include "../xml.h"
#include "../stralloc.h"
#include "../hmap.h"

int
xml_get_attribute_sa(xmlnode* n, stralloc* sa, const char* name) {
  TUPLE* t;
  if(hmap_search(n->attributes, (char*)name, str_len(name), &t) == HMAP_SUCCESS) {
    stralloc_copyb(sa, t->vals.val_chars, t->data_len);
    if(sa->len > 0 && sa->s[sa->len - 1] == '\0') --sa->len;
    return 1;
  }
  return 0;
}
