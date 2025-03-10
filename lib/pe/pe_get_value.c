#include "../pe.h"
#include "../uint64.h"

uint64
pe_get_value(void* pe, void* ptr, unsigned off32, unsigned size32, unsigned off64, unsigned size64) {
  uint8* base = (uint8*)pe;
  uint8* p = (uint8*)ptr;
  unsigned off, size;
  uint64 ret = 0;

  if(PE_32(base)) {
    off = off32;
    size = size32;
  } else {
    off = off64;
    size = size64;
  }

  switch(size) {
    case 8: ret = uint64_get(&p[off]); break;
    case 4: ret = uint32_get(&p[off]); break;
    case 2: ret = uint16_get(&p[off]); break;
    case 1: ret = p[off]; break;
  }
  return ret;
}
