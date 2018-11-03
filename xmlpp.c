#include "lib/buffer.h"
#include "lib/hmap.h"
#include "lib/io.h"
#include "lib/stralloc.h"
#include "lib/xml.h"
#include <ctype.h>

static buffer infile, b;
static int depth = 0, prev_closing = 0;
static stralloc prev_element;

int
xml_read_function(xmlreader* reader, xmlnodeid id, stralloc* name, stralloc* value, HMAP_DB** attrs) {
  switch(id) {
    case XML_TEXT: {
      buffer_putsa(buffer_1, value);
      break;
    }
    case XML_ELEMENT: {
      int closing = reader->closing || reader->self_closing;

      if(reader->closing) --depth;

      if(!(reader->closing && !prev_closing && stralloc_equal(&prev_element, name)) && stralloc_length(&prev_element)) {
        buffer_puts(buffer_1, "\n");
        buffer_putnspace(buffer_1, depth * 2);
      }

      buffer_putm_3(buffer_1, "<", reader->closing ? "/" : "", name->s);

      if(attrs && *attrs && (*attrs)->list_tuple) {
        buffer_putspace(buffer_1);
        xml_print_attributes(*attrs, buffer_1, " ", "=", "\"");
      }

      buffer_puts(buffer_1, reader->self_closing ? (name->s[0] == '?' ? "?>" : "/>") : ">");

      stralloc_copy(&prev_element, name);
      prev_closing = closing;

      if(!reader->closing && !reader->self_closing) ++depth;
      break;
    }
    default: break;
  }
  return 1;
}

int
main(int argc, char* argv[]) {
  xmlreader r;

  if(argc > 1)
    buffer_mmapprivate(&infile, argv[1]);
  else
    buffer_read_fd(&infile, 0);

  xml_reader_init(&r, &infile);
  xml_read_callback(&r, xml_read_function);
  buffer_putnlflush(buffer_1);
  buffer_close(&b);
}
