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
newline_indent(buffer* b, int depth) {
  buffer_putc(b, '\n');
  buffer_putnspace(b, depth * 2);
}

void
xml_dump(xmlnode* n, buffer* b, const char* parent, int depth) {
  TUPLE* t;
  int i, text_children, chained_attributes;
  xmlnode* prev = 0;
  stralloc name;
  stralloc_init(&name);
  do {
    text_children = 0;
    chained_attributes = 0;
    if(n->type == XML_DOCUMENT)
      n = n->children;
    stralloc_zero(&name);
    if(n->type == XML_TEXT) {
      const char* x = xml_get_text(n, &name);
      if(x[0]) {
        buffer_putm_internal(b, "", parent, "->children = xml_textnode(\"", x, "\");", 0);
        newline_indent(b, depth);
      }
    } else if(n->type == XML_ELEMENT) {
      if(n->name[0] == '/')
        continue;
      stralloc_decamelize(n->name, &name, '_');
      stralloc_remove_all(&name, "-.", 2);
      stralloc_lower(&name);
      stralloc_nul(&name);
      if(parent && stralloc_equals(&name, parent)) {
        long num = 1;
        if(name.len > 0) {
          size_t i = scan_noncharsetnskip(name.s, "0123456789", 10, name.len);
          if(name.s[i]) {
            if(scan_longn(&name.s[i], name.len - i, &num) > 0)
              ++num;
            name.len = i;
          }
        }
        stralloc_catlong(&name, num);
        stralloc_nul(&name);
      }
      /*      if(prev && !(parent && prev == 0)) {
       buffer_puts(b, ";");      }*/
      chained_attributes = xml_num_attrs(n) > 0 && (!text_children || n->children == 0);
      if((n->children && n->children->next == 0 && n->children->type == XML_TEXT)) {
        name.s = NULL;
        text_children = 1;
      }
      if(name.s && !text_children) {
        if(!strlist_contains(&vars, name.s)) {
                newline_indent(b, depth );

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
    }
    if(xml_num_attrs(n) > 0) {
      if(chained_attributes) {
        if(name.s) {
          newline_indent(b, depth);
          buffer_putsa(b, &name);
        }
        buffer_putm_internal(b, "->attributes", 0);
        buffer_putm_internal(b, " =", 0);
        newline_indent(b, depth);
        buffer_putm_internal(b, "  xml_attributes", 0);
        i = 0;
        hmap_foreach(n->attributes, t) {
          if(1) {
            newline_indent(b, depth);
          }
          buffer_putm_internal(b, i == 0 ? "  (\"" : "   \"", t->key, "\", \"", t->vals.val_chars, "\",", 0);
          ++i;
        }
        buffer_puts(b, " 0);");
        newline_indent(b, depth);
      } else {
        hmap_foreach(n->attributes, t) {
          newline_indent(b, depth);
          buffer_putm_internal(b, "xml_set_attribute(", name.s, ", \"", t->key, "\", \"", t->vals.val_chars, "\")", 0);
        }
      }
    }
    // if(!text_children && !chained_attributes)
    if(text_children) {
      buffer_putm_internal(b, "->children =", 0);
                  newline_indent(b, depth);

      buffer_putm_internal(b,  " xml_textnode(\"", n->children->name, "\");", 0);
    }

    newline_indent(b, depth + 1);

    if(n->children) {
      if(name.s && (n->type == XML_ELEMENT || n->type == XML_DOCUMENT) && n->children)
        xml_dump(n->children, b, name.s, depth + 1);
    }

    buffer_flush(b);
    prev = n;
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
    xml_dump(doc->children, buffer_1, 0, 0);
    xml_free(doc);
  }
  buffer_close(&infile);
}