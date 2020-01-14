#include "lib/uint8.h"
#include "lib/uint16.h"
#include "lib/uint32.h"
#include "lib/slist.h"
#include "lib/mmap.h"


typedef struct cvalue {
  struct cvalue* next;
  uint16 mask;
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

typedef struct {
  struct cword* next;
  uint32 address;
  uint16 mask;
  uint16 default_value;
  const char* name;
  csetting* settings;
} cword;

int
parse_cfgdata(char* x, size_t n) {
  size_t eol;

  while(n >= 0) {


    eol = byte_chr(x, n, '\n');

    

    x += eol;
    n -= eol;
  }
}

int
main(int argc, char *argv[]) {
  char* x;
  size_t n;

  if(argv[1]  == NULL)
    argv[1] = "/opt/microchip/xc8/v1.43/dat/cfgdata/18f2550.cfgdata";

  x = mmap_read(argv[1], &n);


  mmap_unmap(x, n);
  return 0;
}