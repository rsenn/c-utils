#include "../xml.h"


static void
xml_dump_node(xmlnode* node, buffer* b, int depth) {
  do {
    buffer_putnspace(b, depth * 2);
    buffer_putm(b, "<", node->name);

    if(node->attributes)
      xml_dump_attributes(node, b, " ", "=", "\"");

    if(node->children) {
      buffer_puts(b, ">\n");
      xml_dump_node(node->children, b, depth + 1);
      buffer_putnspace(b, depth * 2);
      buffer_putm(b, "</", node->name, ">");
    } else {
      buffer_puts(b, node->name[0] == '?' ? "?>" : "/>");
    }

    buffer_putnlflush(b);
  } while((node = node->next));
}

void
xml_dump(xmlnode* node, buffer* b) {
  xml_dump_node(node, b, 0);
}
