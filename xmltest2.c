#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/hmap.h"
#include "lib/iterator.h"
#include "lib/stralloc.h"
#include "lib/strlist.h"
#include "lib/xml.h"
#include "lib/str.h"
#include "lib/mmap.h"
#include "lib/scan.h"
#include "lib/array.h"
#include <assert.h>
#include <ctype.h>
#include <sys/types.h>

static buffer infile;
static buffer b;

void
put_str_escaped(buffer* b, const char* str) {
  stralloc esc;
  stralloc_init(&esc);
  stralloc_fmt_pred(&esc, str, str_len(str), (stralloc_fmt_fn*)&fmt_escapecharc, (int (*)()) & iscntrl);
  buffer_putsa(b, &esc);
}

void
xml_dump(xmlnode* n, buffer* b) {
  do {
    stralloc path;
    stralloc_init(&path);
    xml_path(n, &path);
    buffer_putsa(b, &path);

    if(n->type == XML_TEXT) {
      buffer_puts(b, " \"");
      put_str_escaped(b, n->name);
      buffer_puts(b, "\"");
    } else if(n->type == XML_ELEMENT) {
      xml_print_attributes(n->attributes, b, ", ", ":", "");
    }
    buffer_putnlflush(b);
    if(n->children)
      xml_dump(n->children, b);
  } while((n = n->next));
}

int
main(int argc, char* argv[1]) {
  const char* elem_name = "text";
  stralloc tmp;
  stralloc_init(&tmp);
  buffer_mmapprivate(&infile, argc > 1 ? argv[1] : "../dirlist/test.xml");
  
  if(argc > 2)
    elem_name = argv[2];
  {
    xmlnode* doc = xml_read_tree(&infile);

    xmlnodeset ns = xml_find_all_1(doc, xml_match_name, elem_name);
    xmlnodeset_iter_t it, e;
    size_t i = 0;
    xml_print_nodeset(&ns, buffer_1);

    for(it = xmlnodeset_begin(&ns), e = xmlnodeset_end(&ns); it != e; ++it) {
      xmlnode* n = *it;
      buffer_puts(buffer_1, "NODESET[");
      buffer_putlong(buffer_1, i++);
      buffer_puts(buffer_1, "]: ");
      xml_debug(n, buffer_1);
      buffer_putnlflush(buffer_1);
    }
    xml_free(doc);
    buffer_close(&b);
  }
}
