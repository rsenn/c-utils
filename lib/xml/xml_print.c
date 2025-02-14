#include "../xml.h"
#include "../str.h"

static void xml_print_list(xmlnode*, buffer*, int, xml_print_fmt_t* fmt);
static void
default_fmt(xmlnode* node, buffer* b, int depth, char ch, int n) {
  int inner = depth < 0;

  depth = depth < 0 ? -depth : depth;

  while(n-- > 0)
    buffer_putc(b, ch);

  /*   switch(ch) {
      case '\n':
      break;
      case ' ':
      break;
    } */
}

static void
xml_print_node(xmlnode* node, buffer* b, int depth, xml_print_fmt_t* fmt) {
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

  if(!closing) {
    fmt(node, b, depth, ' ', depth * 2);

    //  buffer_putnspace(b, depth * 2);
  }

  buffer_putm_internal(b, "<", node->name, NULL);

  if(node->type == XML_ELEMENT && node->attributes &&
     node->attributes->tuple_count) {
    // fmt(node, b, depth, ' ', 1);

    buffer_putc(b, ' ');
    xml_print_attributes(node->attributes, b, " ", "=", "\"");
    buffer_flush(b);
  }

  if(node->children) {
    int only_text_children = (node->children->type == XML_TEXT);
    buffer_puts(b, ">");

    if(only_text_children) {
      (num_children > 1 ? xml_print_list
                        : xml_print_node)(node->children, b, 0, fmt);
    } else {
      fmt(node, b, -depth, '\n', 1);

      // buffer_puts(b, "\n");
      (num_children > 1
           ? xml_print_list
           : xml_print_node)(node->children, b, depth + 1, fmt);
      fmt(node, b, -depth, ' ', depth * 2);
    }
    buffer_puts(b, "</");
    buffer_puts(b, node->name);
    buffer_putc(b, '>');

    fmt(node->parent, b, depth, '\n', 1);

  } else if(node->name[0] == '/' ||
            (node->next && node_is_closing(node->next))) {
    buffer_putc(b, '>');
  } else {
    buffer_puts(b, node->name[0] == '?' ? "?>" : " />");
    closing = 1;
  }

  //  if(node->next && depth == 0) {
  //    xmlnode* next = node->next;
  //    if(node_is_closing(next) && !str_diff(&next->name[1], node->name))
  //    return xml_print_node(next, b, depth);
  //  }
  if(closing)
    fmt(node->parent, b, depth, '\n', 1);

  buffer_flush(b);
}

static void
xml_print_list(xmlnode* list, buffer* b, int depth, xml_print_fmt_t* fmt) {
  xmlnode* node;
  for(node = list; node; node = node->next)
    xml_print_node(node, b, depth, fmt);
}

void
xml_print(xmlnode* node, buffer* b, xml_print_fmt_t* fmt) {
  if(node->type == XML_DOCUMENT)
    node = node->children;

  if(!(node->name && node->name[0] == '?'))
    buffer_puts(b, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");

  (node && node->next
       ? xml_print_list
       : xml_print_node)(node, b, 0, fmt ? fmt : &default_fmt);
}
