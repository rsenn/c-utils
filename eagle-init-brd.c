#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <libxml/SAX.h>

#include "str.h"
#include "stralloc.h"

static stralloc element_name, character_buf;

static void
set_element_name(const char* name) {
  stralloc_copys(&element_name, name);
}

static const char*
get_element_name() {
  stralloc_nul(&element_name);
  return element_name.s ? element_name.s : "(null)";
}

static const char*
get_characters() {
  stralloc_nul(&character_buf);
  return character_buf.s ? character_buf.s : "";
}

int read_xmlfile(FILE* f);
xmlSAXHandler make_sax_handler();

static void on_attribute_decl(
  void* ctx,
  const xmlChar* elem,
  const xmlChar* fullname,
  int type,
  int def,
  const xmlChar* defaultValue,
  xmlEnumerationPtr tree
);

static void on_start_element(void *ctx, const xmlChar* name, const xmlChar **attrs);
static void on_end_element(void *ctx, const xmlChar* name);

static void on_start_element_ns(
  void* ctx,
  const xmlChar* localname,
  const xmlChar* prefix,
  const xmlChar* URI,
  int nb_namespaces,
  const xmlChar** namespaces,
  int nb_attributes,
  int nb_defaulted,
  const xmlChar** attributes
);

static void on_end_element_ns(
  void* ctx,
  const xmlChar* localname,
  const xmlChar* prefix,
  const xmlChar* URI
);

static void on_characters(void* ctx, const xmlChar* ch, int len);

int
main(int argc, char* argv[]) {
  const char* filename = "sample.xml";
  
  if(argc > 1)
    filename = argv[1];

  FILE* f = fopen(filename, "r");
  if(!f) {
    puts("file open error.");
    exit(1);
  }

  if(read_xmlfile(f)) {
    puts("xml read error.");
    exit(1);
  }

  fclose(f);
  return 0;
}

int
read_xmlfile(FILE* f) {
  char chars[1024];
  int res = fread(chars, 1, 4, f);
  if(res <= 0) {
    return 1;
  }

  xmlSAXHandler sax_hander = make_sax_handler();

  xmlParserCtxtPtr ctxt = xmlCreatePushParserCtxt(
                            &sax_hander, NULL, chars, res, NULL
                          );

  while((res = fread(chars, 1, sizeof(chars), f)) > 0) {
    if(xmlParseChunk(ctxt, chars, res, 0)) {
      xmlParserError(ctxt, "xmlParseChunk");
      return 1;
    }
  }
  xmlParseChunk(ctxt, chars, 0, 1);

  xmlFreeParserCtxt(ctxt);
  xmlCleanupParser();
  return 0;
}

xmlSAXHandler
make_sax_handler() {
  xmlSAXHandler sax_hander;

  memset(&sax_hander, 0, sizeof(xmlSAXHandler));

  sax_hander.initialized = 0; //XML_SAX2_MAGIC;
  sax_hander.startElement = on_start_element;
  sax_hander.startElementNs = on_start_element_ns;
  sax_hander.endElement = on_end_element;
  sax_hander.endElementNs = on_end_element_ns;
  sax_hander.characters = on_characters;
//  sax_hander.attributeDecl = on_attribute_decl;

  return sax_hander;
}

static void
on_attribute_decl(
  void* ctx,
  const xmlChar* elem,
  const xmlChar* fullname,
  int type,
  int def,
  const xmlChar* defaultValue,
  xmlEnumerationPtr tree
) {
//  printf("<%s> %s=\"%s\"\n", get_element_name(), fullname, defaultValue);
}

static void
on_start_element(void *ctx, const xmlChar* name, const xmlChar **attrs) {
  int i, nb_attributes;
  set_element_name((const char*)name);
  
  for(i = 0; attrs[i]; ++i) ;
  nb_attributes = i;
  
    printf("<%s> %d\n", name, nb_attributes);
  
  for(i = 0; attrs[i]; ++i) {
    printf("<%s> %d: %s\n", name, i, attrs[i]);
  }
}

static void
on_end_element(void *ctx, const xmlChar* name) {
}

on_start_element_ns(
  void* ctx,
  const xmlChar* localname,
  const xmlChar* prefix,
  const xmlChar* URI,
  int nb_namespaces,
  const xmlChar** namespaces,
  int nb_attributes,
  int nb_defaulted,
  const xmlChar** attributes
) {

  
  set_element_name((const char*)localname);
/*  printf("<%s> %d\n", localname, nb_attributes);
  
  for(i = 0; i < nb_attributes; ++i) {
    printf("<%s> %d/%d %s\n", localname, i, nb_attributes, attributes[i]);
  }
*/
}

static void
on_end_element_ns(
  void* ctx,
  const xmlChar* localname,
  const xmlChar* prefix,
  const xmlChar* URI
) {
  //printf("</%s>\n", localname);
  
 // printf("<%s>%s</%s>\n", get_element_name(), get_characters(), get_element_name());
  stralloc_zero(&character_buf);
}

size_t
str_copyn(char* out, const char* in, size_t n) {
  strncpy(out, in, n);
  out[n] = '\0';
  return str_len(out);
}

size_t
str_escapen(char* out, const char* in, size_t n) {
  size_t i;
  for(i = 0; i < n; ++i, ++out) {
    if(in[i] == '\n') {
      *out = '\\';
      *++out = 'n';
    } else {
      *out = in[i];
    }
  }
  *out = '\0';
  return i;
}

static void
on_characters(void* ctx, const xmlChar* ch, int len) {
  char chars[len+ 1];
  char escaped[len*2 + 1];
  int i;
  str_copyn(chars, (const char*)ch, len);
  str_escapen(escaped, chars, str_len(chars));
  for (i = len - 1; i >= 0; --i) {
    if(!isspace(escaped[i]))
      break;
    escaped[i] = '\0';
  }

  if(str_len(escaped) > 0) {
  stralloc_cats(&character_buf, escaped);
//	printf("<%s> [%s]\n", get_element_name(), escaped);
  }
}