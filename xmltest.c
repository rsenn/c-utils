#include "lib/xml.h"
#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/hmap.h"
#include "lib/iterator.h"
#include "lib/stralloc.h"
#include "lib/str.h"
#include "lib/mmap.h"

#include <assert.h>
#include <ctype.h>
#include <sys/types.h>
static buffer infile;
static buffer b;
void
put_str_escaped(buffer* b, const char* str) {
  stralloc esc;
  stralloc_init(&esc);
  stralloc_fmt_pred(&esc, str, str_len(str), (stralloc_fmt_fn*)&fmt_escapecharc, (int (*)()) & iscntrl);
  buffer_putsa(b, &esc);
}

const char* node_types[] = {
    "(null)",
    "XML_DOCUMENT",
    "XML_ELEMENT",
    "XML_TEXT",
};

static int depth = 0;

int
xml_read_function(xmlreader* reader, xmlnodeid id, stralloc* name, stralloc* value, HMAP_DB** attrs) {
  xmlnode* n;
  TUPLE* t;
  if(id == XML_TEXT) {
    if(reader->closing)
      --depth;
    if(value && value->s)
      stralloc_nul(value);
    buffer_puts(buffer_1, node_types[id - 1]);
    buffer_puts(buffer_1, " \"");
    buffer_putsa(buffer_1, value);
    buffer_puts(buffer_1, "\"");
    buffer_putnlflush(buffer_1);
    return 1;
  }
  if(id != XML_ELEMENT)
    return 1;

  if(reader->closing)
    --depth;

  buffer_putm_internal(buffer_1, "node = xml_element(\"", name ? name->s : "", "\");\n", 0);

  hmap_foreach(*attrs, t) {
    buffer_putm_internal(
        buffer_1, "node = xml_set_attribute(node, \"", t->key, "\", \"", t->vals.val_chars, "\");\n", 0);
  }
  buffer_putnlflush(buffer_1);

  /*  if(value)
      buffer_putm_2(buffer_1, ", value=", value ? value->s : "");
    buffer_puts(buffer_1, ", depth=");
    buffer_putlong(buffer_1, depth);
    buffer_puts(buffer_1, ", closing=");
    buffer_putlong(buffer_1, reader->closing);
    buffer_puts(buffer_1, ", self_closing=");
    buffer_putlong(buffer_1, reader->self_closing);*/
  buffer_putnlflush(buffer_1);
  if(!reader->closing && !reader->self_closing)
    ++depth;
  return 1;
}

void
xml_dump(xmlnode* n, buffer* b) {
  do {
 
    if(n->type == XML_TEXT) {
      buffer_puts(b, " \"");
      put_str_escaped(b, n->name);
      buffer_puts(b, "\"");
    } else if(n->type == XML_ELEMENT) {
      TUPLE* t;

      buffer_putm_internal(b, "node = xml_element(\"", n->name, "\");\n", 0);

      hmap_foreach(n->attributes, t) {
        buffer_putm_internal(b, "xml_set_attribute(node, \"", t->key, "\", \"", t->vals.val_chars, "\");\n", 0);
      }

      //  xml_print_attributes(n->attributes, b, ", ", ":", "");
    }
    buffer_putnlflush(b);

    if((n->type == XML_ELEMENT||n->type == XML_DOCUMENT) && n->children) {

      xml_dump(n->children, b);
    }

  } while((n = n->next));
}

int
main(int argc, char* argv[1]) {
  xmlreader r;
  xmlnode* doc;
  buffer_mmapprivate(&infile, argc > 1 ? argv[1] : "../dirlist/test.xml");
  /* xml_reader_init(&r, &infile);
   xml_read_callback(&r, xml_read_function);
 */

  buffer_skip_until(&infile, "\r\n", 2);

  doc = xml_read_tree(&infile);

  xml_dump(doc, buffer_1);
  // xml_walk(doc, testwalk);

  xml_free(doc);

  buffer_close(&infile);
}
