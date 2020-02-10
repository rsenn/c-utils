#include "../xml.h"
#include "../str.h"

static void xml_print_list(xmlnode*, buffer*, int);

static void
xml_print_node(xmlnode* node, buffer* b, int depth) {
  int closing;
  size_t num_children;

  // start:
  closing = node_is_closing(node);
  num_children = node->children == 0 ? 0 : node->children->next ? 2 : 1;

  if(node->type == XML_TEXT) {
    stralloc text;
    stralloc_init(&text);
    xml_escape(node->name, str_len(node->name), &text);
    buffer_putsa(b, &text);
    stralloc_free(&text);
    return;
  }

  if(!closing)
    buffer_putnspace(b, depth * 2);

  buffer_putm_internal(b, "<", node->name, 0);

  if(node->type == XML_ELEMENT && node->attributes && node->attributes->tuple_count) {
    buffer_putc(b, ' ');
    xml_print_attributes(node->attributes, b, " ", "=", "\"");
    buffer_flush(b);
  }

  if(node->children) {
    int only_text_children = (node->children->type == XML_TEXT);
    buffer_puts(b, ">");

    if(only_text_children) {
      (num_children > 1 ? xml_print_list : xml_print_node)(node->children, b, 0);
    } else {
      buffer_puts(b, "\n");
      (num_children > 1 ? xml_print_list : xml_print_node)(node->children, b, depth + 1);
      buffer_putnspace(b, depth * 2);
    }
    buffer_puts(b, "</");
    buffer_puts(b, node->name);
    buffer_puts(b, ">\n");

  } else if(node->name[0] == '/' || (node->next && node_is_closing(node->next))) {
    buffer_putc(b, '>');
  } else {
    buffer_puts(b, node->name[0] == '?' ? "?>" : " />");
    closing = 1;
  }

  //  if(node->next && depth == 0) {
  //    xmlnode* next = node->next;
  //    if(node_is_closing(next) && !str_diff(&next->name[1], node->name)) return xml_print_node(next, b, depth);
  //  }
  if(closing)
    buffer_putnlflush(b);
  else
    buffer_flush(b);
}

static void
xml_print_list(xmlnode* list, buffer* b, int depth) {
  xmlnode* node;
  for(node = list; node; node = node->next) xml_print_node(node, b, depth);
}

void
xml_print(xmlnode* node, buffer* b) {
  if(node->type == XML_DOCUMENT) {
    buffer_puts(b, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    node = node->children;
  }

  (node && node->next ? xml_print_list : xml_print_node)(node, b, 0);
}
