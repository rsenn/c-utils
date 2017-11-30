#include <libxml/xmlreader.h>
#include "playlist.h"
#include "buffer.h"

int
buffer_close_return_int(buffer *b) {
  buffer_close(b);
  return 0;
}

static void
xspf_process(xmlTextReaderPtr reader) {
  const xmlChar *name, *value;

  name = xmlTextReaderConstName(reader);
  if(name == NULL)
    name = BAD_CAST "--";

  value = xmlTextReaderConstValue(reader);

//  if(xmlTextReaderIsEmptyElement(reader)) return;
  
  int nodeType =   xmlTextReaderNodeType(reader);

  static stralloc nodeText;

 if(nodeType == 15){

  }


  if(value || xmlTextReaderReadString(reader))  {
    xmlChar* s = value ? value : xmlTextReaderReadString(reader);

    while(*s && (isspace(*s) || *s == '\n' || *s == '\r' || *s == '\t' || *s == ' ')) {
    s++;
    }

    stralloc_cats(&nodeText, s);
    stralloc_0(&nodeText);
    
    
    if(nodeText.len && nodeText.s[0])
    printf("%*s\"%s\"\n",          xmlTextReaderDepth(reader),"    ", nodeText.s);
  }
  
  if(name[0] == '#')  {
  
  }
  else
  printf("%*s<%s%s>%s\n",
         xmlTextReaderDepth(reader),"    ",
         nodeType == 15 ? "/" : "",
         name,
         nodeText.s );


 if(name[0] != '#'  && nodeText.len) {

  
    stralloc_zero(&nodeText);
  }
/*
  if(value == NULL)
    printf("\n");
  else {
    if(xmlStrlen(value) > 40)
      printf(" %.40s...\n", value);
    else
      printf(" %s\n", value);
  }*/
}

static int
xspf_reader(xmlTextReaderPtr reader) {
  int ret = xmlTextReaderRead(reader);
  while(ret == 1) {
    xspf_process(reader);
    ret = xmlTextReaderRead(reader);
  }
  xmlFreeTextReader(reader);
  if(ret != 0) {
    fprintf(stderr, "failed to parse\n");
  }

}

void
playlist_xspf(playlist* pls, buffer *b) {
  xmlTextReaderPtr rd;
  rd = xmlReaderForIO((xmlInputReadCallback)buffer_get, (xmlInputCloseCallback)buffer_close_return_int, b, "", "UTF-8", 0);
  pls->ptr = rd;
  pls->type = XSPF;

  xspf_reader(pls->ptr);
}
