#include "lib/windoze.h"
#include "lib/charbuf.h"
#include "lib/mmap.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/hmap.h"
#include "lib/iterator.h"
#include "lib/stralloc.h"
#include "lib/json.h"
#include "lib/open.h"
#include "lib/io.h"
#include "lib/iarray.h"
#include "lib/str.h"
#include "lib/scan.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

#include <assert.h>
#include <ctype.h>
#include <sys/types.h>

static charbuf infile;
//static buffer b;

void
put_str_escaped(buffer* b, const char* str) {
  stralloc esc;
  stralloc_init(&esc);
  stralloc_fmt_pred(&esc, str, str_len(str), (stralloc_fmt_fn*)&fmt_escapecharc, iscntrl);
  buffer_putsa(b, &esc);
}

static const jsonprinter compact_printer = {
  "", "", "", ","
};

void
json_pretty_print(jsonval* val, buffer* b) {
  stralloc out;
  stralloc_init(&out);
  json_tosa(val, &out, &compact_printer);

  if(out.len > 16384) {
    json_print(val, b, 0);
  } else {
    buffer_putsa(b, &out);
  }
}

int
main(int argc, char* argv[]) {
  int fd;
  jsonval* doc;
  stralloc tmp;
  stralloc_init(&tmp);

  fd = argc > 1 ? open_read(argv[1]) : 0;

  charbuf_init(&infile, (read_fn*)&read, fd);

  doc = json_read_tree(&infile);

  json_pretty_print(doc, buffer_1);

  charbuf_close(&infile);

  json_free(doc);
}
