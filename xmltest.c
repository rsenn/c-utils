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
#include "map.h"
#include "lib/scan.h"

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
  size_t num_attrs;
  const char* attrs_str;
  int i, text_children, do_assign, chained_attributes;
  xmlnode* prev = 0;
  stralloc name;
  stralloc_init(&name);
  do {
    do_assign = 0;
    chained_attributes = 0;
    if(n->type == XML_DOCUMENT) {
      n = n->children;
      continue;
    }

    stralloc_zero(&name);

    if(prev == 0)
      newline_indent(b, depth);

    if(n->type == XML_TEXT) {
      const char* x = xml_get_text(n, &name);
      if(x[0]) {
        buffer_putm_internal(b, "", parent, "->children = xml_textnode(\"", x, "\");", NULL);
        newline_indent(b, depth);
      }
    } else if(n->type == XML_ELEMENT) {
      size_t p = 0;
      if(n->name[0] == '/')
        continue;
      stralloc_decamelize(n->name, &name, '_');
      stralloc_remove_all(&name, "-.", 2);
      stralloc_lower(&name);
      stralloc_nul(&name);
      if(parent)
        p = scan_noncharsetnskip(parent, "0123456789", str_len(parent));
      if((parent && !stralloc_diffb(&name, parent, p)) || strlist_contains(&vars, name.s)) {
        long num = 1;
        if(name.len > 0) {
          size_t i = scan_noncharsetnskip(name.s, "0123456789", name.len);
          if(isdigit(parent[p])) {
            scan_longn(&parent[p], str_len(parent) - p, &num);
            ++num;
            name.len = p;
          }
        }

        do {

          stralloc_catlong(&name, num);
          stralloc_nul(&name);

          ++num;
          name.len = p;

        } while(strlist_contains(&vars, name.s));
      }
      num_attrs = xml_num_attrs(n);
      text_children = (n->children && n->children->next == 0 && n->children->type == XML_TEXT);

      chained_attributes = num_attrs > 0 && (!text_children || n->children == 0);
      if(n->children) {
        if(/*!text_children &&
            */
           ((n->children->type == XML_ELEMENT) || (!chained_attributes && !text_children && name.s))) {
          if(!strlist_contains(&vars, name.s)) {
            buffer_puts(b, "xmlnode*");
            newline_indent(b, depth);
            strlist_push(&vars, name.s);
          }
          buffer_putm_internal(b, name.s, " = ", NULL);
          do_assign = 1;
        }
      }

      attrs_str = num_attrs > 0 && !n->children ? "_attrs" : "";

      if(text_children && parent)
        attrs_str = "_text";

      if(attrs_str[1] == 'a')
        chained_attributes = 0;

      if(parent)
        buffer_putm_internal(b, "xml_child_element", attrs_str, "(\"", n->name, "\", ", parent, NULL);
      else
        buffer_putm_internal(b, "xml_element", attrs_str, "(\"", n->name, NULL);
      buffer_flush(b);

      if(attrs_str[1] == 't') {
        stralloc text;
        stralloc_init(&text);
        buffer_putm_internal(b, ", \"", xml_get_text(n, &text), "\"", NULL);
        stralloc_free(&text);
        text_children = 0;
      } else if(!chained_attributes && n->attributes) {
        i = 0;
        hmap_foreach(n->attributes, t) {
          buffer_putm_internal(b, ", \"", NULL);
          buffer_put(b, t->key, t->key_len);
          buffer_putm_internal(b, "\", \"", t->vals.val_chars, "\"", NULL);
        }
        if(attrs_str[1] == 't' || attrs_str[1] == 'a')
          buffer_puts(b, ", 0");
      }

      buffer_puts(b, ")");

      if(!text_children && !chained_attributes)
        buffer_puts(b, ";");

      buffer_flush(b);

      if(num_attrs > 0 && !attrs_str[0]) {
        if(chained_attributes) {
          if(name.s && do_assign) {
            buffer_puts(b, ";");
            newline_indent(b, depth);
            buffer_putsa(b, &name);
          } else {
          }
          buffer_puts(b, "->attributes = xml_attributes(");
          i = 0;
          hmap_foreach(n->attributes, t) {
            if(1)
              newline_indent(b, depth + 1);

            buffer_putm_internal(b, i == 0 ? "\"" : "\"", t->key, "\", \"", t->vals.val_chars, "\",", NULL);
            ++i;
          }
          newline_indent(b, depth);
          buffer_puts(b, "0);");
        } else {
          hmap_foreach(n->attributes, t) {
            newline_indent(b, depth);
            buffer_putm_internal(b, "xml_set_attribute(", name.s, ", \"", t->key, "\", \"", t->vals.val_chars, "\")", NULL);
          }
        }
      }

      if(attrs_str[1] != 't') {
        if(text_children) {
          buffer_putm_internal(b, "->children =", NULL);
          // newline_indent(b, depth + 2)
          buffer_putm_internal(b, " xml_textnode(\"", n->children->name, "\");", NULL);
        }

        if(!text_children) {
          if(n->children) {
            newline_indent(b, depth + 1);

            if(name.s && (n->type == XML_ELEMENT || n->type == XML_DOCUMENT) && n->children)
              xml_dump(n->children, b, name.s, depth + 1);
          }
        }
      }
    }

    newline_indent(b, depth);

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
    xml_dump(doc->children, buffer_1, 0, 1);
    xml_print(doc->children, buffer_2, 0);
    xml_free(doc);
  }
  buffer_close(&infile);
}
