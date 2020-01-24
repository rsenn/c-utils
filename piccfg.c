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
#include "lib/map.h"
#include <assert.h>

typedef struct cvalue {
  struct cvalue* next;
  uint16 value;
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
static uint32 addr;
static stralloc cfg;
static map_t(const char*) pragmas;

uint8
config_byte_at(uint32 addr) {
  size_t offs = addr & 0x0fff;
  assert(offs < cfg.len);
  return cfg.s[offs];
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
parse_cfgvalue(cvalue** vptr, const char* x, size_t n) {
  cvalue* v = *vptr = alloc(sizeof(cvalue));
  size_t i;
  unsigned long value;
  i = scan_xlongn(x, n, &value);
  assert(i);
  v->value = value;
  x += i + 1;
  n -= i + 1;
  i = byte_chr(x, n, ':');
  v->name = str_ndup(x, i);
  assert(i < n);
  x += i + 1;
  n -= i + 1;
  v->description = str_ndup(x, n);
  v->next = NULL;
  return &v->next;
}

csetting**
parse_cfgsetting(csetting** sptr, const char* x, size_t n) {
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
parse_cfgword(cword** wptr, const char* x, size_t n) {
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
parse_cfgdata(cword** wptr, const char* x, size_t n) {
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
        cword** nwptr = parse_cfgword(wptr, x, eol);
        w = *wptr;
        sptr = &w->settings;
        wptr = nwptr;
      } else if(!str_diffn(line, "CSETTING", 8)) {
        csetting** nsptr = parse_cfgsetting(sptr, x, eol);
        s = *sptr;
        vptr = &s->values;
        sptr = nsptr;
      } else if(!str_diffn(line, "CVALUE", 6)) {
        cvalue** nvptr = parse_cfgvalue(vptr, x, eol);
        v = *vptr;
        vptr = nvptr;
      }
    }

    if(eol < n)
      eol++;

    x += eol;
    n -= eol;
  }
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

uint8
get_setting_byte(cword* word, csetting* setting) {
  uint8 value = config_byte_at(word->address);

  value &= setting->mask;

  return value;
}

cvalue*
get_setting_value(cword* word, csetting* setting) {
  cvalue* value;
  uint8 byteval = get_setting_byte(word, setting);
  slink_foreach(setting->values, value) {
    if(value->value == byteval)
      return value;
  }
  return NULL;
}

void
process_config() {
  cword* word;
  csetting* setting;
  cvalue* value;

  map_init(&pragmas);

  slink_foreach(words, word) {
    if(!str_diffn(word->name, "IDLOC", 5))
      break;

    dump_cword(buffer_2, word);

    slink_foreach(word->settings, setting) {

      value = get_setting_value(word, setting);

      map_set(&pragmas, setting->name, value->name);
      /*
       *//*
 dump_csetting(buffer_2, setting);
  dump_cvalue(buffer_2, value);*/

      /*
       slink_foreach(setting->values, value) {  }*/
    }
  }

  const char* key;

  for(map_iter_t it = map_iter(&pragmas); (key = map_next(&pragmas, &it));) {
    const char* value = *(const char**)map_get(&pragmas, key);
    buffer_putm_internal(buffer_1, "#pragma ", key, " = ", value, 0);
    buffer_putnlflush(buffer_1);
  }
}

int
main(int argc, char* argv[]) {
  const char* x;
  size_t n;

  const char* cfgdata = argc >= 2 ? argv[1] : "/opt/microchip/xc8/v1.43/dat/cfgdata/18f2550.cfgdata";
  const char* hexfile = argc >= 3 ? argv[2] : "/home/roman/Sources/pictest/bootloaders/usb-msd-bootloader-18f2550.hex";

  x = mmap_read(cfgdata, &n);
  parse_cfgdata(&words, x, n);
  mmap_unmap(x, n);

  x = mmap_read(hexfile, &n);
  ihex_load_buf(&hex, x, n);
  mmap_unmap(x, n);

  stralloc_init(&cfg);
  config_bytes(&hex, &cfg, &addr);

  for(size_t i = 0; i < cfg.len; i += 2) {
    uint16 v = uint16_read(&cfg.s[i]);

    buffer_putxlong0(buffer_2, addr + i, 4);
    buffer_puts(buffer_2, ": ");
    buffer_putxlong0(buffer_2, v, 4);
    buffer_putnlflush(buffer_2);
  }

  process_config();

  return 0;
}