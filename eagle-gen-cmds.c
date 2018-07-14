#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#if !defined(_WIN32) && !(defined(__MSYS__) && __MSYS__ == 1)
#include <libgen.h>
#endif
#include "lib/buffer.h"
#include "lib/fmt.h"
#include "lib/hmap.h"
#include "lib/mmap.h"
#include "lib/scan.h"
#include "lib/str.h"
#include "lib/stralloc.h"
#include <libxml/SAX.h>
/**
 * section: Parsing
 * synopsis: Parse an XML document in memory to a tree and free it
 * purpose: Demonstrate the use of xmlReadMemory() to read an XML file
 *          into a tree and and xmlFreeDoc() to free the resulting tree
 * usage: parse3
 * test: parse3
 * author: Daniel Veillard
 * copy: see Copyright for the status of this software.
 */
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <stdio.h>

#define node_name(n) ((char*)(n)->name)
#define node_prop(n, p) ((char*)xmlGetProp((xmlNode*)(n), (xmlChar*)p))
#define node_is_elem(n) (((xmlNode*)(n))->type == 1)
#define node_attrs(n) (void*)(node_is_elem(n) ? ((xmlElement*)(n))->attributes : NULL)

const char* document = "<doc/>";

void
node_print(xmlNode* node);
int
node_depth(xmlNode* node);
int
str_ischarset(const char* s, const char* set);
int
str_isfloat(const char* s);
int
str_isspace(const char* s);
void
print_element_attrs(xmlNode* a_node);

xmlXPathObject*
getnodeset(xmlDoc* doc, const char* xpath) {

  xmlXPathContext* context;
  xmlXPathObject* result;

  context = xmlXPathNewContext(doc);
  if(context == NULL) {
    buffer_putsflush(buffer_2, "Error in xmlXPathNewContext\n");
    return NULL;
  }
  result = xmlXPathEvalExpression((xmlChar*)xpath, context);
  xmlXPathFreeContext(context);
  if(result == NULL) {
    buffer_putsflush(buffer_2, "Error in xmlXPathEvalExpression\n");
    return NULL;
  }
  if(xmlXPathNodeSetIsEmpty(result->nodesetval)) {
    xmlXPathFreeObject(result);
    buffer_putsflush(buffer_2, "No result\n");
    return NULL;
  }
  return result;
}

/**
 *  node_print: Prints XML node
 */
void
node_print(xmlNode* node) {
  buffer_putm(buffer_1, "<", node->name, NULL);

  print_element_attrs(node);

  buffer_putm(buffer_1, ">", NULL);
  buffer_putnlflush(buffer_1);
}

/**
 *  hashmap_dump: Gets depth of node in hierarchy
 */
int
node_depth(xmlNode* node) {
  size_t i = 0;
  while((node = node->next))
    ++i;
  return i;
}

int
str_ischarset(const char* s, const char* set) {
  while(*s) {
    if(set[str_chr(set, *s)] == '\0')
      return 0;
    ++s;
  }
  return 1;
}

int
str_isfloat(const char* s) {
  return str_ischarset(s, "0123456789.-+Ee");
}
int
str_isdoublenum(const char* s) {
  char* end;
  strtod(s, &end);
  return (const char*)end > s;
}

int
str_isspace(const char* s) {
  return str_ischarset(s, "\t\r\n\v ");
}

/**
 *  hashmap_dump: Outputs hashmap to stdout
 */
void
print_element_name(xmlNode* a_node) {
  const char* name = node_name(a_node);

  if(a_node->parent) {
    xmlNode* p = a_node->parent;
    ;
    const char* pn = node_name(p);

    if(pn && !str_diffn(pn, name, str_len(name))) {
      p = p->parent;
    }

    print_element_name(p);
  }

  if(node_name(a_node)) {

    if(str_diff(node_name(a_node), "eagle") && str_diff(node_name(a_node), "drawing")) {

      buffer_putm(buffer_1, a_node->parent ? "/" : "", node_name(a_node), NULL);
    }
  }
}

/**
 *  print_element_attrs: Prints all element attributes to stdout
 */
void
print_element_attrs(xmlNode* a_node) {

  if(!node_is_elem(a_node))
    return;

  for(xmlNode* a = node_attrs(a_node); a; a = a->next) {
    const char* v = node_prop(a_node, a->name);
    buffer_putm(buffer_1, " ", node_name(a), str_isdoublenum(v) ? "=" : "=\"", v, str_isdoublenum(v) ? "" : "\"", NULL);
  }
}

/**
 *  hashmap_dump: Outputs hashmap to stdout
 */
void
hashmap_dump(HMAP_DB* db, const char* name) {
  int i = 0;
  TUPLE* tuple = NULL;
  tuple = db->tuple;
  buffer_putm(buffer_1, name, ": ", NULL);
  for(i = 0; i < db->bucket_size; i++) {
    switch(tuple->data_type) {
      case HMAP_DATA_TYPE_CHARS:
        buffer_putm(buffer_1, " ", tuple->key, "=", tuple->data, NULL);
        buffer_putnlflush(buffer_1);
        break;
      case HMAP_DATA_TYPE_DOUBLE: {
        char dbl[100];
        fmt_double(dbl, *(double*)tuple->data, sizeof(dbl), -1);
        buffer_putm(buffer_1, " ", tuple->key, "=", dbl, NULL);
        buffer_putnlflush(buffer_1);
        break;
      }
    }
  }
}

HMAP_DB*
element_to_hashmap(xmlElement* elm) {
  HMAP_DB* hash;
  hmap_init(1024, &hash);
  for(xmlNode* ptr = node_attrs(elm); ptr; ptr = ptr->next) {
    char* name = node_name(ptr);
    if(name && str_len(name)) {
      char* content = node_prop(elm, name);
      if(content && str_len(content) && !str_isspace(content)) {
        if(str_isfloat(content)) {
          hmap_add(&hash, name, str_len(name), 1, HMAP_DATA_TYPE_DOUBLE, strtod(content, NULL) / 2.54);
        } else {
          size_t i, len = str_len(content);
          char *in = content, *dest = malloc(len * 4 + 1);
          for(i = 0; *in; ++in) {
            i += fmt_escapecharquotedprintableutf8(&dest[i], *in);
          }
          //        dest[i] = '\0';
          hmap_add(&hash, name, str_len(name), 0, HMAP_DATA_TYPE_CHARS, dest, i);
          //  free(dest);
        }
      }
    }
  }
  //  hashmap_dump(hash);
  return hash;
}

/**
 * print_element_names:
 * @a_node: the initial xml node to consider.
 *
 * Prints the names of the all the xml elements
 * that are siblings or children of a given xml node.
 */
void
print_element_names(xmlNode* cur_node) {
  for(; cur_node; cur_node = cur_node->next) {
    if(node_is_elem(cur_node)) {
      print_element_name(cur_node);

      if(node_depth(cur_node) >= 1) {

        print_element_attrs(cur_node);

        //      HMAP_DB* hmap = element_to_hashmap(elm);
        //      hashmap_dump(hmap, elm->name);
        //      hmap_destroy(&hmap);
        //      for(xmlAttribute* attr = elm->attributes; attr; attr = attr->next) {
        //        xmlChar* prop = xmlGetProp(cur_node, attr->name);
        //        buffer_putm(buffer_1, " ", attr->name, str_isfloat(prop) ? "=" : "=\"", prop, str_isfloat(prop) ? "" : "\"", NULL);
        //      }
      }
      buffer_putnlflush(buffer_1);
      //      printf("node type: Element, name: %s\n", cur_node->name);
    }
    print_element_names(cur_node->children);
  }
}

/**
 * example3Func:
 * @content: the content of the document
 * @length: the length in bytes
 *
 * Parse the in memory document and free the resulting tree
 */

int buffer_read(void* ptr, char* buf, int len) {
  return buffer_get(ptr, buf, len);

}

xmlDoc*
read_xml_tree(const char* filename, buffer* in) {
  xmlDoc* doc; /* the resulting document tree */
  /*
   * The document being in memory, it have no base per RFC 2396,
   * and the "noname.xml" argument will serve as its base.
   */
  doc = xmlReadFile(filename, "UTF-8", XML_PARSE_RECOVER);

//  doc = xmlReadIO(buffer_read, (void*)buffer_close, in, XML_XML_NAMESPACE,  "UTF-8", XML_PARSE_RECOVER);

  if(doc == NULL) {
    buffer_puts(buffer_2, "Failed to parse document");
    buffer_putnlflush(buffer_2);
    return NULL;
  }
  return doc;
}
void
xpath_query(xmlDoc* doc, const char *q) {

  xmlXPathObject* nodes = getnodeset(doc, q);

  buffer_puts(buffer_2, "nodes: ");
  buffer_putulong(buffer_2, xmlXPathNodeSetGetLength(nodes->nodesetval));
  buffer_putnlflush(buffer_2);

  for(int i = 0; i < xmlXPathNodeSetGetLength(nodes->nodesetval); ++i) {
     xmlNode* node = xmlXPathNodeSetItem(nodes->nodesetval, i);

     print_element_name(node);
     print_element_attrs(node);
     buffer_putnlflush(buffer_1);

     stralloc query;
     stralloc_init(&query);
     stralloc_catm_internal(&query, "//*[@", &q[2], "='", xmlGetProp(node, (xmlChar*)"name"), "']", NULL);
     stralloc_0(&query);

     xpath_query(doc, query.s);

     //title[@lang='en']
  }

}

int
main(int argc, char* argv[]) {
  /*
   * this initialize the library and check potential ABI mismatches
   * between the version it was compiled for and the actual shared
   * library used.
   */
  LIBXML_TEST_VERSION
  if(!argv[1])
    argv[1] = "C:/Users/roman/Documents/Sources/an-tronics/eagle/40106-4069-Synth.brd";

//  size_t mapsz;
//  void* ptr = mmap_private(argv[1], &mapsz);

  buffer input;
  buffer_mmapprivate(&input, argv[1]);
  buffer_skip_until(&input, "\r\n", 2);

  xmlDoc* doc = read_xml_tree(argv[1], &input);
  xmlNode* node = xmlDocGetRootElement(doc);
  size_t child_count = xmlChildElementCount(node);
  print_element_names(node);
  xmlNode* child;
  for(child = node->children; child; child = child->next) {
    if(child->type == XML_ELEMENT_NODE) {
      // if(child->type == XML_ELEMENT_TYPE_ANY)
      HMAP_DB* db = element_to_hashmap((xmlElement*)child);
      hashmap_dump(db, node_name(child));
      hmap_destroy(&db);
    }
    // print_node(child);
  }

xpath_query(doc, "//part");

  /*
   * Cleanup function for the XML library.
   */
  xmlCleanupParser();
  /*
   * this is to debug memory for regression tests
   */
  //    xmlMemoryDump();
  return (0);
}
