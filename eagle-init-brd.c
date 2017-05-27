#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <libxml/SAX.h>

#include "str.h"
#include "stralloc.h"
#include "hmap.h"

static stralloc element_name, character_buf;

static xmlDocPtr xmldoc = NULL;

static HMAP_DB* hashmap = NULL;
static TUPLE* ptr_tuple    = NULL;

static HMAP_DB* instances_db = NULL;


#define NAMELEN 8

typedef struct part {
  const char name[NAMELEN];
  const char library[NAMELEN];
  const char deviceset[NAMELEN];
  const char device[NAMELEN];
  const char value[NAMELEN];
} part_t;

typedef struct instance {
  const char part[NAMELEN];
  const char gate[NAMELEN];
  float x, y;
  float rot;  
} instance_t;

/* ----------------------------------------------------------------------- */
static size_t
str_copyn(char* out, const char* in, size_t n) {
  strncpy(out, in, n);
  out[n] = '\0';
  return str_len(out);
}

/* ----------------------------------------------------------------------- */
static void
dump_instance(instance_t const* i) {

  printf("instance{part=%s,gate=%s,x=%f,y=%f,rot=%f}\n",
    i->part, i->gate, i->x, i->y, i->rot);
}

/* ----------------------------------------------------------------------- */
static instance_t*
create_instance(const char* part, const char* gate, float x, float y, float rot) {
  int ret;
  stralloc key;
  instance_t* i;
  stralloc_init(&key);
  stralloc_copys(&key, part);
  stralloc_cats(&key, ":");
  stralloc_cats(&key, gate);
  i = malloc(sizeof(instance_t));
  if(i == NULL) return NULL;
  //memset(i, 0, sizeof(instance_t));
  str_copyn(i->part, part, sizeof(i->part)-1);
  str_copyn(i->gate, gate, sizeof(i->gate)-1);
  i->x = x;
  i->y = y;
  i->rot = rot; 
  hmap_add(&instances_db, key.s, key.len, 1, HMAP_DATA_TYPE_CUSTOM, i);
  printf("create_instance ");dump_instance(i);
  return i;
}

/* ----------------------------------------------------------------------- */
static void
attr_list(stralloc* sa, HMAP_DB* hmap) {
  TUPLE* p;
  if(hmap == NULL) return;
  for(p = hmap->list_tuple; p; p = p->next) {
    if(p->data_type == HMAP_DATA_TYPE_CHARS) {
      stralloc_catb(sa, " ", 1);
      stralloc_cats(sa, p->key);
      stralloc_cats(sa, "=\"");
      stralloc_cats(sa, p->vals.val_chars);
      stralloc_cats(sa, "\"");
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

/* ----------------------------------------------------------------------- */
static void
print_attributes(xmlElement* e) {
  xmlAttribute* a;
  for(a = e->attributes; a; a = a->next) {
  printf("%s=\"%s\"\n", a->name, a->def);
  }
}

/* ----------------------------------------------------------------------- */
static const char*
get_attribute(xmlElement* e, const char* name) {
  xmlAttribute* a;
  for(a = e->attributes; a; a = a->next) {
    if(!str_diff(a->name, name)) {
      return (const char*)xmlNodeGetContent(a);
    }
  }
  return NULL;
}

/* ----------------------------------------------------------------------- */
static int
get_attribute_sa(stralloc* sa, xmlElement* e, const char* name) {
  xmlAttribute* a;
  for(a = e->attributes; a; a = a->next) {
    if(!str_diff(a->name, name)) {
      const char value = (const char*)xmlNodeGetContent(a);
      stralloc_copys(sa, value);
      stralloc_nul(sa);
      return 1;
    }
  }
  return 0;
}

/* ----------------------------------------------------------------------- */
static int
get_attribute_double(double* d, xmlElement* e, const char* name) {
  stralloc sa;
  stralloc_init(&sa);
  if(!get_attribute_sa(&sa,e, name))
    return 0;
  stralloc_nul(&sa);
  if(scan_double(sa.s, d) == sa.len)
    return 1;
  return 0;
}

/* ----------------------------------------------------------------------- */
static void
cat_attributes(stralloc* sa, xmlElement* e) {
  xmlAttribute* a;
  for(a = e->attributes; a; a = a->next) {
    const char* value = (const char*)xmlNodeGetContent(a);
  stralloc_cats(sa, "\n  ");
  stralloc_cats(sa, (const char*)a->name);
  stralloc_cats(sa, "=\"");
  if(value) stralloc_cats(sa, value);
  stralloc_catb(sa, "\"", 1);
  }
}
/* ----------------------------------------------------------------------- */
static void
process_instance(xmlElement* e) {
  double x = 0.0, y = 0.0, rot = 0.0;
  stralloc part, gate, rot;
  stralloc_init(&part); get_attribute_sa(&part, e, "part");
  stralloc_init(&gate); get_attribute_sa(&gate, e, "gate");
  stralloc_init(&rot); get_attribute_sa(&rot, e, "rot");
  
  if(rot.len > 0) {
    const char* r = rot.s;
    while(*r && !isdigit(*r)) ++r;
    
    scan_double(r, &rot);
  }
  
  get_attribute_double(&x, e, "x");
  get_attribute_double(&y, e, "y");
  
  instance_t* newinst = create_instance(part.s, gate.s, x, y, rot);
}

/* ----------------------------------------------------------------------- */
static void
print_element_names(xmlNode* a_node) {
  xmlNode* n = NULL;

  for(n = a_node; n; n = n->next) {
    if(n->type == XML_ELEMENT_NODE) {
    stralloc attrs;
      xmlElement* e = (xmlElement*)n;
    const char* value = (const char*)xmlNodeGetContent(n);

      stralloc_init(&attrs);
      cat_attributes(&attrs, e);
      stralloc_nul(&attrs);

      if(!str_diff((const char*)e->name, "instance")) {
        process_instance(e);
      } else {
        printf("<%s%s>%s</%s>\n", e->name, attrs.s, value ? value : "", e->name);
      }
      stralloc_free(&attrs);
//      print_attributes(e);
    }
    print_element_names(n->children);
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
  doc = xmlCtxtReadFile(ctxt, filename, NULL, XML_PARSE_RECOVER
                                            | XML_PARSE_NOENT
                                            | XML_PARSE_NOBLANKS
                                            | XML_PARSE_NSCLEAN
                                            | XML_PARSE_COMPACT);

  /* check if parsing suceeded */
  if(doc == NULL) {
    fprintf(stderr, "Failed to parse %s\n", filename);
    xmlFreeParserCtxt(ctxt);
    return 1;
  }
    /* check if validation suceeded */
/*    if(ctxt->valid == 0)
      fprintf(stderr, "Failed to validate %s\n", filename);
*/
  *p_doc = doc;

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
  int ret, i, numAttrs = 0;
  set_element_name((const char*)name);

  if(attrs) {
    for(i = 0; attrs[i]; ++i) {}
    numAttrs = i >> 1;
  }

  printf("<%s> %d\n", name, numAttrs);

  for(i = 0; i < numAttrs; ++i) {
    char* attr = ((char**)attrs)[i << 1],
          *value = ((char**)attrs)[(i << 1) + 1];

    printf("<%s> %d/%d: %s=\"%s\"\n", name, i, numAttrs, attr, value);

    ret = hmap_add(&hashmap, attr, str_len(attr), 1, HMAP_DATA_TYPE_CHARS, value, str_len(value));

  }
}

/* ----------------------------------------------------------------------- */
static void
on_start_element_ns(void* ctx, const xmlChar* name, const xmlChar* prefix,
                    const xmlChar* URI, int nb_nss, const xmlChar** nss, int nb_attrs,
                    int nb_defaulted, const xmlChar** attrs) {


  set_element_name((const char*)name);
  /*  printf("<%s> %d\n", name, numAttrs);

    for(i = 0; i < numAttrs; ++i) {
      printf("<%s> %d/%d %s\n", name, i, numAttrs, attributes[i]);
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
on_end_element_ns(void* ctx, const xmlChar* name, const xmlChar* prefix,
                  const xmlChar* URI) {
  after_element((const char*)name);
}

/* ----------------------------------------------------------------------- */
static void
after_element(const char* name) {
  stralloc saa;
  stralloc_init(&saa);

  attr_list(&saa, hashmap);
  stralloc_nul(&saa);

  if(saa.len)
    printf("<%s> attrs:%s\n", get_element_name(), saa.s);

  stralloc_free(&saa);

  hmap_destroy(&hashmap);
  hmap_init(1024, &hashmap);

  stralloc_zero(&character_buf);
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
  hmap_init(1024, &hashmap);
  hmap_init(1024, &instances_db);

  if(argc > 1)
    filename = argv[1];

  /*   if(read_xmlfile(f)) {*/
  if(parse_xmlfile(filename, &xmldoc)) {
    puts("xml read error.");
    exit(1);
  }

  /* Get the root element node */
  root_element = xmlDocGetRootElement(xmldoc);
  print_element_names(root_element);

  /* free up the resulting document */
  xmlFreeDoc(xmldoc);

  return 0;
}
