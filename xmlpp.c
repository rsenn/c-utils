#include "lib/buffer.h"
#include "lib/hmap.h"
#include "lib/io.h"
#include "lib/iarray.h"
#include "lib/stralloc.h"
#include "lib/xml.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/mmap.h"
#include "lib/str.h"
#include "lib/getopt.h"
#include "lib/errmsg.h"
#include "lib/scan.h"
#include "lib/open.h"
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

static buffer infile, b;
static int depth = 0, prev_closing = 0;
static stralloc prev_element;
static int quote_char = '"';
static int one_line, indent = 2, compact, terminate, inplace;
static stralloc indent_str;
buffer* output;

int
xml_read_function(xmlreader* reader, xmlnodeid id, stralloc* name, stralloc* value, HMAP_DB** attrs) {
  static int newline_written = 0;
  switch(id) {
    case XML_TEXT: {
      if(name) {
        stralloc_trimr(name, " \r\n\t", 4);
        buffer_putsa(output, name);
      }
      break;
    }
    case XML_ELEMENT: {

      /* buffer_puts(buffer_2, "Element: ");
      buffer_putsa(buffer_2, name);
      if(value)
        buffer_putsa(buffer_2, value);
      buffer_putnlflush(buffer_2); */

      int closing = reader->closing || reader->self_closing;

      if(reader->closing) {
        --depth;
      }

      if(!(reader->closing && !prev_closing && stralloc_equal(&prev_element, name)) && stralloc_length(&prev_element)) {

        if(!newline_written && !one_line) {
          buffer_puts(output, "\n");
          buffer_flush(output);

          newline_written = 1;
          buffer_putnspace(output, depth * indent);
        }
      }

      buffer_putm_internal(output, "<", reader->closing ? "/" : "", name->s, NULL);

      if(attrs && *attrs && (*attrs)->list_tuple) {
        buffer_putspace(output);
        xml_print_attributes(*attrs, output, " ", "=", "\"");
      }

      if(terminate && !reader->self_closing) {
        if(str_equal(name->s, "img") || str_equal(name->s, "link") || str_equal(name->s, "br"))
          reader->self_closing = 1;
      }

      buffer_puts(output, reader->self_closing ? (name->s[0] == '?' ? "?>" : " />") : ">");

      stralloc_copy(&prev_element, name);
      prev_closing = closing;

      if(!reader->closing && !reader->self_closing)
        ++depth;

      newline_written = 0;

      break;
    }
    default: break;
  }
  return 1;
}

void
usage(char* av0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(av0),
                       " [OPTIONS] [FILES...]\n"
                       "\n"
                       "Options:\n"
                       "\n"
                       "  -h, --help              Show this help\n"
                       "  -s, --single-quote      Use ' as quote\n"
                       "  -d, --double-quote      Use \" as quote\n"
                       "  -o, --one-line          One-line\n"
                       "  -c, --compact           Compact\n"
                       "  -l, --indent NUM        Indent level\n"
                       "  -t, --terminate         Terminate non-closed tags (img, link, br, ...)\n"
                       "\n",
                       0);
  buffer_flush(buffer_1);
}
static int max_depth;
static int
xmlpp_get_depth(xmlnode* node, int d) {
  xmlnode* n;
  if(d == 0)
    max_depth = 0;
  else if(d > max_depth)
    max_depth = d;

  for(n = node->children; n; n = n->next) xmlpp_get_depth(n, d + 1);

  return max_depth;
}

static void
xmlpp_fmt(xmlnode* node, buffer* b, int depth, char ch, int n) {
  static xmlnode* parent;

  xmlnode* p;

  int inner = depth < 0;

  depth = depth < 0 ? -depth : depth;
  if(one_line)
    return;
  if(compact) {
    if(inner && xmlpp_get_depth(node, 0) <= 1 && xml_num_children(node) <= 1) {
      parent = node;
      return;
    }

    for(p = node; p; p = p->parent) {
      if(p == parent)
        return;
    }
  }

  while(n-- > 0) buffer_putc(b, ch);

  if(ch == '\n')
    buffer_flush(buffer_1);

  /*   switch(ch) {
      case '\n':
      break;
      case ' ':
      break;
    } */
}

int
main(int argc, char* argv[]) {
  buffer outfile;
  xmlreader r;
  xmlnode* doc;
  int ret;
  int c;
  int index = 0;
  const char* input_file;
  const char* tmpl = 0;
  struct longopt opts[] = {{"help", 0, NULL, 'h'}, {"single-quote", 0, &quote_char, '\''}, {"double-quote", 0, &quote_char, '"'}, {"one-line", 0, NULL, 'o'}, {"compact", 0, NULL, 'c'}, {"indent", 0, NULL, 'l'}, {"inplace", 0, NULL, 'i'}, {"terminate", 0, NULL, 't'}, {0, 0, 0, 0}};

  errmsg_iam(argv[0]);
  output = buffer_1;

  for(;;) {
    c = getopt_long(argc, argv, "hsdol:cti", opts, &index);
    if(c == -1)
      break;
    if(c == 0)
      continue;

    switch(c) {
      case 'h': usage(argv[0]); return 0;
      case 's': quote_char = '\''; break;
      case 'd': quote_char = '"'; break;
      case 'o': one_line = 1; break;
      case 'c': compact = 1; break;
      case 't': terminate = 1; break;
      case 'i': inplace = 1; break;
      case 'l': scan_int(optarg, &indent); break;
      default: usage(argv[0]); return 1;
    }
  }

  if(optind < argc)
    ret = buffer_mmapprivate(&infile, (input_file = argv[optind]));
  else
    ret = buffer_read_fd(&infile, 0);

  if(inplace) {
    fd_t out_fd = open_temp(&tmpl);
    if(out_fd == -1) {
      errmsg_warn("output file", 0);
      return 1;
    }
    buffer_write_fd(&outfile, out_fd);
    output = &outfile;
  }

  if(ret) {
    errmsg_infosys("input");
    return errno == ENOENT ? 127 : 1;
  }

  // xml_reader_init(&r, &infile);
  doc = xml_read_tree(&infile);

  xml_print(doc, output, xmlpp_fmt);
  // xml_read_callback(&r, xml_read_function);
  buffer_close(&infile);

  buffer_putnlflush(output);
  buffer_close(output);

  if(inplace) {
    if(unlink(input_file) == -1) {
      errmsg_infosys("unlink");
      return 1;
    }
    if(rename(tmpl, input_file) == -1) {
      errmsg_infosys("rename");
      return 1;
    }
  }

  return 0;
}
