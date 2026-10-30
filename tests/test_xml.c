#include "unit_test.h"
#include "../lib/strlist.h"
#include "../lib/xml.h"
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

#define RUN_XML_TESTS() \
  RUN(test_xml_build_print); \
  RUN(test_xml_read_tree); \
  RUN(test_xml_read_tree_attributes); \
  RUN(test_xml_read_tree_markup); \
  RUN(test_xml_read_tree_stray_close); \
  RUN(test_xml_read_token)

TESTS(xml) { RUN_XML_TESTS(); }
