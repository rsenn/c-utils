#include "../lib/stralloc.h"
#include "unit_test.h"
#include "../lib/stralloc.h"
#include "../lib/alloc.h"
#include "../lib/strlist.h"
#include "../lib/xml.h"
#include "../lib/str.h"
#include "../lib/stralloc.h"
#include "../lib/buffer.h"
#include "../lib/byte.h"
#include "../lib/open.h"
#include <unistd.h>

/*
 * xmlnode* xml_element(const char*);
 * xmlnode* xml_child_element_attrs(const char*, xmlnode*, const char*, ...);
 * xmlnode* xml_child_element_text(const char*, xmlnode*, const char*);
 * void xml_print(xmlnode*, buffer*, xml_print_fmt_t*);
 */
TEST(test_xml_build_print) {
  static const char expected[] = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                                 "<configurationDescriptor version=\"65\">\n"
                                 "  <property key=\"optimization-level\" value=\"-Os\" />\n"
                                 "  <itemPath>../../../lib/delay.c</itemPath>\n"
                                 "</configurationDescriptor>\n";
  xmlnode *doc, *root;
  buffer out, in;
  char text[256];
  ssize_t n;

  doc = xml_newnode(XML_DOCUMENT);
  root = doc->children = xml_element("configurationDescriptor");
  root->attributes = xml_attributes("version", "65", 0);
  xml_child_element_attrs("property", root, "key", "optimization-level", "value", "-Os", 0);
  xml_child_element_text("itemPath", root, "../../../lib/delay.c");

  buffer_write_fd(&out, open_trunc("test_xml.tmp"));
  xml_print(doc, &out, 0);
  buffer_flush(&out);
  buffer_close(&out);
  xml_free(doc);

  ASSERT_EQ(0, buffer_mmapprivate(&in, "test_xml.tmp"));
  n = buffer_get(&in, text, sizeof(text));
  buffer_close(&in);
  unlink("test_xml.tmp");

  ASSERT_EQ(sizeof(expected) - 1, n);
  ASSERT_EQ(0, byte_diff(text, n, expected));
}

/*
 * xmlnode* xml_read_tree(buffer*);
 */
TEST(test_xml_read_tree) {
  static const char text[] = "<a>\n  <b>x</b>\n</a>\n";
  xmlnode* doc;
  xmlnodeset ns;
  buffer in;

  buffer_frombuf(&in, text, sizeof(text) - 1);
  doc = xml_read_tree(&in);

  ASSERT_NE(NULL, doc);
  ASSERT_STR_EQUAL("a", doc->children->name);

  ns = xml_find_all_1(doc, (xml_pred_t*)&xml_match_name, "b");
  ASSERT_EQ(1, xmlnodeset_size(&ns));

  xml_free(doc);
}

TEST(test_xml_read_tree_attributes) {
  static const char text[] = "<a v=\"1\">\n  <b k=\"x\">t</b>\n  <c d='2' e=3 f />\n</a>\n";
  xmlnode *doc, *a, *b, *c;
  xmlnodeset ns;
  buffer in;

  buffer_frombuf(&in, text, sizeof(text) - 1);
  doc = xml_read_tree(&in);

  ASSERT_NE(NULL, doc);
  a = doc->children;
  ASSERT_STR_EQUAL("a", a->name);
  ASSERT_STR_EQUAL("1", xml_get_attribute(a, "v"));

  ns = xml_find_all_1(doc, (xml_pred_t*)&xml_match_name, "b");
  ASSERT_EQ(1, xmlnodeset_size(&ns));
  b = ns.nodes[0];
  ASSERT_STR_EQUAL("x", xml_get_attribute(b, "k"));
  ASSERT_EQ(NULL, xml_get_attribute(b, "v"));
  ASSERT_STR_EQUAL("t", b->children->name);

  /* self-closing, single quotes, no quotes and no value */
  ns = xml_find_all_1(doc, (xml_pred_t*)&xml_match_name, "c");
  ASSERT_EQ(1, xmlnodeset_size(&ns));
  c = ns.nodes[0];
  ASSERT_EQ(NULL, c->children);
  ASSERT_STR_EQUAL("2", xml_get_attribute(c, "d"));
  ASSERT_STR_EQUAL("3", xml_get_attribute(c, "e"));
  ASSERT_STR_EQUAL("", xml_get_attribute(c, "f"));

  xml_free(doc);
}

TEST(test_xml_read_tree_markup) {
  static const char text[] =
      "<?xml version=\"1.0\"?><!DOCTYPE r><!-- a > b --><r><![CDATA[x<y]]><c/>tail &amp; more<e>1</e>end</r>";
  xmlnode *doc, *r, *n;
  buffer in;

  buffer_frombuf(&in, text, sizeof(text) - 1);
  doc = xml_read_tree(&in);

  ASSERT_NE(NULL, doc);
  ASSERT_STR_EQUAL("?xml", doc->children->name);
  r = doc->children->next;
  ASSERT_STR_EQUAL("r", r->name);

  /* text, elements and text again keep their order */
  n = r->children;
  ASSERT_STR_EQUAL("x<y", n->name);
  n = n->next;
  ASSERT_STR_EQUAL("c", n->name);
  n = n->next;
  ASSERT_STR_EQUAL("tail & more", n->name);
  n = n->next;
  ASSERT_STR_EQUAL("e", n->name);
  ASSERT_STR_EQUAL("1", n->children->name);
  n = n->next;
  ASSERT_STR_EQUAL("end", n->name);
  ASSERT_EQ(NULL, n->next);

  xml_free(doc);
}

TEST(test_xml_read_tree_stray_close) {
  static const char text[] = "</x><a>t</a></y>";
  xmlnode* doc;
  buffer in;

  buffer_frombuf(&in, text, sizeof(text) - 1);
  doc = xml_read_tree(&in);

  ASSERT_NE(NULL, doc);
  ASSERT_STR_EQUAL("a", doc->children->name);

  xml_free(doc);
}

/*
 * void xml_scanner_buf(xmlscanner*, buffer*);
 * xmltoken xml_read_token(xmlscanner*);
 */
TEST(test_xml_read_token) {
  static const char text[] = "<a k=\"v\">text</a>";
  static const xmltokid expected[6] = {XML_TAG_NAME, XML_ATTR_NAME, XML_ATTR_VALUE, XML_TAG_CLOSE, XML_DATA, XML_TAG_CLOSE};
  xmlscanner s;
  xmltoken tok;
  buffer in;
  size_t i = 0;

  buffer_frombuf(&in, text, sizeof(text) - 1);
  xml_scanner_buf(&s, &in);

  do {
    tok = xml_read_token(&s);

    if(tok.id != XML_EOF && i < 6)
      ASSERT_EQ(expected[i], tok.id);
    i++;
  } while(tok.id != XML_EOF);

  ASSERT_EQ(7, i);
}


static const char tree_doc[] = "<root a=\"1\"><item id=\"x\" n=\"3.5\">one</item><item id=\"y\">two <b>bold</b></item><other/></root>";

static xmlnode*
load(buffer* b, const char* text, size_t n) {
  buffer_frombuf(b, text, n);
  return xml_read_tree(b);
}

static int
sa_is(const stralloc* sa, const char* s) {
  return sa->len == str_len(s) && byte_equal(sa->s, sa->len, s);
}

/*
 * xml_root_element, xml_num_children, xml_find_element, xml_get_attribute*, xml_has_attribute*
 */
TEST(test_xml_navigate_attributes) {
  buffer b;
  xmlnode *d, *root, *item;
  stralloc sa;

  stralloc_init(&sa);
  d = load(&b, tree_doc, sizeof(tree_doc) - 1);
  root = xml_root_element(d);
  ASSERT_STR_EQUAL("root", root->name);
  ASSERT_EQ(XML_DOCUMENT, d->type);
  ASSERT_EQ(3, xml_num_children(root));
  ASSERT_EQ(1, xml_get_document(root) == d);

  item = xml_find_element(root, "item");
  ASSERT_EQ(1, item != NULL);
  ASSERT_STR_EQUAL("x", xml_get_attribute(item, "id"));
  ASSERT_EQ(1, xml_get_attribute(item, "nope") == NULL);
  ASSERT_EQ(1, xml_get_attribute_long(root, "a"));
  ASSERT_EQ(1, xml_get_attribute_double(item, "n") == 3.5);
  ASSERT_EQ(1, xml_has_attribute(item, "id"));
  ASSERT_EQ(0, xml_has_attribute(item, "zz"));
  ASSERT_EQ(1, xml_has_attribute_n(item, "idXX", 2));
  ASSERT_EQ(1, xml_get_attribute_sa(item, &sa, "id"));
  ASSERT_EQ(1, sa_is(&sa, "x"));
  ASSERT_EQ(0, xml_get_attribute_sa(item, &sa, "zz"));
  ASSERT_STR_EQUAL("one", xml_content(item));
  xml_free(d);
  stralloc_free(&sa);
}

TEST(test_xml_find_element_attr_text) {
  buffer b;
  xmlnode *d, *root, *y, *bold;
  stralloc sa;

  stralloc_init(&sa);
  d = load(&b, tree_doc, sizeof(tree_doc) - 1);
  root = xml_root_element(d);
  y = xml_find_element_attr(root, "item", "id", "y"); /* by tag, attribute and value */
  ASSERT_EQ(1, y != NULL);
  ASSERT_STR_EQUAL("y", xml_get_attribute(y, "id"));
  ASSERT_EQ(1, xml_find_element_attr(root, "item", "id", "zz") == NULL);

  xml_get_text(y, &sa); /* the text of the element (the text node and the ones after it) */
  ASSERT_EQ(1, sa.len >= 3 && byte_equal(sa.s, 3, "two"));
  stralloc_zero(&sa);
  ASSERT_EQ(3, xml_content_sa(y, &sa) - 0 > 0 ? 3 : 0);

  bold = xml_find_element(root, "b");
  ASSERT_EQ(1, bold != NULL);
  ASSERT_EQ(1, xml_find_parent(bold, "item") == y);
  ASSERT_EQ(1, xml_path(bold, &sa));
  ASSERT_EQ(1, sa_is(&sa, "/root/item/b"));
  ASSERT_EQ(1, xml_path_relative(bold, &sa));
  ASSERT_EQ(1, sa_is(&sa, "/item/b"));
  xml_get_text(NULL, &sa); /* used to crash */
  xml_free(d);
  stralloc_free(&sa);
}

/*
 * xml_set_attribute_double, xml_set_attributes, xml_add_child, xml_element, xml_remove, xml_delete
 */
TEST(test_xml_modify) {
  buffer b;
  xmlnode *d, *root, *item, *added, **np, *gone;

  d = load(&b, tree_doc, sizeof(tree_doc) - 1);
  root = xml_root_element(d);
  item = xml_find_element(root, "item");
  xml_set_attribute_double(item, "n", 1.25, 2);
  ASSERT_STR_EQUAL("1.25", xml_get_attribute(item, "n"));
  ASSERT_EQ(0, xml_set_attributes(item, "k", "v", "k2", "v2", (char*)0)); /* 0 is success here */
  ASSERT_STR_EQUAL("v", xml_get_attribute(item, "k"));
  ASSERT_STR_EQUAL("v2", xml_get_attribute(item, "k2"));

  added = xml_element("new");
  xml_add_child(root, added);
  xml_child_element_text("c", added, "txt");
  ASSERT_EQ(4, xml_num_children(root));

  np = &root->children;
  while(*np && str_diff((*np)->name, "other"))
    np = &(*np)->next;
  gone = xml_remove(np);
  ASSERT_STR_EQUAL("other", gone->name);
  ASSERT_EQ(3, xml_num_children(root));
  xml_delete(gone);
  xml_free(d);
}

TEST(test_xml_escape) {
  stralloc sa;

  stralloc_init(&sa);
  ASSERT_EQ(22, xml_escape("a<b&\"c>", 7, &sa));
  ASSERT_EQ(1, sa_is(&sa, "a&lt;b&amp;&quot;c&gt;"));
  stralloc_free(&sa);
}

TEST(test_xml_attrnode_textnode) {
  xmlnode* t = xml_textnode("hello");
  xmlnode* a = xml_attrnode("name", 4, "value", 5);
  xmlnode* tb = xml_textnodeb("hello world", 5);

  ASSERT_EQ(XML_TEXT, t->type);
  ASSERT_STR_EQUAL("hello", t->name);
  ASSERT_EQ(XML_ATTRIBUTE, a->type);
  ASSERT_EQ(XML_TEXT, tb->type);
  ASSERT_STR_EQUAL("hello", tb->name); /* only the first 5 bytes */
  xml_delete(t);
  xml_delete(a);
  xml_delete(tb);
}

/*
 * xml_scan_comment, xml_scan_tag, xml_scan_attrname, xml_scan_attrvalue
 */
TEST(test_xml_scan_pieces) {
  size_t tl = 99;

  ASSERT_EQ(11, xml_scan_comment("<!-- hi -->x", 12));
  ASSERT_EQ(0, xml_scan_comment("<p>", 3));
  ASSERT_EQ(6, xml_scan_comment("<!-- x", 6)); /* not closed: all of it */

  /* the token is the name with its '<' or "</", the result includes the '>' */
  ASSERT_EQ(4, xml_scan_tag("<ab>", 4, &tl));
  ASSERT_EQ(3, tl);
  ASSERT_EQ(5, xml_scan_tag("</ab>", 5, &tl));
  ASSERT_EQ(4, tl);
  ASSERT_EQ(4, xml_scan_tag("<ab c=1>", 8, &tl)); /* stops in front of the attributes */
  ASSERT_EQ(3, tl);
  ASSERT_EQ(5, xml_scan_tag("<br/>", 5, &tl));
  ASSERT_EQ(4, tl);
  ASSERT_EQ(2, xml_scan_tag("ab", 2, &tl)); /* never looks beyond len */
  ASSERT_EQ(2, tl);

  ASSERT_EQ(5, xml_scan_attrname("href=\"x\"", 8, &tl)); /* the '=' is consumed, not part of the token */
  ASSERT_EQ(4, tl);
  ASSERT_EQ(1, xml_scan_attrname("a", 1, &tl));
  ASSERT_EQ(1, tl);
  tl = 99;
  ASSERT_EQ(0, xml_scan_attrname("=x", 2, &tl)); /* no name */
  ASSERT_EQ(0, xml_scan_attrname("", 0, &tl));

  ASSERT_EQ(6, xml_scan_attrvalue("\"abc\" d", 7, &tl)); /* quotes in the token, the blank after it skipped */
  ASSERT_EQ(5, tl);
  ASSERT_EQ(6, xml_scan_attrvalue("'a\\'b'", 6, &tl));
  ASSERT_EQ(6, tl); /* an escaped quote does not end it */
  ASSERT_EQ(3, xml_scan_attrvalue("abc>", 4, &tl)); /* unquoted */
  ASSERT_EQ(3, tl);
  ASSERT_EQ(2, xml_scan_attrvalue("x y", 3, &tl));
  ASSERT_EQ(1, tl);
  ASSERT_EQ(0, xml_scan_attrvalue(">", 1, &tl));
  ASSERT_EQ(0, tl);
  ASSERT_EQ(3, xml_scan_attrvalue("\"ab", 3, &tl)); /* not closed: the rest */
  ASSERT_EQ(3, tl);
  ASSERT_EQ(0, xml_scan_attrvalue("", 0, &tl));
  ASSERT_EQ(0, tl);
}

/* a tree to search in: <r><a id="1"/><b><a id="2"/></b><c/></r> */
static xmlnode*
sample_tree(void) {
  static const char text[] = "<r><a id=\"1\"/><b><a id=\"2\"/></b><c/></r>";
  static buffer in;

  buffer_frombuf(&in, text, sizeof(text) - 1);
  return xml_read_tree(&in);
}

/*
 * xml_find_pred_1/2/3/n, xml_find_all_1/2/3, xml_pfind_pred, xml_pfind_all, xml_find_parent_pred_1
 */
TEST(test_xml_find_pred_all) {
  /* the arguments of the find functions are lists separated by '|'; the predicates get them as strlists */
  xml_pred_t* by_name = (xml_pred_t*)(void*)&xml_match_name;
  xml_pred_t* has_attr = (xml_pred_t*)(void*)&xml_has_attr;
  xmlnode *doc = sample_tree(), *n;
  xmlnodeset set;

  ASSERT_EQ(1, doc != NULL);
  n = xml_find_pred_1(doc, by_name, "a");
  ASSERT_EQ(1, n != NULL);
  ASSERT_STR_EQUAL("1", xml_get_attribute(n, "id")); /* the first in document order */
  n = xml_find_pred_1(doc, by_name, "c|b");
  ASSERT_STR_EQUAL("b", n->name); /* either name */
  ASSERT_EQ(1, xml_find_pred_3(doc, by_name, "c", NULL, NULL) != NULL);
  ASSERT_EQ(1, xml_find_pred_n(doc, by_name, 1, "b") != NULL);
  ASSERT_EQ(NULL, xml_find_pred_1(doc, by_name, "zzz"));
  n = xml_find_pred_2(doc, has_attr, NULL, "id");
  ASSERT_EQ(1, n != NULL);
  ASSERT_STR_EQUAL("a", n->name);

  set = xml_find_all_1(doc, by_name, "a");
  ASSERT_EQ(2, xmlnodeset_size(&set));
  ASSERT_STR_EQUAL("1", xml_get_attribute(set.nodes[0], "id"));
  ASSERT_STR_EQUAL("2", xml_get_attribute(set.nodes[1], "id"));
  alloc_free(set.nodes);
  set = xml_find_all_2(doc, has_attr, NULL, "id");
  ASSERT_EQ(2, xmlnodeset_size(&set));
  alloc_free(set.nodes);
  set = xml_find_all_3(doc, by_name, "zzz", NULL, NULL);
  ASSERT_EQ(1, xmlnodeset_empty(&set));
  alloc_free(set.nodes);

  /* from the second a, the parent search finds b */
  set = xml_find_all_1(doc, by_name, "a");
  ASSERT_STR_EQUAL("b", xml_find_parent_pred_1(set.nodes[1], by_name, "b")->name);
  alloc_free(set.nodes);
  xml_free(doc);
}

/*
 * xml_insert, xml_walk, xml_set_attribute, xml_child_element, xml_element_attrs, xml_debug,
 * xml_print_nodeset, xml_print_attributes
 */
static int
count_visit(xmlnode* n, xmlnode* root) {
  static int visited;
  (void)root;
  (void)n;
  return ++visited < 0;
}

static int walked;
static int
walk_count(xmlnode* n, xmlnode* root) {
  (void)n;
  (void)root;
  ++walked;
  return 0;
}

TEST(test_xml_tree_edit) {
  xmlnode *doc = sample_tree(), *root, *e, *kid;
  stralloc sa;
  buffer b;

  stralloc_init(&sa);
  buffer_tosa(&b, &sa);

  root = doc->children;
  ASSERT_STR_EQUAL("r", root->name);

  walked = 0;
  xml_walk(doc, walk_count);
  ASSERT_EQ(1, walked >= 6); /* the document, r, a, b, a, c (and the closing tags if read as nodes) */
  (void)count_visit;

  e = xml_element_attrs("n", "k", "v", 0);
  ASSERT_STR_EQUAL("n", e->name);
  ASSERT_STR_EQUAL("v", xml_get_attribute(e, "k"));
  xml_set_attribute(e, "k", "w");
  ASSERT_STR_EQUAL("w", xml_get_attribute(e, "k"));
  xml_set_attribute(e, "j", "x");
  ASSERT_STR_EQUAL("x", xml_get_attribute(e, "j"));

  kid = xml_child_element("kid", e);
  ASSERT_EQ(e, kid->parent);
  ASSERT_EQ(kid, e->children);

  {
    xmlnode* head = NULL;
    xmlnode* x1 = xml_element("x1");
    xmlnode* x2 = xml_element("x2");

    xml_insert(&head, x1);
    xml_insert(&head, x2); /* inserted in front */
    ASSERT_EQ(x2, head);
    ASSERT_EQ(x1, head->next);
    xml_free(x2); /* frees the chain */
  }

  xml_debug(e, &b);
  buffer_flush(&b);
  ASSERT_EQ(1, sa.len > 0);
  stralloc_nul(&sa);
  ASSERT_EQ(1, str_contains(sa.s, "element[n]"));
  ASSERT_EQ(1, str_contains(sa.s, "element[kid]"));

  stralloc_free(&sa);
  xml_free(e);
  xml_free(doc);
}

TEST(test_xml_print_nodeset_attributes) {
  xmlnode* doc = sample_tree();
  xmlnodeset set = xml_find_all_1(doc, (xml_pred_t*)(void*)&xml_match_name, "a");
  stralloc sa;
  buffer b;

  stralloc_init(&sa);
  buffer_tosa(&b, &sa);
  xml_print_nodeset(&set, &b);
  buffer_flush(&b);
  stralloc_nul(&sa);
  ASSERT_EQ(1, str_contains(sa.s, "xmlnodeset[0]"));
  ASSERT_EQ(1, str_contains(sa.s, "xmlnodeset[1]"));
  ASSERT_EQ(1, str_contains(sa.s, "id=\"2\""));

  sa.len = 0;
  buffer_tosa(&b, &sa);
  xml_print_attributes(set.nodes[0]->attributes, &b, ";", ":", "'");
  buffer_flush(&b);
  stralloc_nul(&sa);
  ASSERT_STR_EQUAL("id:'1'", sa.s);

  alloc_free(set.nodes);
  stralloc_free(&sa);
  xml_free(doc);
}

/*
 * xml_reader, xml_read_callback, xml_scanner, xml_scanner_sa
 */
static int
cb_count(xmlreader* r, xmlnodeid id, stralloc* name, stralloc* value, HMAP_DB** attrs) {
  (void)r;
  (void)name;
  (void)value;
  (void)attrs;
  if(id == XML_ELEMENT)
    ++*(int*)r->data;
  return 1;
}

TEST(test_xml_reader_callback_scanner) {
  static const char text[] = "<r><a/><b>t</b></r>";
  buffer in;
  xmlreader r;
  int elements = 0;
  xmlscanner sc;
  stralloc sa;

  buffer_frombuf(&in, text, sizeof(text) - 1);
  xml_reader(&r, &in);
  r.data = &elements;
  xml_read_callback(&r, cb_count);
  ASSERT_EQ(1, elements >= 3);

  stralloc_init(&sa);
  stralloc_copyb(&sa, text, sizeof(text) - 1);
  xml_scanner_sa(&sc, &sa);
  ASSERT_EQ(1, sc.b != NULL);
  ASSERT_EQ(sa.len, sc.b->n);
  buffer_close(sc.b);
  xml_scanner(&sc, text, sizeof(text) - 1);
  ASSERT_EQ(sizeof(text) - 1, sc.b->n);
  buffer_close(sc.b);
  stralloc_free(&sa);
}

/*
 * xml_find_attr, xml_find_with_attrs(_l), xml_find_all_attrs, xml_find_parent_attr,
 * xml_find_parent_pred_2/3, xml_tag_pred, xml_match_name_and_attr, xml_pfind_pred/all/parent
 */
TEST(test_xml_attr_finders) {
  xmlnode *doc = sample_tree(), *n;
  xmlnodeset set;
  strlist names;
  const void* ptr[4];

  n = xml_find_attr(doc, "id");
  ASSERT_EQ(1, n != NULL);
  ASSERT_STR_EQUAL("a", n->name);
  ASSERT_EQ(NULL, xml_find_attr(doc, "nope"));

  n = xml_find_with_attrs(doc, "zz|id");
  ASSERT_EQ(1, n != NULL);
  ASSERT_STR_EQUAL("1", xml_get_attribute(n, "id"));

  set = xml_find_all_attrs(doc, "id");
  ASSERT_EQ(2, xmlnodeset_size(&set));
  alloc_free(set.nodes);

  /* from the second a: a parent with an id? none; a parent called b: yes */
  set = xml_find_all_attrs(doc, "id");
  n = set.nodes[1];
  ASSERT_EQ(NULL, xml_find_parent_attr(n, "id"));
  ASSERT_STR_EQUAL("b", xml_find_parent_pred_2(n, (xml_pred_t*)(void*)&xml_tag_pred, "b", NULL)->name);
  ASSERT_STR_EQUAL("r", xml_find_parent_pred_3(n, (xml_pred_t*)(void*)&xml_tag_pred, "r", NULL, NULL)->name);
  alloc_free(set.nodes);

  strlist_init(&names, '\0');
  strlist_push(&names, "a");
  n = xml_find_pred_1(doc, (xml_pred_t*)(void*)&xml_match_name, "a");
  ASSERT_EQ(1, xml_match_name_and_attr(n, &names, "id", "1"));
  ASSERT_EQ(0, xml_match_name_and_attr(n, &names, "id", "2"));
  ASSERT_EQ(0, xml_match_name_and_attr(n, &names, "other", "1"));
  strlist_free(&names);

  ptr[0] = "b";
  ptr[1] = ptr[2] = ptr[3] = NULL;
  n = xml_pfind_pred(doc, (xml_pred_t*)(void*)&xml_match_name, ptr);
  ASSERT_STR_EQUAL("b", n->name);
  ptr[0] = "a";
  ptr[1] = ptr[2] = ptr[3] = NULL;
  set = xml_pfind_all(doc, (xml_pred_t*)(void*)&xml_match_name, ptr);
  ASSERT_EQ(2, xmlnodeset_size(&set));
  n = set.nodes[1];
  alloc_free(set.nodes);
  ptr[0] = "r";
  ptr[1] = ptr[2] = ptr[3] = NULL;
  ASSERT_STR_EQUAL("r", xml_pfind_parent(n, (xml_pred_t*)(void*)&xml_match_name, ptr)->name);
  xml_free(doc);
}

#define RUN_XML_TESTS() \
  RUN(test_xml_attr_finders); \
  RUN(test_xml_find_pred_all); \
  RUN(test_xml_tree_edit); \
  RUN(test_xml_print_nodeset_attributes); \
  RUN(test_xml_reader_callback_scanner); \
  RUN(test_xml_scan_pieces); \
  RUN(test_xml_navigate_attributes); \
  RUN(test_xml_find_element_attr_text); \
  RUN(test_xml_modify); \
  RUN(test_xml_escape); \
  RUN(test_xml_attrnode_textnode); \
  RUN(test_xml_build_print); \
  RUN(test_xml_read_tree); \
  RUN(test_xml_read_tree_attributes); \
  RUN(test_xml_read_tree_markup); \
  RUN(test_xml_read_tree_stray_close); \
  RUN(test_xml_read_token)

TESTS(xml) { RUN_XML_TESTS(); }
