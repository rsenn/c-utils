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
#include "lib/fmt.h"
#include "lib/path.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>

typedef void cfg_item(strlist*, const char*, const char*);
typedef void cfg_comment(strlist*, const char*);

typedef struct cvalue {
  struct cvalue* next;
  uint16 value;
  int is_default;
  const char* name;
  const char* description;
} cfg_value;

typedef struct csetting {
  struct csetting* next;
  uint16 mask;
  const char* name;
  const char* description;
  struct cvalue* values;
} cfg_setting;

typedef struct cword {
  struct cword* next;
  uint32 address;
  uint16 mask;
  uint16 default_value;
  const char* name;
  struct csetting* settings;
} cfg_word;

static struct cword* words;
static ihex_file hex;
static uint32 baseaddr;
static stralloc cfg;
static strlist pragmas;
static int nodefault = false, oneline = false, comments = true, names = true, output_name = false, verbose = 0;

static void
cfg_add_item(strlist* output, const char* name, const char* value) {
  stralloc out;

  stralloc_init(&out);
  stralloc_catm_internal(&out, value ? name : "// ", value ? " = " : name, value, NULL);
  strlist_push_sa(output, &out);
  stralloc_free(&out);
}

static void
cfg_add_comment(strlist* output, const char* text) {
  stralloc out;

  stralloc_init(&out);
  stralloc_catm_internal(&out, "/* ", text, " */", NULL);
  strlist_push_sa(output, &out);
  stralloc_free(&out);
}

uint16
cfg_data_at(uint32 addr) {
  ssize_t offset, result = -1;

#ifdef DEBUG_OUTPUT_
  buffer_putm_internal(buffer_2, __FUNCTION__, "(): ", "address = 0x", 0);
  buffer_putxlong(buffer_2, addr);
  buffer_putnlflush(buffer_2);
#endif

  if(addr < 0x2007 && addr >= 0x2000) {
    offset = (ssize_t)addr - 0x2000;

    if(offset + 1 < cfg.len)
      result = uint16_read(&cfg.s[offset]);

  } else if(baseaddr == 0x0000400e) {
    offset = (ssize_t)addr - 0x2007;

    if(offset + 1 < cfg.len)
      result = uint16_read(&cfg.s[offset]);

  } else if(addr >= 0x8007) {
    offset = ((ssize_t)addr - 0x8007) * 2;

    if(cfg.len < offset)
      offset &= 0xfff;

    if(offset + 1 < cfg.len)
      result = uint16_read(&cfg.s[offset]);
  } else {
    offset = (ssize_t)addr & 0x0fff;

    if(offset < cfg.len)
      result = cfg.s[offset];
  }

  if(offset >= cfg.len) {
#ifdef DEBUG_OUTPUT_
    buffer_putm_internal(buffer_2, __FUNCTION__, "(): ", "offset >= cfg.len (", 0);
    buffer_putlonglong(buffer_2, offset);
    buffer_puts(buffer_2, " >= ");
    buffer_putulong(buffer_2, cfg.len);
    buffer_putm_internal(buffer_2, ")", 0);
    buffer_putnlflush(buffer_2);
#endif

    result = -1;
  }

  // assert(offset < cfg.len);

#ifdef DEBUG_OUTPUT_
  buffer_putm_internal(buffer_2, __FUNCTION__, "(): ", "result = ", result >= 0 ? "0x" : 0, 0);

  if(result < 0)
    buffer_putlonglong(buffer_2, result);
  else
    buffer_putxlonglong(buffer_2, result);
  buffer_putnlflush(buffer_2);
#endif

  return result;
}

uint16
cfg_read_word(cfg_word* word, cfg_setting* setting) {
  uint16 value = cfg_data_at(word->address);

  value &= setting->mask;

  return value;
}

cfg_value**
cfg_read_value(cfg_value** vptr, cfg_word* w, cfg_setting* s, const char* x, size_t n) {
  cfg_value* v = *vptr = alloc(sizeof(cfg_value));
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

cfg_setting**
cfg_read_setting(cfg_setting** sptr, cfg_word* word, const char* buf, size_t len) {
  cfg_setting* setting = *sptr = alloc(sizeof(cfg_setting));
  size_t i;
  unsigned long value;

  i = scan_xlongn(buf, len, &value);

  assert(i);

  setting->mask = value;
  buf += i + 1;
  len -= i + 1;
  i = byte_chr(buf, len, ':');
  setting->name = str_ndup(buf, i);

  assert(i < len);

  buf += i + 1;
  len -= i + 1;

  setting->description = str_ndup(buf, len);
  setting->next = NULL;
  setting->values = NULL;

  return &setting->next;
}

void
cfg_dump_word(buffer* b, cfg_word* word) {
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
cfg_dump_setting(buffer* b, cfg_setting* setting) {
  buffer_puts(b, "  ");
  buffer_puts(b, setting->name ? setting->name : "(null)");
  buffer_puts(b, "\tmask 0x");
  buffer_putxlong0(b, setting->mask, 2);
  buffer_puts(b, "\t\t");
  buffer_puts(b, setting->description);
  buffer_putnlflush(b);
}

void
cfg_dump_value(buffer* b, cfg_value* value) {
  buffer_puts(b, "    ");
  buffer_puts(b, value->name ? value->name : "(null)");
  buffer_puts(b, "\tvalue 0x");
  buffer_putxlong0(b, value->value, 2);
  buffer_puts(b, "\t\t");
  buffer_puts(b, value->description);
  buffer_putnlflush(b);
}

size_t
cfg_get_bytes(ihex_file* ihf, stralloc* sa, uint32* addr) {
  size_t bytes;

  stralloc_zero(sa);
  stralloc_ready(sa, 14);

  if(((bytes = ihex_read_at(&hex, 0x00300000, sa->s, 14)) == 14)) {
    *addr = 0x00300000;

  } else {
    if((bytes = ihex_read_at(&hex, 0x8007 << 1, sa->s, 4)) == 4)
      *addr = 0x8007;
    else if((bytes = ihex_read_at(&hex, 0x400e, sa->s, 2)) == 2)
      *addr = 0x400e;
  }

  sa->len = bytes;

  return bytes;
}

void
cfg_print(buffer* b, stralloc* sa) {
  size_t i, n = sa->len >> 1;

  for(i = 0; i < n; i++) {
    buffer_putxlong0u(b, (((uint8_t*)sa->s)[i * 2] << 8) | ((uint8_t*)sa->s)[i * 2 + 1], 4);
    buffer_putnlflush(b);
    /*if(i < n - 1) buffer_putc(b, '\n');*/
  }
}

cfg_value*
cfg_get_value(cfg_word* word, cfg_setting* setting) {
  cfg_value* value;
  uint16 byteval = cfg_read_word(word, setting);

#ifdef DEBUG_OUTPUT_
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

cfg_setting*
cfg_find_setting(const char* str) {
  cfg_word* word;
  cfg_setting* setting;

  slink_foreach(words, word) for(setting = word->settings; setting; setting = setting->next) if(!str_diffn(str, setting->name, str_len(setting->name))) return setting;

  return NULL;
}

cfg_value*
cfg_find_value(const char* str) {
  cfg_setting* setting;

  if((setting = cfg_find_setting(str))) {
    cfg_value* value;

    str += str_chr(str, '=');

    while(*str == '=' || *str == ' ')
      ++str;

    slink_foreach(setting->values, value) if(!str_diffn(str, value->name, str_len(value->name))) return value;
  }

  return NULL;
}

const char*
cfg_infer_chip(const char* x, size_t n) {
  static stralloc chip;
  size_t i;

  for(i = 0; i + 3 < n; i++) {
    size_t len = n - i;
    const char* setting = &x[i];

    if(*setting == '1') {
      char c2 = tolower(setting[2]);

      if(c2 == 'l' || c2 == 'f') {
        char c1 = setting[1];

        if(c1 == '2' || c1 == '6' || c1 == '8') {
          len = scan_charsetnskip(setting,
                                  "0123456789aAbBcCeEfFgGhH                                  IjJkKlLmMnNpPrRtTvV",
                                  len);

          if(len > 3) {
            stralloc_copyb(&chip, setting, len);
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
cfg_get_data(const char* chip) {
  size_t i;
  static stralloc path;
  static const char* const search_dirs[] = {
      "/opt/microchip",
      "C:\\Program Files\\Microchip",
      "C:\\Program Files (x86)\\Microchip",
  };

  if(path.len == 0) {
    dir_t d;
    const char *dir = 0, *subdir;

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

      /*buffer_putm_internal(buffer_2, "subdir = ", subdir, NULL);
        buffer_putnlflush(buffer_2);*/
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
#ifdef DEBUG_OUTPUT_
    buffer_putm_internal(buffer_2, "Found cfgdata: ", path.s, NULL);
    buffer_putnlflush(buffer_2);
#endif

    return path.s;
  }

  return 0;
}

cfg_word**
cfg_get_word(cfg_word** wptr, const char* buf, size_t len) {
  cfg_word* word = *wptr = alloc(sizeof(cfg_word));
  size_t i;
  unsigned long value;

  i = scan_xlongn(buf, len, &value);

  assert(i);

  word->address = value;
  buf += i + 1;
  len -= i + 1;
  i = scan_xlongn(buf, len, &value);

  assert(i);

  word->mask = value;
  buf += i + 1;
  len -= i + 1;
  i = scan_xlongn(buf, len, &value);

  assert(i);

  word->default_value = value;
  buf += i + 1;
  len -= i + 1;

  word->name = str_ndup(buf, len);
  word->next = NULL;
  word->settings = NULL;

  return &word->next;
}

int
cfg_data(cfg_word** wptr, const char* buf, size_t len) {
  size_t eol, col;
  cfg_word* word = 0;
  cfg_setting *setting = 0, **sptr = NULL;
  cfg_value *value = 0, **vptr = NULL;

  while(len > 0) {
    eol = byte_chr(buf, len, '\n');

    if(eol > 0 && buf[0] == 'C') {
      const char* line = buf;
      col = byte_chr(buf, len, ':') + 1;
      assert(col < eol);
      buf += col;
      len -= col;
      eol -= col;

      if(!str_diffn(line, "CWORD", 5)) {
        cfg_word** nwptr = cfg_get_word(wptr, buf, eol);

        word = *wptr;
        sptr = &word->settings;
        wptr = nwptr;
      } else if(!str_diffn(line, "CSETTING", 8)) {
        cfg_setting** nsptr = cfg_read_setting(sptr, word, buf, eol);
        setting = *sptr;
        vptr = &setting->values;
        sptr = nsptr;
      } else if(!str_diffn(line, "CVALUE", 6)) {
        cfg_value** nvptr = cfg_read_value(vptr, word, setting, buf, eol);
        value = *vptr;
        vptr = nvptr;
      }
    }

    if(eol < len)
      eol++;

    buf += eol;
    len -= eol;
  }

  return 0;
}

void
cfg_process(cfg_item add_item, cfg_comment add_comment, strlist* output) {
  cfg_word *prevword = 0, *word;
  cfg_value* value;
  char cbuf[64];
  size_t n = 0;

  slink_foreach(words, word) {
    if(!str_diffn(word->name, "IDLOC", 5))
      break;

    if(verbose)
      cfg_dump_word(buffer_2, word);

    if(comments && output_name)
      add_comment(output, word->name);

    for(cfg_setting* setting = word->settings; setting; setting = setting->next) {
      if((value = cfg_get_value(word, setting)) == NULL) {
        buffer_puts(buffer_2, "WARNING:  value ");
        buffer_putxlong(buffer_2, cfg_read_word(word, setting));
        buffer_putm_internal(buffer_2, " for setting ", setting->name, " not found!", NULL);
        buffer_putnlflush(buffer_2);

        continue;
      }

      if(value->is_default && nodefault) {
        if(verbose > 1) {
          buffer_putm_internal(buffer_2, "skip default value ", value->name, " for setting ", setting->name, NULL);
          buffer_putnlflush(buffer_2);
        }

        continue;
      }

      if(output_name && prevword != word) {
        // if(prevword)
        buffer_putnlflush(buffer_2);

        add_item(output, word->name, NULL);
      }

      add_item(output, setting->name, value->name);
      prevword = word;
    }
  }
}

void
cfg_output(const strlist* items) {
  const char *x, *description;
  int i = 0, col = 0;
  size_t n;
  cfg_setting* setting;
  cfg_value* value;

  if(oneline && comments)
    buffer_puts(buffer_1, "#pragma config ");

  strlist_foreach(items, x, n) {
    if(x[0] != '/') {
      buffer_puts(buffer_1, i == 0 ? "#pragma config " : oneline ? ((i < 1 || col < 1) ? " " : ", ") : "\n#pragma config ");

    } else if(comments) {
      if(i != 0) {
        col = -1;

        if(!oneline)
          buffer_puts(buffer_1, "\n\n");
      }
    }

    setting = cfg_find_setting(x);
    description = setting ? setting->description : 0;
    value = cfg_find_value(x);

    if(value)
      description = value->description;

    if(!oneline && description && description[0]) {
      buffer_putspad(buffer_1, x, 20);

      if(comments)
        buffer_putm_internal(buffer_1, " // ", description, NULL);

    } else /*if(comments)*/ {
      buffer_put(buffer_1, x, n);
    }

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
                       "  -D, --no-default          don't output settings with default                        alue\n"
                       "  -d, --default             output settings with default value\n"
                       "  -C, --no-comments         don't output description comments\n"
                       "  -n, --name                output register name\n"
                       "  -N, --no-name             don't output register name\n"
                       "  -v, --verbose             show verbose messages\n"
                       "\n",
                       NULL);

  buffer_putnlflush(buffer_1);
}

int
main(int argc, char* argv[]) {
  const char *x, *cfgdata = 0, *hexfile = 0;
  size_t i, n;
  int c, index = 0, print = 0;

  struct unix_longopt opts[] = {
      {"help", 0, NULL, 'h'},
      {"oneline", 0, &oneline, 1},
      {"no-default", 0, &nodefault, 1},
      {"default", 0, &nodefault, 0},
      {"comments", 0, NULL, 'c'},
      {"no-comments", 0, NULL, 'C'},
      {"name", 0, NULL, 'n'},
      {"no-name", 0, NULL, 'N'},
      {"verbose", 0, NULL, 'v'},
      {0, 0, 0, 0},
  };

  for(;;) {
    if((c = unix_getopt_long(argc, argv, "hodcCnNvp", opts, &index)) == -1)
      break;

    if(c == 0)
      continue;

    switch(c) {
      case 'h': usage(argv[0]); return 0;
      case 'o': oneline = true; break;
      case 'd': nodefault = false; break;
      case 'D': nodefault = true; break;
      case 'c': comments = true; break;
      case 'C': comments = false; break;
      case 'n': output_name = true; break;
      case 'N': output_name = false; break;
      case 'v': verbose++; break;
      case 'p': print = true; break;
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

  if(!hexfile) {
    usage(argv[0]);
    return 1;
  }

  if(cfgdata) {
    if(!path_exists(cfgdata))
      cfgdata = cfg_get_data(cfgdata);

  } else {
    const char* chip;

    if((chip = cfg_infer_chip(hexfile, str_len(hexfile))))
      cfgdata = cfg_get_data(chip);
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
  cfg_get_bytes(&hex, &cfg, &baseaddr);

  strlist_init(&pragmas, '\0');

  cfg_process(&cfg_add_item, &cfg_add_comment, &pragmas);

  if(verbose) {
    cfg_word* word;

    slink_foreach(words, word) {
      uint16 value = cfg_data_at(word->address);

      buffer_puts(buffer_2, word->name);
      buffer_puts(buffer_2, " @ ");
      buffer_putxlong0(buffer_2, word->address, 4);
      buffer_puts(buffer_2, ": ");
      buffer_putxlong0(buffer_2, value & word->mask, 4);
      buffer_putnlflush(buffer_2);
    }

    /*for(i = 0; i < cfg.len; i += 2) {
      uint16 v = uint16_read(&cfg.s[i]);

      buffer_putxlong0(buffer_2, baseaddr + i, 4);
      buffer_puts(buffer_2, ": ");
      buffer_putxlong0(buffer_2, v, 4);
      buffer_putnlflush(buffer_2);
    }*/
  }

  if(print)
    cfg_print(buffer_1, &cfg);
  else
    cfg_output(&pragmas);

  return 0;
}
