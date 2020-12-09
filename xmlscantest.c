#include "lib/xml.h"
#include "lib/buffer.h"
#include "lib/mmap.h"
#include "lib/str.h"
#include "lib/stralloc.h"
#include "lib/strlist.h"
#include "lib/fmt.h"
#include <ctype.h>

static buffer infile;
static buffer b;

void
put_str_escaped(buffer* b, const char* str) {
  stralloc esc;
  stralloc_init(&esc);
  stralloc_fmt_pred(
      &esc, str, str_len(str), (stralloc_fmt_fn*)&fmt_escapecharc, (int (*)()) & iscntrl);
  buffer_putsa(b, &esc);
}

const char* token_types[] = {
    "(null)",
    "XML_TAG_NAME",
    "XML_TAG_CLOSE",
    "XML_ATTR_NAME",
    "XML_ATTR_VALUE",
    "XML_DATA",

};

static strlist vars;
void
newline_indent(buffer* b, int depth) {
  buffer_putc(b, '\n');
  buffer_putnspace(b, depth * 2);
}

int
main(int argc, char* argv[]) {
  xmlscanner s;
  char* x;
  size_t len;

  buffer_mmapprivate(&infile, argc > 1 ? argv[1] : "3rdparty/zlib/treebuild.xml");

  xml_scanner(&s, &infile);

  while((x = xml_read_token(&s, &len))) {
  }

  buffer_close(&infile);
}
