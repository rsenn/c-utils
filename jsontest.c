#include "lib/charbuf.h"
#include "lib/mmap.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/hmap.h"
#include "lib/iterator.h"
#include "lib/stralloc.h"
#include "lib/json.h"
#include "lib/open.h"
#include "lib/io_internal.h"
#include <assert.h>
#include <ctype.h>
#include <sys/types.h>

static charbuf infile;
//static buffer b;

void
put_str_escaped(buffer* b, const char* str) {
  stralloc esc;
  stralloc_init(&esc);
  stralloc_fmt_pred(&esc, str, str_len(str), fmt_escapecharc, iscntrl);
  buffer_putsa(b, &esc);
}

int
main(int argc, char* argv[1]) {
  stralloc tmp;
  stralloc_init(&tmp);

  int fd = open_read("../dirlist/test.json");
  //size_t sz;
  //char* map = mmap_private( argc > 1 ? argv[1] : "../dirlist/test.json", &sz);;

  //buffer_mmapprivate(&infile, argc > 1 ? argv[1] : "../dirlist/test.json");
  //
  charbuf_init(&infile, &read, fd);

  jsonval* doc = json_read_tree(&infile);

  json_print(doc, buffer_1);

  charbuf_close(&infile);

  json_free(doc);

}
