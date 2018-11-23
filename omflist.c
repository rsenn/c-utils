/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "lib/buffer.h"
#include "lib/unix.h"
#include "lib/str.h"
#include "lib/errmsg.h"
#include "lib/omf.h"
#include "lib/fmt.h"
#include "lib/mmap.h"
#include "lib/byte.h"

void
usage(char* av0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(av0),
                       " [OPTIONS] <file...>\n",
                       "\n",
                       "Options:\n",
                       "\n",
                       "  -h, --help              Show this help\n",
                       "  -i, --imports           List imports\n",
                       "  -e, --exports           List exports\n",
                       "\n",
                       0);
  buffer_flush(buffer_1);
}

int
main(int argc, char** argv) {
  uint8* base = NULL;
  size_t filesize;

  int c, index = 0;

  struct longopt opts[] = {{"help", 0, NULL, 'h'}, {0}};
  errmsg_iam(argv[0]);
  buffer_puts(buffer_1, "Number of arguments: ");
  buffer_putlong(buffer_1, argc);
  buffer_putnlflush(buffer_1);

  for(;;) {
    c = getopt_long(argc, argv, "hiedsEDO", opts, &index);
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

  for(; argv[optind]; ++optind) {
    omf_file omf;

    if(omf_open(&omf, argv[optind])) {

      omf_record* r;

      omf_foreach(&omf, r) {
        range x = omf_data(r);
        const char* name = omf_name(r);
        buffer_putm_internal(buffer_1, name ? name : "<null>", " (0x", 0);
        buffer_putxlong0(buffer_1, r->type, 2);
        buffer_puts(buffer_1, ") : ");
        buffer_putulong(buffer_1, x.elem_size);
        buffer_putsflush(buffer_1, " bytes\n");
      }

      omf_close(&omf);

    } else {
      errmsg_warnsys("ERROR opening '", argv[optind], "': ");
      return 127;
    }
  }

  return 0;
}
