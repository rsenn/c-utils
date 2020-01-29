#include "../xml.h"
#include "../str.h"
#include "../scan.h"

char*
xml_get_text(xmlnode* node, stralloc* out) {
  xmlnode* tnode = node;
  if(node->type != XML_TEXT) {
    if(node->next && node->next->type == XML_TEXT)
      node = node->next;
    else if(node->children && node->children->type == XML_TEXT)
      node = node->children;
  }
  while(node && node->type == XML_TEXT) {
    const char* x = node->name;
    ssize_t i, n = str_len(x);
    i = scan_whitenskip(x, n);
    x += i;
    n -= i;
    if(n >= 0) {
      stralloc_catb(out, x, n);
      //  stralloc_trimr(out, " \t\r\n", 4);
    }

    node = node->next;
  }
  stralloc_nul(out);
  return out->s;
}
