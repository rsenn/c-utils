#include <libxml/xmlreader.h>
#include "playlist.h"
#include "buffer.h"

int
buffer_close_return_int(buffer *b) {
  buffer_close(b);
  return 0;
}

void
playlist_xspf(playlist* pls, buffer *b) {
  xmlTextReaderPtr rd;
  rd = xmlReaderForIO((xmlInputReadCallback)buffer_get, (xmlInputCloseCallback)buffer_close_return_int, b, "", "UTF-8", 0);
  pls->ptr = rd;
  pls->type = XSPF;
}
