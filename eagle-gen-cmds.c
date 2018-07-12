#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#if !defined(_WIN32) && !(defined(__MSYS__) && __MSYS__ == 1)
#include <libgen.h>
#endif

#include <libxml/SAX.h>

#include "lib/buffer.h"
#include "lib/fmt.h"
#include "lib/hmap.h"
#include "lib/mmap.h"
#include "lib/scan.h"
#include "lib/str.h"
#include "lib/stralloc.h"

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
#include <stdio.h>

static const char* document = "<doc/>";

void
print_node(xmlNode* node) {
  buffer_putm(buffer_1, "<", node->name, ">", NULL);
  buffer_putnlflush(buffer_1);
}

static void
print_element_name(xmlNode* a_node) {
  if(!str_diff(a_node->name, "eagle")) return;
  if(!str_diff(a_node->name, "drawing")) return;
  if(a_node->parent) {

    print_element_name(a_node->parent);
  }

  if(a_node->name) buffer_putm(buffer_1, a_node->parent ? "/" : "", a_node->name, NULL);
}

static void
hashmap_dump(HMAP_DB* db, const char* name) {

  int i = 0;
  TUPLE* tuple = NULL;
  tuple = db->tuple;  buffer_putm(buffer_1, name, ": ", NULL);
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

static int
str_isspace(const char* s) {
  while(*s) {
    if(!isspace(*s)) return 0;
    s++;
  }
  return 1;
}

static int
str_isfloat(const char* s) {
  while(*s) {
    static const char floatchars[] = "0123456789+-.Ee";

    if(!floatchars[str_chr(floatchars, *s)]) return 0;
    s++;
  }
  return 1;
}

static HMAP_DB*
element_to_hashmap(xmlElement* elm) {
  HMAP_DB* hash;

  hmap_init(1024, &hash);

  xmlAttr* ptr;
  for(ptr = elm->attributes; ptr; ptr = ptr->next) {

    //    if(ptr->type == XML_ELEMENT_CONTENT_ELEMENT) continue;p
    if(ptr->name && str_len(ptr->name)) {

      xmlChar* content = xmlGetProp(elm, ptr->name); // xmlNodeGetContent(ptr);
      if(content && str_len(content) && !str_isspace(content)) {
        if(str_isfloat(content)) {
          hmap_add(&hash, ptr->name, str_len(ptr->name), 1, HMAP_DATA_TYPE_DOUBLE, strtod(content, NULL) / 2.54);

        } else {

          size_t len = str_len(content);
          const char* in = content, * dest = malloc(len * 4 + 1);
          size_t i = 0;

          for(i = 0; *in; ++in) {

            i += fmt_escapecharquotedprintableutf8(&dest[i], *in);
          }
          //        dest[i] = '\0';
          hmap_add(&hash, ptr->name, str_len(ptr->name), 0, HMAP_DATA_TYPE_CHARS, dest, i);
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
static void
print_element_names(xmlNode* a_node) {
  xmlNode* cur_node = NULL;

  for(cur_node = a_node; cur_node; cur_node = cur_node->next) {

    if(cur_node->type == XML_ELEMENT_NODE) {
      xmlElement* elm = (xmlElement*)cur_node;

      print_element_name(cur_node);

      HMAP_DB* hmap = element_to_hashmap(elm);

      hashmap_dump(hmap, elm->name);

      hmap_destroy(&hmap);

      /*

      for(xmlAttribute* attr = elm->attributes; attr; attr = attr->next) {

        xmlChar* prop = xmlGetProp(cur_node, attr->name);
        buffer_putm(buffer_1, " ", attr->name, str_isfloat(prop) ? "=" : "=\"", prop, str_isfloat(prop) ? "" : "\"", NULL);
      }*/

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
static xmlDocPtr
read_xml_tree(const char* filename, void* ptr, size_t length) {
  xmlDocPtr doc; /* the resulting document tree */

  /*
   * The document being in memory, it have no base per RFC 2396,
   * and the "noname.xml" argument will serve as its base.
   */
  doc = xmlReadMemory(ptr, length, filename, "UTF8", XML_PARSE_RECOVER);
  if(doc == NULL) {
    fprintf(stderr, "Failed to parse document\n");
    return NULL;
  }
  return doc;
}

int
main(int argc, char* argv[]) {
  /*
   * this initialize the library and check potential ABI mismatches
   * between the version it was compiled for and the actual shared
   * library used.
   */
  LIBXML_TEST_VERSION

  size_t mapsz;
  void* ptr = mmap_private(argv[1], &mapsz);

  xmlDocPtr* doc = read_xml_tree(argv[1], ptr, mapsz);

  xmlNodePtr node = xmlDocGetRootElement(doc);
  size_t child_count = xmlChildElementCount(node);

  print_element_names(node);

  xmlNode* child;
  for(child = node->children; child; child = child->next) {

    if(child->type == XML_ELEMENT_NODE) {

      // if(child->type == XML_ELEMENT_TYPE_ANY)
      HMAP_DB* db = element_to_hashmap((xmlElement*)child);

      hashmap_dump(db, child->name);

      hmap_destroy(&db);
    }
    // print_node(child);
  }

  /*
   * Cleanup function for the XML library.
   */
  xmlCleanupParser();
  mmap_unmap(ptr, mapsz);
  /*
   * this is to debug memory for regression tests
   */
  //    xmlMemoryDump();
  return (0);
}
