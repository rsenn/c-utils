#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/hmap.h"
#include "lib/iterator.h"
#include "lib/stralloc.h"
#include "lib/xml.h"
#include <assert.h>
#include <sys/types.h>

typedef struct {
  unsigned line;
  unsigned col;
  buffer* b;
} line_buffer;

static ssize_t
linebuf_read(line_buffer* lb, char *x, size_t n) {
  ssize_t r = buffer_get(lb->b, x, n);
  for(ssize_t i = 0; i < r; ++i) {
    if(x[i] == '\n') {
      ++lb->line;
      lb->col = 0;
    } else {
      ++lb->col;
    }
  }
  return r;
}


static line_buffer linebuf_inst;


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

    } else if(n->type == XML_NODE_ELEMENT) {
      xml_dump_attributes(n, buffer_1, ", ", ":", "");
    }

    buffer_putnlflush(buffer_1);

    if(n->children) xml_print(n->children);

  } while((n = n->next));
}

int
main() {
  buffer infile;
  buffer_mmapprivate(&infile, "../dirlist/test.xml");

  linebuf_inst.b = &infile;

static buffer input_buffer;

buffer_init(&input_buffer, &linebuf_read, &linebuf_inst, NULL, 0);

  xmlnode* doc = xml_read_tree(&input_buffer);
  xml_print(doc);
  xml_free(doc);
}
