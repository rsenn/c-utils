#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <libxml/SAX.h>

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

  sax_hander.initialized = XML_SAX2_MAGIC;
  sax_hander.startElementNs = on_start_element_ns;
  sax_hander.endElementNs = on_end_element_ns;
  sax_hander.characters = on_characters;
  sax_hander.attributeDecl = on_attribute_decl;

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
  printf("%s=\"%s\"\n", fullname, defaultValue);
}

static void
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
  printf("<%s>\n", localname);
}

static void
on_end_element_ns(
  void* ctx,
  const xmlChar* localname,
  const xmlChar* prefix,
  const xmlChar* URI
) {
  printf("</%s>\n", localname);
}

static void
on_characters(void* ctx, const xmlChar* ch, int len) {
  char chars[len + 1];
  int i;
  strncpy(chars, (const char*)ch, len);
  chars[len] = (char)NULL;
  for (i = len - 1; i >= 0; --i) {
    if(!isspace(chars[i]))
      break;
    chars[i] = '\0';
  }
  printf("[%s]\n", chars);
}