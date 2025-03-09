#include "lib/xml.h"
#include "lib/buffer.h"
#include "lib/mmap.h"
#include "lib/str.h"
#include "lib/stralloc.h"
#include "lib/strlist.h"
#include "lib/fmt.h"
#include "lib/errmsg.h"
#include <ctype.h>

static buffer infile;
static buffer b;

void
put_escaped(buffer* b, const char* x, size_t len) {
  size_t i;
  char buf[32];

  for(i = 0; i < len; i++) {
    char c = x[i];

    if(c >= 0x20) {
      buffer_putc(b, c);
    } else {
      /*buffer_putc(b, '\\'); */
      buffer_put(b, buf, fmt_escapecharc(buf, (uint64)(unsigned char)c));
    }
  }
}

const char* token_types[] = {"XML_EOF", "XML_DATA", "XML_TAG_NAME", "XML_TAG_CLOSE", "XML_ATTR_NAME", "XML_ATTR_VALUE", "XML_COMMENT"};
const char* token_colors[] = {"\x1b[1;37m", "\x1b[1;31m", "\x1b[1;35m", "\x1b[1;33m", "\x1b[1;36m", "\x1b[1;32m"};

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
  xmltoken tok;
  const char* path = argc > 1 ? argv[1] : "3rdparty/zlib/                                reebuild.xml";
  errmsg_iam(argv[0]);

  if(buffer_readfile(&infile, path)) {
    errmsg_warnsys("Error opening file ", path, ":", 0);
    return 127;
  }

  xml_scanner_buf(&s, &infile);

  do {
    tok = xml_read_token(&s);

    buffer_puts(buffer_2, "Token: ");
    buffer_puts(buffer_2, token_colors[tok.id]);
    buffer_putspad(buffer_2, token_types[tok.id + 1], 16);
    put_escaped(buffer_2, tok.x, tok.len);
    buffer_puts(buffer_2, "\x1b[0m");
    /*  buffer_puts(buffer_2, "\nXML
       token length = ");
          buffer_putulong(buffer_2,
       tok.len);*/
    buffer_putnlflush(buffer_2);
  } while(tok.id != XML_EOF);

  buffer_close(&infile);
}
