#include "../xml.h"

static size_t
xml_path_out(const xmlnode* node, stralloc* out, int recurse) {
  size_t n = 0;

  if(node->parent && recurse) {
    n += xml_path_out(node->parent, out, recurse - 1);
  }
  ++n;
  stralloc_catc(out, '/');

  if(node->type != XML_DOCUMENT)
    stralloc_cats(out, node->type == XML_TEXT ? "#text" : node->name);

  return n;
}

int
xml_path(const xmlnode* node, stralloc* out) {
  stralloc_zero(out);
  return xml_path_out(node, out, -1) != 0;
}

int
xml_path_relative(const xmlnode* node, stralloc* out) {
  stralloc_zero(out);
  return xml_path_out(node, out, 1) != 0;
}
