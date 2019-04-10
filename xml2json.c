#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/hmap.h"
#include "lib/iterator.h"
#include "lib/stralloc.h"
#include "lib/xml.h"
#include "lib/str.h"
#include "lib/mmap.h"
#include "lib/scan.h"
#include "lib/strlist.h"
#include "lib/json.h"
#include "lib/array.h"
#include "lib/textcode.h"

static jsonval xml_to_json_obj(xmlnode* node);

static int one_line = 0, indent = 2, compact = 0;
static stralloc indent_str = { " ", 1, 0 };


static void
max_depth_fn(jsonval* v, int* arg, int depth) {
  if(depth > *arg)
    *arg = depth;
}

/**
 * @brief      Gets depth of JSON node tree.
 * @param      v     JSON value
 * @return     The depth.
 */
static int
get_depth(jsonval* v) {
  int max_depth = -1;
  json_recurse(v, max_depth_fn, &max_depth);
  return max_depth;
}

/**
 * @brief      { function_description }
 * @param      p      { parameter_description }
 * @param      v      { parameter_description }
 * @param[in]  depth  The depth
 * @param[in]  index  The index
 */
static void
pretty_printer(jsonfmt* p, jsonval* v, int depth, int index) {
  int valdepth = get_depth(v); 
  p->newline = (!one_line  && valdepth > 1 && ((index > -1) || index < -2) && index > 0) ? "\n" : "";
  p->indent = indent_str.s;
  p->spacing = " "; //((valdepth < 1 && index > 0) || (valdepth >= 1 &&  index > -1)) ? " " : "";
  p->quote = '"';
}

/**
 * @brief      { function_description }
 * @param      list  The list
 * @return     { description_of_the_return_value }
 */
static jsonval
xmllist_to_jsonarray(xmlnode* list) {
  jsonval arr = json_array();
  xmlnode* n;
  for(n = list; n; n = n->next) {
    if(n->name[0] == '/') continue;
    jsonval* item = json_push(&arr, xml_to_json_obj(n));
  }
  return arr;
}

/**
 * @return     1 on succes
 */
static int
hmap_to_jsonobj(HMAP_DB* db, jsonval* obj) {
  if(db && db->list_tuple) { 
  TUPLE* t;
    hmap_foreach(db, t) {
      json_set_property(obj, json_string(t->key), json_stringn(t->vals.val_chars, t->data_len));
    }
    return 1;
  }
  return 0;
}

/**
 * @brief      { function_description }
 * @param      node  The node
 * @return     { description_of_the_return_value }
 */
static jsonval
xml_to_json_obj(xmlnode* node) {
  TUPLE* t;
  jsonval obj = json_object();
  json_set_property(&obj, json_string("name"), json_string(node->name));
  if(node->attributes && node->attributes->list_tuple)
    hmap_to_jsonobj(node->attributes, &obj);
  if(node->children)
    json_set_property(&obj, json_string("children"), xmllist_to_jsonarray(node->children));
  return obj;
}

static jsonval
xml_to_json(xmlnode* node) {
  if(node->type == XML_DOCUMENT || node->attributes == NULL)
    return xmllist_to_jsonarray(node->children);
  return xml_to_json_obj(node);
}

int
main(int argc, char* argv[]) {
  buffer* input = buffer_0;
  xmlnode* doc;
  static xmlnodeset ns;
  xmlnodeset_iter_t it, e;
  size_t i = 0;

  if(argv[1]) {
   input->fd = open_read(argv[1]);
  }

  buffer_skip_until(input, "\r\n", 2);
  doc = xml_read_tree(input);
   buffer_close(input);
  jsonval root = xml_to_json(doc);

  json_print(root, buffer_1, 0);

  /*
   * Cleanup function for the XML library.
   */
  xml_free(doc);
  return (0);
}
