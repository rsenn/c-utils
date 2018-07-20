#include "lib/buffer.h"
#include "lib/fmt.h"
#include "lib/hmap.h"
#include "lib/iterator.h"
#include "lib/stralloc.h"
#include "lib/xml.h"
#include "lib/byte.h"
#include <assert.h>

void
put_str_escaped(buffer* b, const char* str) {
  stralloc esc;
  stralloc_init(&esc);
  byte_fmt(str, str_len(str), &esc, fmt_escapecharc);
  buffer_putsa(b, &esc);
}

void
xml_print(xmlnode* n) {
  do {
    stralloc path;
    stralloc_init(&path);

    xml_path(n, &path);

    buffer_puts(buffer_1, "path: ");
    buffer_putsa(buffer_1, &path);

    if(n->type == XML_NODE_TEXT) {
      buffer_puts(buffer_1, " \"");
      put_str_escaped(buffer_1, n->name);
      buffer_puts(buffer_1, "\"");
    }

    buffer_putnlflush(buffer_1);

    if(n->children) xml_print(n->children);

  } while((n = n->next));
}

int
main() {
  buffer input;
  buffer_mmapprivate(&input, "../dirlist/test.xml");
  
  xmlnode* doc = xml_read_tree(&input);
  xml_print(doc);
  xml_free(doc);
}
