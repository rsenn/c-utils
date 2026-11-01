#include "unit_test.h"
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

#define RUN_XML_TESTS() \
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
