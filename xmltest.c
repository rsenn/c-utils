#include "lib/xml.h"
#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/hmap.h"
#include "lib/iterator.h"
#include "lib/stralloc.h"
#include "lib/strlist.h"
#include "lib/str.h"
#include "lib/mmap.h"
#include "lib/map.h"

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

static strlist vars;

void
xml_dump(xmlnode* n, buffer* b, const char* parent, int depth) {
  stralloc name;
  stralloc_init(&name);
  int text_children;

  do {
    text_children = 0;
    if(n->type == XML_DOCUMENT)
      n = n->children;
    stralloc_zero(&name);
    if(n->type == XML_TEXT) {
      const char* x = xml_get_text(n, &name);
      if(x[0]) {
        buffer_putm_internal(b, "", parent, "->children = xml_textnode(\"", x, "\");\n", 0);
        buffer_flush(b);
      }
    } else if(n->type == XML_ELEMENT) {
      TUPLE* t;
      if(n->name[0] == '/')
        continue;
      stralloc_decamelize(n->name, &name, '_');
      stralloc_remove_all(&name, "-.", 2);
      stralloc_lower(&name);
      stralloc_nul(&name);
      buffer_puts(b, "\n");
      /*      buffer_putnspace(b, depth * 2);
            buffer_puts(b, "// depth: ");
            buffer_putlong(b, depth);
            buffer_putnlflush(b);*/

      buffer_putnspace(b, depth * 2);

      if(n->children && n->children->next == 0 && n->children->type == XML_TEXT) {
        name.s = NULL;
        text_children = 1;
      }

      if(name.s && !text_children) {
        if(!strlist_contains(&vars, name.s)) {
          buffer_puts(b, "xmlnode* ");
          strlist_push(&vars, name.s);
        }

        buffer_putm_internal(b, name.s, " = ", 0);
      }

      if(parent) {
        buffer_putm_internal(b, "xml_child_element(\"", n->name, "\", ", parent, ")", 0);
      } else {
        buffer_putm_internal(b, "xml_element(\"", n->name, "\")", 0);
      }

      if(n->attributes) {
        hmap_foreach(n->attributes, t) {
          buffer_putm_internal(
              b, "xml_set_attribute(", name.s, ", \"", t->key, "\", \"", t->vals.val_chars, "\");\n", 0);
          buffer_putnspace(b, depth * 2);
        }
      }
    }

    if(text_children) {

      buffer_putm_internal(b, "->children", 0);
      buffer_puts(b, "\n");
      buffer_putnspace(b, depth * 2);
      buffer_putm_internal(b, "  =  xml_textnode(\"", n->children->name, "\");\n", 0);

    } else {

      buffer_puts(b, ";\n");
      buffer_putnspace(b, depth * 2);

      if(name.s && (n->type == XML_ELEMENT || n->type == XML_DOCUMENT) && n->children) {
        xml_dump(n->children, b, name.s, depth + 1);
      }
      buffer_flush(b);
    }

  } while((n = n->next));

  buffer_flush(b);
}

int
main(int argc, char* argv[1]) {
  xmlreader r;
  xmlnode* doc;
  buffer_mmapprivate(&infile, argc > 1 ? argv[1] : "../dirlist/test.xml");
  /* xml_reader_init(&r, &infile);
   xml_read_callback(&r, xml_read_function);
 */
  strlist_init(&vars, '\0');
  buffer_skip_until(&infile, "\r\n", 2);
  doc = xml_read_tree(&infile);
  if(doc) {
    xml_dump(doc, buffer_1, 0, 0);
    xml_free(doc);
  }

  buffer_close(&infile);
}
