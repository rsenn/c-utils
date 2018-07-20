#include "../xml.h"

static void
xml_debug_node(xmlnode* node, buffer* b, int depth) {
  do {
    int closing = node_is_closing(node);

    if(!closing) buffer_putnspace(b, depth * 2);
    if(node->type == XML_NODE_TEXT) {
      stralloc text;
      stralloc_init(&text);
      xml_escape(node->name, str_len(node->name), &text);
      buffer_puts(b, "TEXT{");
      buffer_putsa(b, &text);
      buffer_puts(b, "},");
      stralloc_free(&text);
      continue;
    }
    buffer_putm(b, "ELEM{", node->name);
    if(node->attributes)
      xml_print_attributes(node, b, ",", ":", "'");
    if(node->children) {
      buffer_puts(b, "}");
      int only_text_children = (node->children->type == XML_NODE_TEXT);
      if(only_text_children) {
        xml_debug_node(node->children, b, 0);
      } else {
        buffer_puts(b, "\n");
        xml_debug_node(node->children, b, depth + 1);
        buffer_putnspace(b, depth * 2);
      }
    } else if(node->name[0] == '/' || (node->next && node_is_closing(node->next)))  {
      buffer_puts(b, "}");
    } else {
      buffer_puts(b, node->name[0] == '?' ? "?}" : "/}");
      closing = 1;
    }
    buffer_puts(b, ",");

    (closing ? buffer_putnlflush : buffer_flush)(b);
  } while((node = node->next));
}

void
xml_debug(xmlnode* node, buffer* b) {
  xml_debug_node(node, b, 0);
}
