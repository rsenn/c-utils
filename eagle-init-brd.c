#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <libxml/SAX.h>

#include "str.h"
#include "stralloc.h"
#include "hmap.h"

static stralloc element_name, character_buf;

static xmlDocPtr my_doc = NULL;

static HMAP_DB* my_hmap_db = NULL;
static TUPLE* ptr_tuple    = NULL;

/* ----------------------------------------------------------------------- */
static void
attr_list(stralloc* sa, HMAP_DB* hmap) {
  TUPLE* p;
  if(hmap == NULL) return;
  for(p = hmap->list_tuple; p; p = p->next) {
    if(p->data_type == HMAP_DATA_TYPE_CHARS) {
      stralloc_catb(sa, " ", 1);
      stralloc_cats(sa, p->key);
      stralloc_catb(sa, "=\"", 2);
      stralloc_cats(sa, p->vals.val_chars);
      stralloc_catb(sa, "\"", 1);
    }
    if(p->next == hmap->list_tuple) break;
  }
}

/* ----------------------------------------------------------------------- */
static void
print_list(HMAP_DB* hmap) {
  TUPLE* p;
  if(hmap == NULL) return;
  for(p = hmap->list_tuple; p; p = p->next) {
    if(p->data_type == HMAP_DATA_TYPE_CHARS) {
      /*//printf("index[%d][%p] key[%s], data[%s]\n", p->index, p,  p->key, p->vals.val_chars);
      */
      printf("key=\"%s\",data=\"%s\"\n", p->key, p->vals.val_chars);
    }
    if(p->next == hmap->list_tuple) break;
  }
}

/**
 * print_element_names:
 * @a_node: the initial xml node to consider.
 *
 * Prints the names of the all the xml elements
 * that are siblings or children of a given xml node.
 */
static void
print_element_names(xmlNode* a_node) {
  xmlNode* cur_node = NULL;

  for(cur_node = a_node; cur_node; cur_node = cur_node->next) {
    if(cur_node->type == XML_ELEMENT_NODE) {
      printf("node type: Element, name: %s\n", cur_node->name);
    }

    print_element_names(cur_node->children);
  }
}

/* ----------------------------------------------------------------------- */
static void
set_element_name(const char* name) {
  stralloc_copys(&element_name, name);
}

/* ----------------------------------------------------------------------- */
static const char*
get_element_name() {
  stralloc_nul(&element_name);
  return element_name.s ? element_name.s : "(null)";
}

/* ----------------------------------------------------------------------- */
static const char*
get_characters() {
  stralloc_nul(&character_buf);
  return character_buf.s ? character_buf.s : "";
}

/* ----------------------------------------------------------------------- */
int read_xmlfile(const char* filename);
int parse_xmlfile(const char* filename, xmlDocPtr* p_doc);
xmlSAXHandler make_sax_handler();

/* ----------------------------------------------------------------------- */
static void on_attribute_decl(void*, const xmlChar*, const xmlChar*, int, int,
                              const xmlChar*, xmlEnumerationPtr);

static void after_element(const char*);
static void on_start_element(void*, const xmlChar*, const xmlChar**);
static void on_end_element(void*, const xmlChar*);

static void on_start_element_ns(void*, const xmlChar*, const xmlChar*,
                                const xmlChar*, int, const xmlChar**, int, int, const xmlChar**);

static void on_end_element_ns(void*, const xmlChar*, const xmlChar*,
                              const xmlChar*);

static void on_characters(void* ctx, const xmlChar* ch, int len);

/* ----------------------------------------------------------------------- */
int
read_xmlfile(const  char* filename) {
  FILE* f;
  int res;
  char chars[1024];

  f = fopen(filename, "r");
  if(!f) {
    puts("file open error.");
    return 1;
  }

  res = fread(chars, 1, 4, f);
  if(res <= 0) {
    fclose(f);
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

  fclose(f);
  return 0;
}

/* ----------------------------------------------------------------------- */
int
parse_xmlfile(const char* filename, xmlDocPtr* p_doc) {
  xmlParserCtxtPtr ctxt; /* the parser context */
  xmlDocPtr doc; /* the resulting document tree */

  /* create a parser context */
  ctxt = xmlNewParserCtxt();
  if(ctxt == NULL) {
    fprintf(stderr, "Failed to allocate parser context\n");
    return 1;
  }
  /* parse the file, activating the DTD validation option */
  doc = xmlCtxtReadFile(ctxt, filename, NULL, XML_PARSE_DTDVALID);
  /* check if parsing suceeded */
  if(doc == NULL) {
    fprintf(stderr, "Failed to parse %s\n", filename);
    return 1;
  } else {
    /* check if validation suceeded */
    if(ctxt->valid == 0)
      fprintf(stderr, "Failed to validate %s\n", filename);

    *p_doc = doc;
    return 1;
  }
  /* free up the parser context */
  xmlFreeParserCtxt(ctxt);
  return 0;
}

/* ----------------------------------------------------------------------- */
xmlSAXHandler
make_sax_handler() {
  xmlSAXHandler sax_hander;

  memset(&sax_hander, 0, sizeof(xmlSAXHandler));

  /*//sax_hander.initialized = 0;
  */
  sax_hander.initialized = XML_SAX2_MAGIC;
  sax_hander.startElement = on_start_element;
  sax_hander.startElementNs = on_start_element_ns;
  sax_hander.endElement = on_end_element;
  sax_hander.endElementNs = on_end_element_ns;
  sax_hander.characters = on_characters;
  /* sax_hander.attributeDecl = on_attribute_decl; */

  return sax_hander;
}

/* ----------------------------------------------------------------------- */
static void
on_attribute_decl(void* ctx, const xmlChar* elem, const xmlChar* fullname,
                  int type, int def, const xmlChar* defaultValue,
                  xmlEnumerationPtr tree) {
  /* printf("<%s> %s=\"%s\"\n", get_element_name(), fullname, defaultValue); */
}

/* ----------------------------------------------------------------------- */
static void
on_start_element(void* ctx, const xmlChar* name, const xmlChar** attrs) {
  int ret, i, nb_attributes = 0;
  set_element_name((const char*)name);

  if(attrs) {
    for(i = 0; attrs[i]; ++i) {}
    nb_attributes = i >> 1;
  }

  printf("<%s> %d\n", name, nb_attributes);

  for(i = 0; i < nb_attributes; ++i) {
    char* attr = ((char**)attrs)[i << 1],
          *value = ((char**)attrs)[(i << 1) + 1];

    printf("<%s> %d/%d: %s=\"%s\"\n", name, i, nb_attributes, attr, value);

    ret = hmap_add(&my_hmap_db, attr, str_len(attr), 1, HMAP_DATA_TYPE_CHARS, value, str_len(value));

  }
}

/* ----------------------------------------------------------------------- */
static void
on_start_element_ns(void* ctx, const xmlChar* localname, const xmlChar* prefix,
                    const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces, int nb_attributes,
                    int nb_defaulted, const xmlChar** attributes) {

  set_element_name((const char*)localname);
  /*  printf("<%s> %d\n", localname, nb_attributes);

    for(i = 0; i < nb_attributes; ++i) {
      printf("<%s> %d/%d %s\n", localname, i, nb_attributes, attributes[i]);
    }
  */
}

/* ----------------------------------------------------------------------- */
static void
on_end_element(void* ctx, const xmlChar* name) {
  after_element((const char*)name);
}

/* ----------------------------------------------------------------------- */
static void
on_end_element_ns(void* ctx, const xmlChar* localname, const xmlChar* prefix,
                  const xmlChar* URI) {
  after_element((const char*)localname);
}

/* ----------------------------------------------------------------------- */
static void
after_element(const char* name) {
  stralloc saa;
  stralloc_init(&saa);

  attr_list(&saa, my_hmap_db);
  /* print_list(my_hmap_db); */
  stralloc_nul(&saa);

  if(saa.len)
    printf("<%s> attrs:%s\n", get_element_name(), saa.s);

  stralloc_free(&saa);

  hmap_destroy(&my_hmap_db);
  hmap_init(1024, &my_hmap_db);

  stralloc_zero(&character_buf);
  //printf("\n");
}

/* ----------------------------------------------------------------------- */
size_t
str_copyn(char* out, const char* in, size_t n) {
  strncpy(out, in, n);
  out[n] = '\0';
  return str_len(out);
}

/* ----------------------------------------------------------------------- */
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

/* ----------------------------------------------------------------------- */
static void
on_characters(void* ctx, const xmlChar* ch, int len) {
  char* chars = malloc(len + 1);
  char* escaped = malloc(len * 2 + 1);
  int i;
  str_copyn(chars, (const char*)ch, len);
  str_escapen(escaped, chars, str_len(chars));
  for(i = len - 1; i >= 0; --i) {
    if(!isspace(escaped[i]))
      break;
    escaped[i] = '\0';
  }

  if(str_len(escaped) > 0) {
    stralloc_cats(&character_buf, escaped);
    /*printf("<%s> [%s]\n", get_element_name(), escaped); */
  }
  free(escaped);
  free(chars);
}

/* ----------------------------------------------------------------------- */
int
main(int argc, char* argv[]) {
  xmlNode *root_element = NULL;
  const char* filename = "sample.xml";

  /* initialize database */
  hmap_init(1024, &my_hmap_db);

  if(argc > 1)
    filename = argv[1];

  /*   if(read_xmlfile(f)) {*/
  if(parse_xmlfile(filename, &my_doc)) {
    puts("xml read error.");
    exit(1);
  }

  /* Get the root element node */
  root_element = xmlDocGetRootElement(my_doc);
  print_element_names(root_element);

  /* free up the resulting document */
  xmlFreeDoc(my_doc);

  return 0;
}
