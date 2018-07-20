#include "lib/buffer.h"
#include "lib/stralloc.h"
#include "lib/xml.h"
#include "lib/hmap.h"
#include "lib/iterator.h"
#include <assert.h>


//void
//xml_add_attrs(xmlnode** ptr, HMAP_DB* hmap) {
//  for(TUPLE** it = hmap_begin(hmap); hmap_iterator_dereference(it); hmap_iterator_increment(it)) {
//    TUPLE* tuple = hmap_iterator_dereference(it);
//    assert(tuple->key);
//    assert(tuple->data);
//    xmlnode* attr = xml_attrnode(tuple->key, tuple->key_len, tuple->data, tuple->data_len);
//    *ptr = attr;
//    ptr = &attr->next;
//    if(hmap_last(hmap, it))
//      break;
//  }
//}

int
main() {
  buffer input;
  buffer_mmapprivate(&input, "../dirlist/test.xml");
  xmlnode* doc = xml_read_tree(&input);
  xml_dump(doc, buffer_1);
}
