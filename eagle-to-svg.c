#include "lib/scan.h"
#include "lib/scan.h"
#include "lib/open.h"
#include "lib/buffer.h"
#include "lib/str.h"
#include "lib/strlist.h"
#include "lib/xml.h"

#include "map.h"

static const char* palette[16] = {"#000000", "#0000aa", "#555555", "#5555ff", "#aa0000", "#00aa00", "#55ff55", "#aaaaaa", "#00aaaa", "#55ffff", "#ff5555", "#aa00aa", "#ff55ff", "#aa5500", "#ffff55", "#aaaaaa"};

static buffer input, output;
static xmlnodeset nodeset;
static MAP_T(layers);
static MAP_T(colors);

typedef struct rect {
  double x1, y1, x2, y2;
} rect_t;

rect_t
get_rect(xmlnode* node) {
  rect_t r;
  r.x1 = xml_get_attribute_double(node, "x1");
  r.y1 = xml_get_attribute_double(node, "y1");
  r.x2 = xml_get_attribute_double(node, "x2");
  r.y2 = xml_get_attribute_double(node, "y2");
  return r;
}

void
print_rect(const rect_t* rect, buffer* b) {
  buffer_putdouble(b, rect->x1, 3);
  buffer_putc(b, ',');
  buffer_putdouble(b, rect->y1, 3);
  buffer_putc(b, '|');
  buffer_putdouble(b, rect->x2, 3);
  buffer_putc(b, ',');
  buffer_putdouble(b, rect->y2, 3);
  buffer_putnlflush(b);
}

void
node_set_num(xmlnode* node, const char* attr, double num, const char* unit) {
  stralloc sa;
  stralloc_init(&sa);
  stralloc_zero(&sa);
  stralloc_catlong(&sa, num);
  stralloc_cats(&sa, unit);
  stralloc_nul(&sa);
  xml_set_attribute(node, attr, sa.s);
  stralloc_free(&sa);
}

xmlnode*
make_line(const rect_t* rect) {
  xmlnode* line = xml_element("line");
  const char* unit = "";
  node_set_num(line, "x1", rect->x1 / 2.54, unit);
  node_set_num(line, "y1", rect->y1 / 2.54, unit);
  node_set_num(line, "x2", rect->x2 / 2.54, unit);
  node_set_num(line, "y2", rect->y2 / 2.54, unit);
  return line;
}

const char*
get_color(xmlnode* node) {
  const char* layer = xml_get_attribute(node, "layer");
  const char* color = MAP_GET(colors, layer, str_len(layer) + 1);
  unsigned int c = 0;
  scan_uint(color, &c);
  return palette[c];
}

xmlnode*
pad(double x, double y) {
  xmlnode* node = xml_element("path");

  /*   <path fill="green"
   * fill-rule="evenodd" stroke="green"
   *    stroke-linecap="round"
   * stroke-linejoin="round"
   * stroke-miterlimit="10"
   *     stroke-width=".265"
   *     d="M1.27.55a.72.72 0
   * 100 1.44.72.72 0 000-1.44zm0
   * .167c.324 0 .587.248.587.553 0
   * .306-.263.553-.587.553-.325
   * 0-.587-.247-.587-.553
   * 0-.305.262-.553.587-.553z"
   *   />
   */

  stralloc sa;
  stralloc_init(&sa);
  stralloc_cats(&sa, "translate(");
  stralloc_catdouble(&sa, x / 2.54, 3);
  stralloc_cats(&sa, ", ");
  stralloc_catdouble(&sa, y / 2.54 - 0.25 - 1, 3);
  stralloc_cats(&sa, ") scale(0.5 0.5) ");
  stralloc_nul(&sa);

  xml_set_attribute(node, "fill", "green");
  xml_set_attribute(node, "fill-rule", "evenodd");
  xml_set_attribute(node, "stroke", "green");
  xml_set_attribute(node, "stroke-linecap", "round");
  xml_set_attribute(node, "stroke-linejoin", "round");
  xml_set_attribute(node, "stroke-miterlimit", "10");
  xml_set_attribute(node, "stroke-width", ".265");
  xml_set_attribute(node,
                    "d",
                    "M0 2.004a.536.536 0 100 "
                    "1.072.536.536 0 000-1.072zm0 "
                    ".124c.242 0 .438.185.438.412 0 "
                    ".228-.196.412-.438.412s-.438-."
                    "184-.438-.412c0-.227.196-.412."
                    "438-.412z");
  xml_set_attribute(node, "transform", sa.s);
  stralloc_free(&sa);
  return node;
}

int
main(int argc, char* argv[]) {
  xmlnode* doc;
  xmlnode** nptr;
  xmlnode* signals;
  xmlnode *svgdoc, *svgelem, *svggroup, *svgdefs;
  MAP_PAIR_T t;
  int output_fd = 1;
  int argi = 1;
  const char* input_file = "/home/roman/Dokumente/Sources/"
                           "pictest/eagle/"
                           "PIC18F2550-USB+ICSP-Board.brd";
  const char* output_file = NULL;
  if(argv[argi])
    input_file = argv[argi++];
  if(argv[argi])
    output_file = argv[argi++];

  if(output_file)
    output_fd = open_trunc(output_file);

  buffer_write_fd(&output, output_fd);
  buffer_mmapprivate(&input, input_file);
  buffer_skip_until(&input, "\r\n", 2);

  doc = xml_read_tree(&input);

  nodeset = xml_find_all_1(doc, (xml_pred_t*)&xml_match_name, "layer");

  MAP_NEW(layers);
  MAP_NEW(colors);

  xmlnodeset_foreach(&nodeset, nptr) {
    const char* name = xml_get_attribute(*nptr, "name");
    const char* number = xml_get_attribute(*nptr, "number");
    const char* color = xml_get_attribute(*nptr, "color");

    MAP_INSERT(layers, number, str_len(number) + 1, (char*)name, str_len(name) + 1);
    MAP_INSERT(colors, number, str_len(number) + 1, (char*)color, str_len(color) + 1);
  }

  MAP_FOREACH(layers, t) {
    const char* color = MAP_GET(colors, MAP_KEY(t), str_len(MAP_KEY(t)));
    unsigned int c = 0;
    scan_uint(color, &c);

    buffer_put(buffer_2, MAP_KEY(t), str_len(MAP_KEY(t)));
    buffer_puts(buffer_2, ": ");
    buffer_put(buffer_2, MAP_VALUE(t), str_len(MAP_VALUE(t)));
    buffer_puts(buffer_2, ": ");
    buffer_puts(buffer_2, color);
    buffer_puts(buffer_2, ": ");
    buffer_puts(buffer_2, palette[c]);
    buffer_putnlflush(buffer_2);
  }

  signals = xml_find_element(doc, "signals");

  svgdoc = xml_newnode(XML_DOCUMENT);

  svgelem = xml_element("svg");
  xml_set_attribute(svgelem, "width", "210mm");
  xml_set_attribute(svgelem, "height", "297mm");
  xml_set_attribute(svgelem, "xmlns", "http://www.w3.org/2000/svg");
  xml_set_attribute(svgelem, "viewBox", "0 0 21 29.7");
  xml_add_child(svgdoc, svgelem);
  xml_add_child(svgelem, (svgdefs = xml_element("defs")));
  xml_add_child(svgelem, (svggroup = xml_element("g")));
  {
    xmlnode *clip_p, *clip_path = xml_element("clipPath");
    xml_set_attribute(clip_path, "id", "pad-clip");
    xml_add_child(svgdefs, clip_path);
    clip_p = xml_element("path");
    xml_set_attribute(clip_p,
                      "d",
                      "M0 2.128c.242 0 "
                      ".438.185.438.412 0 "
                      ".228-.196.412-.438.412s-.438-."
                      "184-.438-.412c0-.227.196-.412."
                      "438-.412z");
    xml_add_child(clip_path, clip_p);
  }
  xml_set_attribute(svggroup, "stroke-width", "0.3");
  xml_set_attribute(svggroup, "stroke-linecap", "round");
  xml_set_attribute(svggroup, "fill", "none");

  nodeset = xml_find_all_1(signals, (xml_pred_t*)&xml_match_name, "wire");

  xmlnodeset_foreach(&nodeset, nptr) {
    rect_t rect = get_rect(*nptr);
    //   print_rect(&rect, buffer_2);

    double width = xml_get_attribute_double(*nptr, "width") / 2.54;
    const char* layer = xml_get_attribute(*nptr, "layer");

    xmlnode* line = make_line(&rect);
    xml_set_attribute(line, "stroke", get_color(*nptr));
    if(width > 0)
      xml_set_attribute_double(line, "stroke-width", width, 3);

    layer = MAP_GET(layers, layer, str_len(layer) + 1);
    xml_set_attribute(line, "class", layer);
    xml_add_child(svggroup, line);
    xml_add_child(svggroup, pad(rect.x1 > rect.x2 ? rect.x2 : rect.x1, rect.y1 > rect.y2 ? rect.y2 : rect.y1));
  }
  // nodeset = xml_find_all_1(signals,
  // xml_match_name, "wire");

  xml_print(svgdoc, &output, 0);
  /*
   * Cleanup function for the XML
   * library.
   */
  xml_free(doc);
  buffer_flush(&output);
  /*
   * this is to debug memory for
   * regression tests
   */
  return (0);
}
