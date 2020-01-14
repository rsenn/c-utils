#include "lib/uint8.h"
#include "lib/uint16.h"
#include "lib/uint32.h"
#include "lib/slist.h"
#include "lib/mmap.h"
#include "lib/alloc.h"
#include "lib/byte.h"
#include "lib/str.h"
#include "lib/scan.h"
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
  csetting** sptr = NULL;
  cvalue** vptr = NULL;

  while(n >= 0) {

    eol = byte_chr(x, n, '\n');
    col = byte_chr(x, n, ':') + 1;
    assert(col < eol);
    x += col;
    n -= col;
    eol -= col;

    if(!str_diffn(x, "CWORD", 5)) {
      wptr = parse_cfgword(wptr, x, eol);
      sptr = &(*wptr)->settings;
    } else if(!str_diffn(x, "CSETTING", 8)) {
      sptr = parse_cfgsetting(sptr, x, eol);
      vptr = &(*sptr)->values;
    } else if(!str_diffn(x, "CVALUE", 6)) {
      vptr = parse_cfgvalue(vptr, x, eol);
    }

    x += eol;
    n -= eol;
  }
}

int
main(int argc, char* argv[]) {
  const char* x;
  size_t n;

  if(argv[1] == NULL)
    argv[1] = "/opt/microchip/xc8/v1.43/dat/cfgdata/18f2550.cfgdata";

  x = mmap_read(argv[1], &n);

  parse_cfgdata(&words, x, n);

  mmap_unmap(x, n);
  return 0;
}