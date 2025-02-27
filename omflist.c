/* -*- Mode: C; indent-tabs-mode: nil;
 * c-basic-offset: 4 -*- */

#include "lib/buffer.h"
#include "lib/unix.h"
#include "lib/str.h"
#include "lib/errmsg.h"
#include "lib/omf.h"
#include "lib/fmt.h"
#include "lib/mmap.h"
#include "lib/byte.h"
#include "lib/stralloc.h"

#include <ctype.h>

void
omf_dump(buffer* b, omf_record* r) {
  uint16 len = uint16_get(&r->length);

  buffer_puts(b, "omf record: type=");
  buffer_putxlong0(b, r->type, 2);
  buffer_puts(b, ", length=");
  buffer_putulong(b, len);
  buffer_putnlflush(b);
}

void
dump_hex(buffer* b, const char* x, const char* y, size_t offset) {
  size_t n = y - x;
  size_t i;
  stralloc linesa;
  stralloc_init(&linesa);

  for(i = 0; i < n; ++i) {
    size_t li = i % 16;

    if(li == 0) {
      if(linesa.len) {
        buffer_puts(b, "  |");
        buffer_putsa(b, &linesa);
        buffer_putnspace(b, 16 - linesa.len);
        buffer_putc(b, '|');
        stralloc_zero(&linesa);
      }
      buffer_putc(b, '\n');
      buffer_putxlong0(b, i + offset, 8);
      buffer_putspace(b);
    }
    buffer_putspace(b);
    buffer_putxlong0(b, (unsigned long)(unsigned char)x[i], 2);
    stralloc_catc(&linesa, isprint(x[i]) ? x[i] : ' ');
  }

  while(i % 16) {
    buffer_putnspace(b, 3);
    ++i;
  }

  if(linesa.len) {
    buffer_puts(b, "  |");
    buffer_putsa(b, &linesa);
    buffer_putc(b, '|');
  }

  stralloc_free(&linesa);
  buffer_putnlflush(b);
}

void
usage(char* av0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(av0),
                       " [OPTIONS] <file...>\n",
                       "\n",
                       "Options:\n",
                       "\n",
                       "  -h, --help              Show                        his help\n",
                       "  -i, --imports           List                        mports\n",
                       "  -e, --exports           List                        xports\n",
                       "\n",
                       NULL);
  buffer_flush(buffer_1);
}

int
main(int argc, char** argv) {
  uint8* base = NULL;
  size_t filesize;

  int c, index = 0;

  struct unix_longopt opts[] = {{"help", 0, NULL, 'h'}, {0, 0, 0, 0}};
  errmsg_iam(argv[0]);
  buffer_puts(buffer_1, "Number of arguments: ");
  buffer_putlong(buffer_1, argc);
  buffer_putnlflush(buffer_1);

  for(;;) {
    c = unix_getopt_long(argc, argv, "hiedsEDO", opts, &index);

    if(c == -1)
      break;

    if(c == '\0')
      continue;

    switch(c) {
      case 'h': usage(argv[0]); return 0;
      default: {
        usage(argv[0]);
        return 1;
      }
    }
  }

  for(; argv[unix_optind]; ++unix_optind) {
    omf_file omf;

    if(omf_open(&omf, argv[unix_optind])) {
      omf_record* r;
      char* p;

      r = omf_begin(&omf);

      omf_dump(buffer_1, r);

      omf_foreach(&omf, r) {
        range x = omf_data(r);
        const char* name = omf_name(r);

        if((p = omf_record_begin(r))) {
          omf_record_foreach(r, p) {

            //  //|| r->type == 0x96
            buffer_putptr(buffer_1, p);

            if((r->type & 0xfe) == 0x90 || r->type == 0x96 || r->type == 0x8c) {
              buffer_putspace(buffer_1);
              buffer_puts(buffer_1, omf_name(r));
              buffer_putspace(buffer_1);
              buffer_put(buffer_1, omf_str_begin(p), omf_str_len(p));
            }

            buffer_putnlflush(buffer_1);
          }

          continue;
        }

#ifdef DEBUG
        buffer_putm_internal(buffer_2, name ? name : "<null>", " (0x", NULL);
        buffer_putxlong0(buffer_2, r->type, 2);
        buffer_puts(buffer_2, ") : ");

        buffer_putulong(buffer_2, x.end - x.start);
        buffer_puts(buffer_2, " bytes");

        if(r->type != OMF_COMENT)
          dump_hex(buffer_2, x.start - 3, x.end, x.start - (char*)omf.map);

        buffer_putnlflush(buffer_2);
#endif
      }

      omf_close(&omf);
    } else {
      errmsg_warnsys("ERROR opening '", argv[unix_optind], "': ");
      return 127;
    }
  }

  return 0;
}
