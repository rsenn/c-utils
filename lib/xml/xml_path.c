#include "../xml.h"

static int
xml_path_out(xmlnode* node, stralloc* out) {
  if(node->parent) {
    xml_path_out(node->parent, out);
  }
  stralloc_catc(out, '/');
  stralloc_cats(out, node->type == XML_NODE_TEXT ? "#text" : node->name);
}

int
xml_path(xmlnode* node, stralloc* out) {
  stralloc_zero(out);
  return xml_path_out(node, out);
}
