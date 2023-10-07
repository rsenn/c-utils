#include "lib/uint8.h"
#include "lib/uint16.h"
#include "lib/uint32.h"
#include "lib/slist.h"
#include "lib/mmap.h"
#include "lib/alloc.h"
#include "lib/byte.h"
#include "lib/str.h"
#include "lib/scan.h"
#include "lib/ihex.h"
#include "lib/stralloc.h"
#include "lib/buffer.h"
#define MAP_USE_HMAP 1
#include "lib/map.h"
#include "lib/strlist.h"
#include "lib/getopt.h"
#include "lib/errmsg.h"
#include "lib/dir.h"
#include "lib/path.h"
#include <assert.h>
#include <ctype.h>

typedef struct cvalue {
  struct cvalue* next;
  uint16 value;
  int is_default;
  const char* name;
  const char* description;
} cvalue;

typedef struct csetting {
  struct csetting* next;
  uint16 mask;
  const char* name;
  const char* description;
  struct cvalue* values;
} csetting;

typedef struct cword {
  struct cword* next;
  uint32 address;
  uint16 mask;
  uint16 default_value;
  const char* name;
  csetting* settings;
} cword;

static cword* words;
static ihex_file hex;
static uint32 baseaddr;
static stralloc cfg;
// static bmap_t(const char*) pragmas;
static strlist pragmas;
static int nodefault = 0, oneline = 0, comments = 1, output_name = 0, verbose = 0;

uint16
config_data_at(uint32 addr) {
  if(baseaddr == 0x0000400e) {
    size_t offs = addr - 0x2007;
    assert(offs < cfg.len);
    return uint16_read(&cfg.s[offs]);
  } else {
    size_t offs = (addr & 0x0fff);
    return cfg.s[offs];
  }
}

void
dump_cword(buffer* b, cword* word) {
  buffer_puts(b, word->name ? word->name : "(null)");
  buffer_puts(b, " @ 0x");
  buffer_putxlong0(b, word->address, 4);
  buffer_puts(b, "\tmask 0x");
  buffer_putxlong0(b, word->mask, 2);
  buffer_puts(b, "\tdefault 0x");
  buffer_putxlong0(b, word->default_value, 2);
  buffer_putnlflush(b);
}

void
dump_csetting(buffer* b, csetting* setting) {
  buffer_puts(b, "  ");
  buffer_puts(b, setting->name ? setting->name : "(null)");
  buffer_puts(b, "\tmask 0x");
  buffer_putxlong0(b, setting->mask, 2);
  buffer_puts(b, "\t\t");
  buffer_puts(b, setting->description);
  buffer_putnlflush(b);
}
void
dump_cvalue(buffer* b, cvalue* value) {
  buffer_puts(b, "    ");
  buffer_puts(b, value->name ? value->name : "(null)");
  buffer_puts(b, "\tvalue 0x");
  buffer_putxlong0(b, value->value, 2);
  buffer_puts(b, "\t\t");
  buffer_puts(b, value->description);
  buffer_putnlflush(b);
}

cvalue**
cfg_value(cvalue** vptr, cword* w, csetting* s, const char* x, size_t n) {
  cvalue* v = *vptr = alloc(sizeof(cvalue));
  size_t i;
  unsigned long value;
  i = scan_xlongn(x, n, &value);
  assert(i);
  v->is_default = (w->default_value & s->mask) == value;
  v->value = value;
  x += i + 1;
  n -= i + 1;
  i = byte_chr(x, n, ':');
  i = byte_chr(x, i, ',');
  v->name = str_ndup(x, i);
  assert(i < n);
  x += i + 1;
  n -= i + 1;
  v->description = str_ndup(x, n);
  v->next = NULL;
  return &v->next;
}

csetting**
cfg_setting(csetting** sptr, cword* w, const char* x, size_t n) {
  csetting* s = *sptr = alloc(sizeof(csetting));
  size_t i;
  unsigned long value;
  i = scan_xlongn(x, n, &value);
  assert(i);
  s->mask = value;
  x += i + 1;
  n -= i + 1;
  i = byte_chr(x, n, ':');
  s->name = str_ndup(x, i);
  assert(i < n);
  x += i + 1;
  n -= i + 1;
  s->description = str_ndup(x, n);
  s->next = NULL;
  s->values = NULL;
  return &s->next;
}

cword**
cfg_word(cword** wptr, const char* x, size_t n) {
  cword* w = *wptr = alloc(sizeof(cword));
  size_t i;
  unsigned long value;
  i = scan_xlongn(x, n, &value);
  assert(i);
  w->address = value;
  x += i + 1;
  n -= i + 1;
  i = scan_xlongn(x, n, &value);
  assert(i);
  w->mask = value;
  x += i + 1;
  n -= i + 1;
  i = scan_xlongn(x, n, &value);
  assert(i);
  w->default_value = value;
  x += i + 1;
  n -= i + 1;
  w->name = str_ndup(x, n);
  w->next = NULL;
  w->settings = NULL;
  return &w->next;
}

int
cfg_data(cword** wptr, const char* x, size_t n) {
  size_t eol, col;
  cword* w = 0;
  csetting *s = 0, **sptr = NULL;
  cvalue *v = 0, **vptr = NULL;

  while(n > 0) {

    eol = byte_chr(x, n, '\n');

    if(eol > 0 && x[0] == 'C') {
      const char* line = x;
      col = byte_chr(x, n, ':') + 1;
      assert(col < eol);
      x += col;
      n -= col;
      eol -= col;

      if(!str_diffn(line, "CWORD", 5)) {
        cword** nwptr = cfg_word(wptr, x, eol);
        w = *wptr;
        sptr = &w->settings;
        wptr = nwptr;
      } else if(!str_diffn(line, "CSETTING", 8)) {
        csetting** nsptr = cfg_setting(sptr, w, x, eol);
        s = *sptr;
        vptr = &s->values;
        sptr = nsptr;
      } else if(!str_diffn(line, "CVALUE", 6)) {
        cvalue** nvptr = cfg_value(vptr, w, s, x, eol);
        v = *vptr;
        vptr = nvptr;
      }
    }

    if(eol < n)
      eol++;

    x += eol;
    n -= eol;
  }
  return 0;
}

size_t
config_bytes(ihex_file* ihf, stralloc* sa, uint32* addr) {
  size_t bytes;
  stralloc_zero(sa);
  stralloc_ready(sa, 14);

  if(((bytes = ihex_read_at(&hex, 0x00300000, sa->s, 14)) == 14)) {
    *addr = 0x00300000;
  } else {
    if((bytes = ihex_read_at(&hex, 0x400e, sa->s, 2)) == 2)
      *addr = 0x400e;
  }

  sa->len = bytes;
  return bytes;
}

uint16
get_setting_word(cword* word, csetting* setting) {
  uint16 value = config_data_at(word->address);

  value &= setting->mask;

  return value;
}

cvalue*
get_setting_value(cword* word, csetting* setting) {
  cvalue* value;
  uint16 byteval = get_setting_word(word, setting);

#ifdef DEBUG_OUTPUT
  buffer_putm_internal(buffer_2, word->name, ": ", setting->name, " = 0x", NULL);
  buffer_putxlong0(buffer_2, byteval, 2);
  buffer_putnlflush(buffer_2);
#endif

  for(value = setting->values; value; value = value->next) {
    if(verbose) {
      buffer_putm_internal(buffer_2, "  0x", NULL);
      buffer_putxlong0(buffer_2, value->value, 2);

      buffer_putm_internal(buffer_2, ": ", value->name, NULL);
      buffer_putnlflush(buffer_2);
    }

    if(value->value == byteval)
      return value;
  }

  return NULL;
}

csetting*
find_setting(const char* str) {
  cword* word;
  csetting* setting;

  slink_foreach(words, word) for(setting = word->settings; setting; setting = setting->next) if(!str_diffn(str, setting->name, str_len(setting->name))) return setting;

  return NULL;
}

cvalue*
find_value(const char* str) {
  csetting* setting;

  if((setting = find_setting(str))) {
    cvalue* value;
    str += str_chr(str, '=');

    while(*str == '=' || *str == ' ')
      ++str;

    slink_foreach(setting->values, value) if(!str_diffn(str, value->name, str_len(value->name))) return value;
  }
  return NULL;
}

const char*
infer_chip(const char* x, size_t n) {
  static stralloc chip;
  size_t i;

  for(i = 0; i + 3 < n; i++) {
    size_t len = n - i;
    const char* s = &x[i];

    if(*s == '1') {
      char c2 = tolower(s[2]);
      if(c2 == 'l' || c2 == 'f') {
        char c1 = s[1];
        if(c1 == '2' || c1 == '6' || c1 == '8') {
          len = scan_charsetnskip(s,
                                  "0123456789aAbBcCeEfFgGhH"
                                  "iIjJkKlLmMnNpPrRtTvV",
                                  len);

          if(len > 3) {
            stralloc_copyb(&chip, s, len);
            stralloc_lower(&chip);
            stralloc_nul(&chip);
            return chip.s;
          }
        }
      }
    }
  }
  return 0;
}

const char*
get_cfgdat(const char* chip) {
  size_t i;
  static stralloc path;
  if(path.len == 0) {
    dir_t d;
    const char *dir = 0, *subdir;
    static const char* const search_dirs[] = {"/opt/microchip",
                                              "C:\\Program Files\\Microchip",
                                              "C:\\Program Files (x86)\\Microchip"};
    for(i = 0; i < sizeof(search_dirs) / sizeof(search_dirs[0]); i++) {
      dir = search_dirs[i];
      if(path_exists(dir))
        break;
    }
    if(dir == NULL)
      return NULL;
    stralloc_copys(&path, dir);
    stralloc_cats(&path, "/xc8/");
    stralloc_nul(&path);
    dir_open(&d, path.s);

    while((subdir = dir_read(&d))) {
      if(subdir[0] == '.' || dir_type(&d) != D_DIRECTORY)
        continue;
      dir = subdir;
      buffer_putm_internal(buffer_2, "subdir = ", subdir, NULL);
      buffer_putnlflush(buffer_2);
      break;
    }
    stralloc_cats(&path, dir);
  }

  path.len = stralloc_finds(&path, "/dat/");
  stralloc_cats(&path, "/dat/cfgdata/");
  stralloc_cats(&path, chip);
  stralloc_cats(&path, ".cfgdata");
  stralloc_nul(&path);

  if(path_exists(path.s)) {
    buffer_putm_internal(buffer_2, "Found cfgdata: ", path.s, NULL);
    buffer_putnlflush(buffer_2);
    return path.s;
  }
  return 0;
}

void
add_item(strlist* list, const char* name, const char* value) {

  stralloc out;
  stralloc_init(&out);

  if(value)
    stralloc_catm_internal(&out, name, " = ", value, NULL);
  else
    stralloc_catm_internal(&out, "// ", name, NULL);

  strlist_push_sa(list, &out);

  stralloc_free(&out);
}

void
add_comment(strlist* list, const char* text) {
  stralloc out;
  stralloc_init(&out);
  stralloc_catm_internal(&out, "/* ", text, " */", NULL);
  strlist_push_sa(list, &out);
  stralloc_free(&out);
}

void
process_config(void (*pragma)(strlist*, const char* key, const char* value), void (*comment)(strlist*, const char*), strlist* list) {
  cword *prevword = 0, *word;
  csetting* setting;
  cvalue* value;

  slink_foreach(words, word) {
    if(!str_diffn(word->name, "IDLOC", 5))
      break;

    if(verbose)
      dump_cword(buffer_2, word);

    comment(list, word->name);

    for(setting = word->settings; setting; setting = setting->next) {

      value = get_setting_value(word, setting);

      if(value == NULL) {
        buffer_puts(buffer_2, "WARNING:  value ");
        buffer_putxlong(buffer_2, get_setting_word(word, setting));
        buffer_putm_internal(buffer_2, " for setting ", setting->name, " not found!", NULL);
        buffer_putnlflush(buffer_2);
        continue;
      }

      if(value->is_default && nodefault) {
#ifdef DEBUG_OUTPUT
        if(verbose > 1) {
          buffer_putm_internal(buffer_2, "skip default value ", value->name, " for setting ", setting->name, NULL);
          buffer_putnlflush(buffer_2);
        }
#endif
        continue;
      }

      if(output_name && prevword != word)
        pragma(list, word->name, NULL);

      pragma(list, setting->name, value->name);
      prevword = word;
    }
  }
}

void
output_items(const strlist* items) {
  const char* x;
  int i, col = 0;
  size_t n;

  i = 0;

  strlist_foreach(items, x, n) {
    if(x[0] != '/') {
      if(i)
        buffer_puts(buffer_1, (oneline && col > 0) ? ", " : "\n#pragma config ");
      else
        buffer_puts(buffer_1, "#pragma config ");

    } else if(comments) {
      if(i) {
        col = -1;
        buffer_puts(buffer_1, "\n\n");
      }
    }

    csetting* setting = find_setting(x);
    const char* description = setting ? setting->description : 0;

    cvalue* value = find_value(x);

    if(value)
      description = value->description;

    if(!oneline && description && description[0]) {
      buffer_putspad(buffer_1, x, 20);
      buffer_putm_internal(buffer_1, " // ", description, NULL);

    } else if(comments)
      buffer_put(buffer_1, x, n);

    ++i;
    ++col;
  }

  buffer_putnlflush(buffer_1);
}

/**
 * @brief usage  Show command line usage
 * @param argv0
 */
void
usage(char* argv0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(argv0),
                       " <hex-file> <cfgdata-file>\n"
                       "\n"
                       "Options\n"
                       "  -h, --help                show this help\n"
                       "  -o, --oneline             output oneliner\n"
                       "  -D, --no-default          don't output settings with default value\n"
                       "  -d, --default             output settings with default value\n"
                       "  -C, --no-comments         don't output description comments\n"
                       "  -n, --name                output register name\n"
                       "  -v, --verbose             show verbose messages\n"
                       "\n",
                       NULL);
  buffer_putnlflush(buffer_1);
}

int
main(int argc, char* argv[]) {
  const char* x;
  size_t i, n;
  int c, index = 0;
  const char *cfgdata = 0, *hexfile = 0;

  struct unix_longopt opts[] = {{"help", 0, NULL, 'h'},
                                {"oneline", 0, &oneline, 1},
                                {"no-default", 0, &nodefault, 1},
                                {"default", 0, &nodefault, 0},
                                {"no-comments", 0, &comments, 0},
                                {"name", 0, &output_name, 1},
                                {"verbose", 0, &verbose, 1},
                                {0, 0, 0, 0}};

  for(;;) {
    c = unix_getopt_long(argc, argv, "hodCnv", opts, &index);
    if(c == -1)
      break;
    if(c == 0)
      continue;

    switch(c) {
      case 'h': usage(argv[0]); return 0;
      case 'o': oneline = 1; break;
      case 'd': nodefault = 0; break;
      case 'D': nodefault = 1; break;
      case 'C': comments = 0; break;
      case 'n': output_name = 1; break;
      case 'v': verbose++; break;
      default:
        buffer_puts(buffer_2, "No such option '-");
        buffer_putc(buffer_2, c);
        buffer_putsflush(buffer_2, "'\n");

        return 1;
    }
  }

  if(unix_optind < argc) {
    hexfile = argv[unix_optind++];

    if(unix_optind < argc)
      cfgdata = argv[unix_optind++];
  }

  if(!hexfile)
    hexfile = "/home/roman/Sources/pictest/bootloaders/usb-msd-bootloader-18f2550.hex";

  if(cfgdata) {
    if(!path_exists(cfgdata))
      cfgdata = get_cfgdat(cfgdata);
  } else {
    const char* chip = infer_chip(hexfile, str_len(hexfile));
    
    if(chip)
      cfgdata = get_cfgdat(chip);
  }

  if(!(x = mmap_read(cfgdata, &n))) {
    errmsg_warnsys("Error opening file ", cfgdata, ":", 0);
    return 127;
  }
  assert(x);
  assert(n);
  cfg_data(&words, x, n);
  mmap_unmap(x, n);

  if(!(x = mmap_read(hexfile, &n))) {
    errmsg_warnsys("Error opening file ", hexfile, ":", 0);
    return 127;
  }

  assert(x);
  assert(n);
  ihex_init(&hex);
  ihex_read_buf(&hex, x, n);
  mmap_unmap(x, n);

  stralloc_init(&cfg);
  config_bytes(&hex, &cfg, &baseaddr);

  strlist_init(&pragmas, '\0');

  process_config(&add_item, &add_comment, &pragmas);

  if(verbose) {
    cword* word;

    slink_foreach(words, word) {
      uint16 value = config_data_at(word->address);
      buffer_puts(buffer_2, word->name);
      buffer_puts(buffer_2, " @ ");
      buffer_putxlong0(buffer_2, word->address, 4);
      buffer_puts(buffer_2, ": ");
      buffer_putxlong0(buffer_2, value & word->mask, 4);
      buffer_putnlflush(buffer_2);
    }

    /*    for(i = 0; i < cfg.len; i +=
       2) { uint16 v =
       uint16_read(&cfg.s[i]);

          buffer_putxlong0(buffer_2,
       baseaddr + i, 4);
          buffer_puts(buffer_2, ": ");
          buffer_putxlong0(buffer_2, v,
       4); buffer_putnlflush(buffer_2);
        }*/
  }

  output_items(&pragmas);

  return 0;
}
