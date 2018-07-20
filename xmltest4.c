#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/hmap.h"
#include "lib/iterator.h"
#include "lib/stralloc.h"
#include "lib/xml.h"
#include "lib/fmt.h"
#include <assert.h>
#include <ctype.h>
#include <sys/types.h>

static buffer infile;
static buffer b;

void
put_str_escaped(buffer* b, const char* str) {
  stralloc esc;
  stralloc_init(&esc);
  byte_fmt_pred(str, str_len(str), &esc, fmt_escapecharc, iscntrl);
  buffer_putsa(b, &esc);
}

void
xml_dump(xmlnode* n, buffer* b) {
  do {
    stralloc path;
    stralloc_init(&path);

    xml_path(n, &path);
    buffer_putsa(b, &path);

    if(n->type == XML_NODE_TEXT) {
      buffer_puts(b, " \"");
      put_str_escaped(b, n->name);
      buffer_puts(b, "\"");

    } else if(n->type == XML_NODE_ELEMENT) {
      xml_print_attributes(n, b, ", ", ":", "");
    }

    buffer_putnlflush(b);

    if(n->children) xml_dump(n->children, b);

  } while((n = n->next));
}

int
main(int argc, char* argv[1]) {
stralloc tmp;
stralloc_init(&tmp);

  buffer_mmapprivate(&infile, argc > 1 ? argv[1] : "../dirlist/test.xml");

  xmlnode* doc = xml_read_tree(&infile);

//  xml_print(doc);

  //xml_debug(doc, buffer_1);
  //xml_print(doc, buffer_1);

  xmlnode* n = xml_find_element(doc, "signals");

  xml_print(n, buffer_1);

  xmlnode* n2 = xml_find_element_attr(doc, "signal", "name", "N$11");
  xml_print(n2, buffer_1);

  n2 = xml_find_element_attr(doc, "element", "name", "C1");
  xml_print(n2, buffer_1);
  xml_path(n2, &tmp); buffer_putsa(buffer_1, &tmp); buffer_putnlflush(buffer_1);

  n2 = xml_find_element_attr(doc, "element", "name", "R1");
  xml_print(n2, buffer_1);
  xml_path(n2, &tmp); buffer_putsa(buffer_1, &tmp); buffer_putnlflush(buffer_1);

  xml_debug(doc, buffer_2);


  xml_free(doc);

  buffer_close(&b);
}
