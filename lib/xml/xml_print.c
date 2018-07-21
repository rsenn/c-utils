#include "../xml.h"

static void xml_print_list(xmlnode*, buffer*, int);

static void
xml_print_node(xmlnode* node, buffer* b, int depth) {
  int closing = node_is_closing(node);
  if(node->type == XML_TEXT) {
    stralloc text;
    stralloc_init(&text);
    xml_escape(node->name, str_len(node->name), &text);
    buffer_putsa(b, &text);
    stralloc_free(&text);
    return;
  }
  if(!closing) buffer_putnspace(b, depth * 2);
  buffer_putm(b, "<", node->name);
  if(node->attributes) xml_print_attributes(node, b, " ", "=", "\"");
  if(node->children) {
    buffer_puts(b, ">");
    int only_text_children = (node->children->type == XML_TEXT);
    if(only_text_children) {
      xml_print_list(node->children, b, 0);
    } else {
      buffer_puts(b, "\n");
      xml_print_list(node->children, b, depth + 1);
      buffer_putnspace(b, depth * 2);
    }
  } else if(node->name[0] == '/' ||
            (node->next && node_is_closing(node->next))) {
    buffer_putc(b, '>');
  } else {
    buffer_puts(b, node->name[0] == '?' ? "?>" : "/>");
    closing = 1;
  }

  if(node->next && depth == 0) {
    xmlnode* next = node->next;
    if(node_is_closing(next) && !str_diff(&next->name[1], node->name))
      return xml_print_node(next, b, depth);
  }

  (closing ? buffer_putnlflush : buffer_flush)(b);
}

static void
xml_print_list(xmlnode* node, buffer* b, int depth) {
  do { xml_print_node(node, b, depth); } while((node = node->next));
}

void
xml_print(xmlnode* node, buffer* b) {
 (node->parent ? xml_print_node : xml_print_list)(node, b, 0);
}
