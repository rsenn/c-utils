#include "lib/buffer.h"
#include "lib/mmap.h"
#include "lib/stralloc.h"
#include "lib/strarray.h"
#include "lib/str.h"
#include "lib/byte.h"
#include "lib/array.h"

int
main(int argc, char* argv[]) {
  const char* const headers[] = {"lib/algorithm.h",
                                 "lib/array.h",
                                 "lib/binfmt.h",
                                 "lib/buffer.h",
                                 "lib/byte.h",
                                 "lib/case.h",
                                 "lib/cb.h",
                                 "lib/cbmap/alloc.h",
                                 "lib/cbmap.h",
                                 "lib/cbmap_internal.h",
                                 "lib/cb_internal.h",
                                 "lib/charbuf.h",
                                 "lib/dir.h",
                                 "lib/directory_iterator.h",
                                 "lib/dir_internal.h",
                                 "lib/dns.h",
                                 "lib/elf.h",
                                 "lib/env/alloc.h",
                                 "lib/env.h",
                                 "lib/errmsg.h",
                                 "lib/errmsg_internal.h",
                                 "lib/expand.h",
                                 "lib/fmt.h",
                                 "lib/fnmatch.h",
                                 "lib/getopt.h",
                                 "lib/glob.h",
                                 "lib/gpio.h",
                                 "lib/gpio_internal.h",
                                 "lib/hashmap.h",
                                 "lib/hmap.h",
                                 "lib/hmap_internal.h",
                                 "lib/http.h",
                                 "lib/iarray.h",
                                 "lib/io.h",
                                 "lib/iob.h",
                                 "lib/ioctlcmd.h",
                                 "lib/iopause.h",
                                 "lib/io_internal.h",
                                 "lib/ip4.h",
                                 "lib/ip6.h",
                                 "lib/iterator.h",
                                 "lib/json.h",
                                 "lib/likely.h",
                                 "lib/linked_list.h",
                                 "lib/list.h",
                                 "lib/macho.h",
                                 "lib/map.h",
                                 "lib/bmap_internal.h",
                                 "lib/memalign.h",
                                 "lib/memory.h",
                                 "lib/mmap.h",
                                 "lib/ndelay.h",
                                 "lib/open.h",
                                 "lib/path.h",
                                 "lib/path.h",
                                 "lib/pe.h",
                                 "lib/playlist.h",
                                 "lib/range.h",
                                 "lib/rdir.h",
                                 "lib/safemult.h",
                                 "lib/scan.h",
                                 "lib/select.h",
                                 "lib/sig.h",
                                 "lib/slist.h",
                                 "lib/socket.h",
                                 "lib/socket_internal.h",
                                 "lib/str.h",
                                 "lib/stralloc.h",
                                 "lib/strarray.h",
                                 "lib/strlist.h",
                                 "lib/tai.h",
                                 "lib/taia.h",
                                 "lib/textbuf.h",
                                 "lib/tree.h",
                                 "lib/uint16.h",
                                 "lib/uint32.h",
                                 "lib/uint64.h",
                                 "lib/uint8.h",
                                 "lib/unit_test.h",
                                 "lib/utf8.h",
                                 "lib/var.h",
                                 "lib/vartab.h",
                                 "lib/windoze.h",
                                 "lib/wordexp.h",
                                 "lib/xml.h"};
  strarray sarr;
  stralloc sa;
  strarray_init(&sarr);
  stralloc_init(&sa);
  strarray_from_argv(sizeof(headers) / sizeof(headers[0]), headers, &sarr);

  strarray_removesuffixs(&sarr, ".h");
  strarray_joins(&sarr, &sa, ", ");
  buffer_putsa(buffer_1, &sa);
  buffer_putnlflush(buffer_1);

  return 0;
}
