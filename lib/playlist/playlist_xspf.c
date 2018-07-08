#include <libxml/xmlreader.h>
#include <ctype.h>
#include "../playlist.h"
#include "../buffer.h"

int
buffer_close_return_int(buffer *b) {
  buffer_close(b);
  return 0;
}

static void
xspf_process(playlist* pl) {
  xmlTextReaderPtr reader = pl->ptr;
  const char *name, *value;
  static stralloc nodeText;
  static struct {
    stralloc title, location;
    uint32 length;
  } track;
  int nodeType = xmlTextReaderNodeType(reader);
  int nodeDepth = xmlTextReaderDepth(reader);
  name = (const char*) xmlTextReaderConstName(reader);

  if(nodeDepth <= 0) return;

  if(name == NULL)
    name = (const char*)(BAD_CAST "--");

  value = (const char*) xmlTextReaderConstValue(reader);

  if(nodeType == 15 && nodeDepth > 1) {

    if(strcmp(name, "location") == 0) {
      stralloc_copy(&track.location, &nodeText);
      //stralloc_0(&track.location);
    } else if(strcmp(name, "title") == 0) {
      stralloc_copy(&track.title, &nodeText);
      //stralloc_0(&track.title);
    } else if(strcmp(name, "duration") == 0) {
      track.length = atoi(nodeText.s);
    } else if(strcmp(name, "track") == 0) {
      //    printf("XTRACKX: title:\"%s\" location:\"%s\" length:%u\n", track.title.s, track.location.s, track.length);

      if(pl->callback) {
        pl->callback(pl, &track.title, &track.location, track.length);
      }
    }

  }

// if(xmlTextReaderIsEmptyElement(reader)) return;

  if(value || xmlTextReaderReadString(reader)) {
    const char* s = value ? (const char*)value : (const char*)xmlTextReaderReadString(reader);

    while(*s && (isspace(*s) || *s == '\n' || *s == '\r' || *s == '\t' || *s == ' ')) {
      s++;
    }

    stralloc_cats(&nodeText, s);
    //stralloc_0(&nodeText);

    if(nodeText.len && nodeText.s[0]) {
      //  printf("@%d: \"%s\"\n", xmlTextReaderDepth(reader), nodeText.s);
    }
  }

  /*  if(name[0] != '#') {
  //   printf("%d: <%s%s(%d))>%s\n", xmlTextReaderDepth(reader), nodeType == 15 ? "/" : "", name, nodeType, nodeText.s);
    }*/

  if(name[0] != '#') {
    stralloc_zero(&nodeText);
  }
}

static int
xspf_reader(playlist*pl) {
  xmlTextReaderPtr reader = pl->ptr;
  int ret = xmlTextReaderRead(reader);

  if(ret)
    xspf_process(pl);
  else
    xmlFreeTextReader(reader);


  return ret;
}

void
playlist_xspf(playlist* pls, buffer *b) {
  xmlTextReaderPtr rd;
  rd = xmlReaderForIO((xmlInputReadCallback)buffer_get, (xmlInputCloseCallback)buffer_close_return_int, b, "", "UTF-8", 0);
  pls->ptr = rd;
  pls->type = XSPF;
  pls->reader = (int(*)(playlist*))xspf_reader;

// xspf_reader(pls->ptr);
}
