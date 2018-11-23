#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/hmap.h"
#include "lib/iterator.h"
#include "lib/stralloc.h"
#include "lib/xml.h"
#include "lib/str.h"
#include "lib/mmap.h"
#include "lib/scan.h"
#include "lib/strlist.h"
#include "lib/array.h"

static void xml_print_list(xmlnode*, buffer*, int, const char*);

static int
xml_num_children(xmlnode* node) {
  int n = 0;
  for(node = node->children; node; node = node->next) {
    n += xml_num_children(node);
    ++n;
  }
  return n;
}

static void
xml_print_node(xmlnode* node, buffer* b, int depth, const char* nl) {
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
  buffer_putm_internal(b, "<", node->name, 0);
 
  if(node->attributes && node->attributes->tuple_count) {
    buffer_putc(b,' ');
    xml_print_attributes(node->attributes, b, " ", "=", "\"");
  }

  if(node->children) {
    int only_text_children = (node->children->type == XML_TEXT);
    buffer_puts(b, ">");

    if(only_text_children) {
      xml_print_list(node->children, b, 0, "\n");
    } else if(xml_num_children(node) == 1) {
      xml_print_list(node->children, b, 0, "");
    } else {
      buffer_puts(b, nl);
      xml_print_list(node->children, b, depth + 1, "\n");
      buffer_putnspace(b, depth * 2);
    }
  } else if(node->name[0] == '/' || (node->next && node_is_closing(node->next))) {
    buffer_putc(b, '>');
  } else {
    buffer_puts(b, node->name[0] == '?' ? "?>" : "/>");
    closing = 1;
  }

//  if(node->next && depth == 0) {
//    xmlnode* next = node->next;
//    if(node_is_closing(next) && !str_diff(&next->name[1], node->name)) return xml_print_node(next, b, depth);
//  }
  if(closing) buffer_puts(b, nl);
   buffer_flush(b);
}

static void
xml_print_list(xmlnode* node, buffer* b, int depth, const char* nl) {
  do {
    xml_print_node(node, b, depth, nl);
  } while((node = node->next));
}

static void
xml_print_tree(xmlnode* node, buffer* b) {
  if(node->type == XML_DOCUMENT) {
    buffer_puts(b, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    node = node->children;
  }
  
  (node->parent ? xml_print_node : xml_print_list)(node, b, 0, "\n");
}

int
main(int argc, char* argv[]) {
  buffer input;
  xmlnode* doc;
  static xmlnodeset ns;
  xmlnodeset_iter_t it, e;
  size_t i = 0;

  if(!argv[1]) {
    argv[1] = "C:\\Users\\roman\\Desktop\\dirlist\\pelist.cbp";
  }

  buffer_mmapprivate(&input, argv[1]);
  buffer_skip_until(&input, "\r\n", 2);
  doc = xml_read_tree(&input);
  xml_print_tree(doc->children, buffer_1);
/*
  for(it = xmlnodeset_begin(&ns), e = xmlnodeset_end(&ns); it != e; ++it) {
    xmlnode* n = *it;
    buffer_puts(buffer_1, "NODESET[");
    buffer_putlong(buffer_1, i++);
    buffer_puts(buffer_1, "]: ");
    xml_debug(n, buffer_1);
    buffer_putnlflush(buffer_1);
  }
  ns = xml_find_all_1(doc, xml_match_name, "Unit");
  xml_print_nodeset(&ns, buffer_1);
  buffer_putlong(buffer_1, xmlnodeset_size(&ns));
  buffer_putnlflush(buffer_1);*/
  /*
   * Cleanup function for the XML library.
   */
  xml_free(doc);
  return (0);
}

