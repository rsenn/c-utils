#include "lib/buffer.h"
#include "lib/hmap.h"
#include "lib/io.h"
#include "lib/iarray.h"
#include "lib/stralloc.h"
#include "lib/strlist.h"
#include "lib/xml.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/mmap.h"
#include "lib/str.h"
#include "lib/getopt.h"
#include "lib/errmsg.h"
#include "lib/scan.h"
#include <ctype.h>
#include <errno.h>

static buffer infile, b;
static int depth = 0, prev_closing = 0;
static stralloc prev_element;
static int quote_char = '"';
static int one_line, indent = 2, compact;
static stralloc indent_str;

char*
reformat_style(char* s) {
  size_t n;
  strlist style;
  stralloc key, value, out;
  stralloc_init(&key);
  stralloc_init(&value);
  stralloc_init(&out);
  strlist_init(&style, ';');
  stralloc_copys(&style.sa, s);

  strlist_foreach(&style, s, n) {
     size_t pos = byte_chr(s, ':', n);
     stralloc_copyb(&key, s, pos);

     while(s[pos]) {
      if(s[pos] == ':' || s[pos] == ' ')
        pos++;
     }
     stralloc_copyb(&value, &s[pos], n - pos);

     if(out.len > 0)
      stralloc_cats(&out, ", ");
     stralloc_camelize(&key);
     stralloc_copy(&out, &key);
     stralloc_cats(&out, ": '");
     stralloc_cat(&out, &value);
     stralloc_cats(&out, "'");
  }
  stralloc_free(&key);
  stralloc_free(&value);
  strlist_free(&style);
  return out.s;
}

int
xml_read_function(xmlreader* reader, xmlnodeid id, stralloc* name, stralloc* value, HMAP_DB** attrs) {
  switch(id) {
    case XML_TEXT: {
      buffer_putsa(buffer_1, value);
      break;
    }
    case XML_ELEMENT: {
      int closing = reader->closing || reader->self_closing;

      if(reader->closing)
        --depth;

      if(!(reader->closing && !prev_closing && stralloc_equal(&prev_element, name)) && stralloc_length(&prev_element)) {
        buffer_puts(buffer_1, "\n");
        buffer_putnspace(buffer_1, depth * 2);
      }

      buffer_putm_3(buffer_1, "<", reader->closing ? "/" : "", name->s);

      if(attrs && *attrs && (*attrs)->list_tuple) {
        char* a = hmap_get(*attrs, "style", 5);

        if(a) {
          a = reformat_style(a);
          hmap_set(attrs, "style", 5, a, str_len(a) + 1);
        }



        buffer_putspace(buffer_1);
        xml_print_attributes(*attrs, buffer_1, " ", "=", "\"");
      }

      buffer_puts(buffer_1, reader->self_closing ? (name->s[0] == '?' ? "?>" : "/>") : ">");

      stralloc_copy(&prev_element, name);
      prev_closing = closing;

      if(!reader->closing && !reader->self_closing)
        ++depth;
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
                       "\n",
                       0);
  buffer_flush(buffer_1);
}

int
main(int argc, char* argv[]) {
  xmlreader r;
  int ret;
  int c;
  int index = 0;
  struct longopt opts[] = {
      {"help", 0, NULL, 'h'},
      {"single-quote", 0, &quote_char, '\''},
      {"double-quote", 0, &quote_char, '"'},
      {"one-line", 0, NULL, 'o'},
      {"compact", 0, NULL, 'c'},
      {"indent", 0, NULL, 'l'},
      {0},
  };

  errmsg_iam(argv[0]);

  for(;;) {
    c = getopt_long(argc, argv, "hsdol:c", opts, &index);
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
      case 'l': scan_int(optarg, &indent); break;
      default: usage(argv[0]); return 1;
    }
  }

  if(argc > 1)
    ret = buffer_mmapprivate(&infile, argv[1]);
  else
    ret = buffer_read_fd(&infile, 0);

  if(ret) {
    errmsg_infosys("input");
    return errno == ENOENT ? 127 : 1;
  }

  xml_reader_init(&r, &infile);
  xml_read_callback(&r, xml_read_function);
  buffer_putnlflush(buffer_1);
  buffer_close(&b);

  return 0;
}
