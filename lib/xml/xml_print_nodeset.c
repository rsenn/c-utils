#include "../xml.h"

void
xml_print_nodeset(const xmlnodeset* ns, buffer* b) {
  size_t i;
  stralloc nodepath;
  stralloc_init(&nodepath);
  for(i = 0; i < ns->count; ++i) {
    buffer_puts(b, "xmlnodeset[");
    buffer_putlong(b, i);
    buffer_puts(b, "] ");

    stralloc_zero(&nodepath);
    xml_path(ns->nodes[i], &nodepath);

    buffer_putsa(b, &nodepath);
    buffer_puts(b, "[@");

    xml_print_attributes(ns->nodes[i], b, ",", "=", "\"");
    buffer_puts(b, "]");

    buffer_putnlflush(b);
//    xml_print(ns->nodes[i], b);
  }
  //buffer_flush(b);
}
