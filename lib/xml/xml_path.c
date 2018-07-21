#include "../xml.h"

static int
xml_path_out(const xmlnode* node, stralloc* out) {
  size_t n = 0;
  if(node->parent) {
    n += xml_path_out(node->parent, out);
  }
  ++n;
  stralloc_catc(out, '/');
  stralloc_cats(out, node->type == XML_TEXT ? "#text" : node->name);
  return n;
}

int
xml_path(const xmlnode* node, stralloc* out) {
  stralloc_zero(out);
  return xml_path_out(node, out);
}
