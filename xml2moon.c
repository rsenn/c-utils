#include "lib/uint64.h"
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
#include "lib/textcode.h"

static void xml_print_list(xmlnode*, buffer*, int, const char*);

void
xml_print_attrs(HMAP_DB* db, buffer* b) {
  TUPLE* tpl = db->list_tuple;

  while(tpl) {
    buffer_putm_internal(b, tpl == db->list_tuple ? "" : ", ", NULL);
    if(tpl->key[str_chr(tpl->key, '-')])
      buffer_putm_internal(b, "\"", tpl->key, "\"", NULL);
    else
      buffer_puts(b, tpl->key);

    buffer_putm_internal(b, ": ", "\"", NULL);

    switch(tpl->data_type) {
      case HMAP_DATA_TYPE_INT: buffer_putlong(b, tpl->vals.val_int); break;
      case HMAP_DATA_TYPE_UINT: buffer_putulong(b, tpl->vals.val_uint); break;
      case HMAP_DATA_TYPE_INT64: buffer_putlonglong(b, tpl->vals.val_longlong); break;
      case HMAP_DATA_TYPE_UINT64: buffer_putulonglong(b, tpl->vals.val_ulonglong); break;
      case HMAP_DATA_TYPE_DOUBLE: buffer_putdouble(b, tpl->vals.val_double, 15); break;
      case HMAP_DATA_TYPE_CHARS: buffer_put(b, tpl->vals.val_chars, tpl->data_len - 1); break;
      case HMAP_DATA_TYPE_CUSTOM: buffer_putptr(b, tpl->vals.val_custom); break;
    }
    buffer_puts(b, "\"");
    if(tpl->next == db->list_tuple)
      break;
    tpl = tpl->next;
  }
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

  if(!closing) {
    buffer_putnspace(b, depth * 2);
    buffer_putm_internal(b, node->name, NULL);
  }

  if(node->attributes && node->attributes->tuple_count) {
    buffer_puts(b, " ");
    xml_print_attrs(node->attributes, b);
  }

  if(node->children) {
    int only_text_children = (node->children->type == XML_TEXT);
    static stralloc text;

    stralloc_zero(&text);

    if(only_text_children) {

      const char* content = xml_content(node);

      if((content = xml_content(node))) {
        size_t len = content ? str_len(content) : 0;

        stralloc_ready(&text, len);
        text.len = fmt_stripwhitespace(text.s, content, len);
      }
      stralloc_nul(&text);
      buffer_putm_internal(b, ", \"", text.s, "\"", NULL);

      /*xml_print_list(node->children, b, 0, " ");
      buffer_puts(b, "\"\n");
      *} else if(xml_num_children(node) == 1) {
        buffer_puts(b, ", ->\n");
        buffer_putnspace(b, (depth + 1) * 2);
        xml_print_list(node->children, b, 0, "");*/
    } else {
      if(node->attributes && node->attributes->tuple_count)
        buffer_puts(b, ",");

      buffer_puts(b, " ->\n");
      xml_print_list(node->children, b, depth + 1, "\n");
      buffer_putnspace(b, depth * 2);
    }
  } else if(node->name[0] == '/' || (node->next && node_is_closing(node->next))) {
    // buffer_putc(b, '>');
  } else {
    // buffer_puts(b, node->name[0] == '?' ? "?>" : "/>");
    closing = 1;
  }

  if(closing)
    buffer_puts(b, nl);
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
    // buffer_puts(b, "@html\"utf-8\"?>\n")
    //  buffer_puts(b, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
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
   * Cleanup function for the XML library.
   */
  xml_free(doc);
  return (0);
}
