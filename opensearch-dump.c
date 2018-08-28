#include "lib/buffer.h"
#include "lib/stralloc.h"
#include <stdbool.h>
#include "lib/xml.h"

static stralloc url, templ;
static char sep = '?';

static void
print_attr_names(xmlnode* elm) {
  bool param = !str_diff((const char*)elm->name, "Param");
  TUPLE* attr_p;
  for(attr_p = (TUPLE*)elm->attributes; attr_p; attr_p = (TUPLE*)attr_p->next) {
    const char* content = (const char*)xml_content((xmlnode*)attr_p);
    bool name = !str_diff((const char*)attr_p->key, "name");
    if(param) {
      stralloc_catb(&url, name ? &sep : "=", 1);
      stralloc_cats(&url, content);
      sep  = '&';
    }
    /*   buffer_puts(buffer_2,  "\n  attribute name: ");
       buffer_puts(buffer_2, attr_p->name);
       buffer_puts(buffer_2, param ? "=" : "\n  attribute value: ");
       buffer_puts(buffer_2, content);*/
    if(!str_diff((const char*)attr_p->key, "template")) {
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
print_element_names(xmlnode* a_node) {
  xmlnode* cur_node = NULL;
  size_t i;
  stralloc_init(&url);
  stralloc_init(&templ);
  a_node = a_node->children;
  for(cur_node = a_node; cur_node; cur_node = cur_node->next) {
    if(cur_node->type == XML_ELEMENT) {
      xmlnode* elm = (xmlnode*)cur_node;
     /*    buffer_puts(buffer_2, "node type: Element, name: ");
          if(nsStr) {
            buffer_puts(buffer_2, nsStr);
            buffer_put(buffer_2, ":", 1);
          }
          buffer_puts(buffer_2, elm->name);
          buffer_puts(buffer_2, ", value: ");
      */
      /*        */
      if(!str_diff((const char*)elm->name, "Url")) {
        print_attr_names(elm);
        xmlnode* child_node = NULL;
        for(child_node = elm->children; child_node; child_node = child_node->next) {
          /*            */
          {
            print_attr_names((xmlnode*)child_node);
          }
        }
      }
      /*        if(!str_diff(elm->name, "Url") || !str_diff(elm->name, "Param")) {
                print_attr_names(elm);
              }*/
      /*        */
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
  xmlnode* doc = NULL;
  xmlnode* root_element = NULL;



  /*parse the file and get the DOM */


  buffer input;
  buffer_mmapprivate(&input, filename);

  doc = xml_read_tree(&input);

  if(doc == NULL) {
    buffer_puts(buffer_2, "error: could not parse file ");
    buffer_puts(buffer_2, filename);
    buffer_putnlflush(buffer_2);
    return -1;
  }
  /*Get the root element node */
  root_element = doc;
  print_element_names(root_element);
  /*free the document */
  xml_free(doc);
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
  return 0;
}
