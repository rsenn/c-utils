#include "lib/buffer.h"
#include "lib/hmap.h"
#include "lib/iterator.h"
#include "lib/stralloc.h"
#include "lib/xml.h"
#include <assert.h>

// void
// xml_add_attrs(xmlnode** ptr, HMAP_DB* hmap) {
//  for(TUPLE** it = hmap_begin(hmap); hmap_iterator_dereference(it);
//  hmap_iterator_increment(it)) {
//    TUPLE* tuple = hmap_iterator_dereference(it);
//    assert(tuple->key);
//    assert(tuple->data);
//    xmlnode* attr = xml_attrnode(tuple->key, tuple->key_len, tuple->data,
//    tuple->data_len); *ptr = attr; ptr = &attr->next; if(hmap_last(hmap, it))
//      break;
//  }
//}

void
xml_print(xmlnode* n) {
  do {
    stralloc path;
    stralloc_init(&path);

    xml_path(n, &path);

    buffer_puts(buffer_1, "path: ");
    buffer_putsa(buffer_1, &path);
    buffer_putnlflush(buffer_1);

    if(n->children)
      xml_print(n->children);

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
