#include "lib/buffer.h"
#include "lib/stralloc.h"
#include <stdbool.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

static stralloc url, templ;
static char sep = '?';

static void
print_attr_names(xmlElement* elm) {
  bool param = !strcmp((const char*)elm->name, "Param");
  xmlAttribute* attr_p;
  for(attr_p = (xmlAttribute*)elm->attributes; attr_p; attr_p = (xmlAttribute*)attr_p->next) {
    const char* content = (const char*)xmlNodeGetContent((xmlNodePtr)attr_p);
    bool name = !strcmp((const char*)attr_p->name, "name");
    if(param) {
      stralloc_catb(&url, name ? &sep : "=", 1);
      stralloc_cats(&url, content);
      sep  = '&';
    }
    /*   buffer_puts(buffer_2,  "\n  attribute name: ");
       buffer_puts(buffer_2, attr_p->name);
       buffer_puts(buffer_2, param ? "=" : "\n  attribute value: ");
       buffer_puts(buffer_2, content);*/
    if(!strcmp((const char*)attr_p->name, "template")) {
      stralloc_copys(&templ, content);
      //templ = content;
    }
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
  size_t i;
  stralloc_init(&url);
  stralloc_init(&templ);
  a_node = a_node->children;
  for(cur_node = a_node; cur_node; cur_node = cur_node->next) {
    if(cur_node->type == XML_ELEMENT_NODE) {
      xmlElement* elm = (xmlElement*)cur_node;
      xmlNs* ns = cur_node->ns;
      const char* nsStr = ns ? (const char*)ns->prefix : NULL;
      /*    buffer_puts(buffer_2, "node type: Element, name: ");
          if(nsStr) {
            buffer_puts(buffer_2, nsStr);
            buffer_put(buffer_2, ":", 1);
          }
          buffer_puts(buffer_2, elm->name);
          buffer_puts(buffer_2, ", value: ");
      */
      //      print_element_names(elm->children);
      if(!strcmp((const char*)elm->name, "Url")) {
        print_attr_names(elm);
        xmlNode* child_node = NULL;
        for(child_node = elm->children; child_node; child_node = child_node->next) {
          //    if(child_node->type == XML_ELEMENT_NODE)
          {
            print_attr_names((xmlElement*)child_node);
          }
        }
      }
      /*        if(!strcmp(elm->name, "Url") || !strcmp(elm->name, "Param")) {
                print_attr_names(elm);
              }*/
      //   buffer_putnlflush(buffer_2);
    }
  }
  stralloc newurl;
  stralloc_init(&newurl);
  stralloc_copy(&newurl, &templ);
  stralloc_cat(&newurl, &url);
  stralloc_copy(&url, &newurl);
  stralloc_zero(&newurl);
  /* buffer_puts(buffer_1, "url: ");
    buffer_putsa(buffer_1, &url);
    buffer_putnlflush(buffer_1);*/
  for(i = 0; i < url.len; ++i) {
    if(url.s[i] == '{') {
      /*  buffer_put(buffer_2, &url.s[i], 13);
        buffer_putnlflush(buffer_2); */
      stralloc_cats(&newurl, "%s");
      while(++i < url.len) {
        if(url.s[i] == '}') {
          break;
        }
      }
      continue;
    }
    stralloc_catb(&newurl, &url.s[i], 1);
  }
  buffer_puts(buffer_1, "newurl: ");
  buffer_putsa(buffer_1, &newurl);
  buffer_putnlflush(buffer_1);
}

int
parse_xml(const char* filename) {
  xmlDoc* doc = NULL;
  xmlNode* root_element = NULL;
  /*
   * this initialize the library and check potential ABI mismatches
   * between the version it was compiled for and the actual shared
   * library used.
   */
  LIBXML_TEST_VERSION
  /*parse the file and get the DOM */
  doc = xmlReadFile(filename, NULL, 0);
  if(doc == NULL) {
    buffer_puts(buffer_2, "error: could not parse file ");
    buffer_puts(buffer_2, filename);
    buffer_putnlflush(buffer_2);
    return -1;
  }
  /*Get the root element node */
  root_element = xmlDocGetRootElement(doc);
  print_element_names(root_element);
  /*free the document */
  xmlFreeDoc(doc);
  /*
   *Free the global variables that may
   *have been allocated by the parser.
   */
  return 0;
}

/**
 * Simple example to parse a file called "file.xml",
 * walk down the DOM, and print the name of the
 * xml elements nodes.
 */
int
main(int argc, char** argv) {
  int ai;
  if(argc < 2)
    return 1;
  for(ai = 1; ai < argc; ++ai) {
    int ret = parse_xml(argv[ai]);
    if(ret == -1)
      return 1;
  }
  xmlCleanupParser();
  return 0;
}
