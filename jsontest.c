#include "lib/buffer.h"
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
#include "lib/taia.h"

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

#include <assert.h>
#include <ctype.h>
#include <sys/types.h>

static charbuf infile;
// static buffer b;

static void
default_printer(jsonfmt* p, jsonval* v, int depth, int index, char q) {
  p->indent = "  ";

  p->spacing = " ";
  p->newline = "\n";
  p->separat = ",\n";
  p->quote[0] = '"';
  p->quote[1] = '\0';
  p->precision = 10;
  p->depth = depth - (index == -2);
  p->index = index;
  p->compliant = 1;
};

void
put_str_escaped(buffer* b, const char* str) {
  stralloc esc;
  stralloc_init(&esc);
  stralloc_fmt_pred(&esc, str, str_len(str), (stralloc_fmt_fn*)&fmt_escapecharc, iscntrl);
  buffer_putsa(b, &esc);
}

int
main(int argc, char* argv[]) {
  int in_fd, out_fd = STDOUT_FILENO;
  buffer out_buf;
  jsonval* doc;
  tai6464 s, t, u;
  stralloc tmp;
  stralloc_init(&tmp);

  in_fd = open_read(argc > 1 ? argv[1] : "../dirlist/test.json");
  if(argc > 2)
    out_fd = open_trunc(argv[2]);

  charbuf_init(&infile, (read_fn*)(void*)&read, in_fd);

  taia_now(&s);

  doc = json_read_tree(&infile);
  taia_now(&t);

  taia_sub(&u, &t, &s);

  buffer_puts(buffer_2, "Parsing took ");
  buffer_putdouble(buffer_2, taia_approx(&u), 6);
  buffer_putsflush(buffer_2, "s\n");

  buffer_write_fd(&out_buf, out_fd);

  taia_now(&s);

  json_print(*doc, &out_buf, &default_printer);
  taia_now(&t);

  taia_sub(&u, &t, &s);

  buffer_puts(buffer_2, "Printing took ");
  buffer_putdouble(buffer_2, taia_approx(&u), 6);
  buffer_putsflush(buffer_2, "s\n");

  charbuf_close(&infile);

  json_free(doc);
}
