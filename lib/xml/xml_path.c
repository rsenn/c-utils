#include "../xml.h"

static size_t
xml_path_out(const xmlnode* node, stralloc* out, int recurse) {
  size_t n = 0;

  if(node->parent && recurse)
    n += xml_path_out(node->parent, out, recurse - 1);

  ++n;
  if(node->type != XML_DOCUMENT) { /* the document is the empty prefix: "/root", not "//root" */
    stralloc_catc(out, '/');
    stralloc_cats(out, node->type == XML_TEXT ? "#text" : node->name);
  }
  return n;
}

/* the path of node from the document: /root/item/b ("/" for the document itself) */
int
xml_path(const xmlnode* node, stralloc* out) {
  stralloc_zero(out);
  if(xml_path_out(node, out, -1) == 0)
    return 0;
  if(out->len == 0)
    stralloc_catc(out, '/');
  return 1;
}

/* the path from the parent of node: /item/b */
int
xml_path_relative(const xmlnode* node, stralloc* out) {
  stralloc_zero(out);
  return xml_path_out(node, out, 1) != 0;
}
