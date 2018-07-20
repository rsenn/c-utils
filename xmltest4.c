#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/hmap.h"
#include "lib/iterator.h"
#include "lib/stralloc.h"
#include "lib/xml.h"
#include <assert.h>
#include <sys/types.h>

static buffer infile;
static buffer b;

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
    buffer_putsa(buffer_1, &path);

    if(n->type == XML_NODE_TEXT) {
      buffer_puts(buffer_1, " \"");
      put_str_escaped(buffer_1, n->name);
      buffer_puts(buffer_1, "\"");

    } else if(n->type == XML_NODE_ELEMENT) {
      xml_dump_attributes(n, buffer_1, ", ", ":", "");
    }

    buffer_putnlflush(buffer_1);

    if(n->children) xml_print(n->children);

  } while((n = n->next));
}

int
main() {
  buffer_mmapprivate(&infile, "../dirlist/test.xml");

  textbuf_init(&b, &infile, 1024);

  assert(is_textbuf(&b));

  xmlnode* doc = xml_read_tree(&b);

  xml_print(doc);
  xml_free(doc);

  buffer_close(&b);
}
