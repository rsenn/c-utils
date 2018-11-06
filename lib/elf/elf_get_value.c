#include "../elf.h"
#include "../uint64.h"

uint64
elf_get_value(void* elf, void* ptr, unsigned off32, unsigned size32, unsigned off64, unsigned size64) {
  uint8* base = elf;
  uint8* p = ptr;
  unsigned off, size;
  uint64 ret = 0;
  if(ELF_32(base)) {
    off = off32;
    size = size32;
  } else {
    off = off64;
    size = size64;
  }
  switch(size) {
    case 8: ret = uint64_read((const char*)&p[off]); break;
    case 4: ret = uint32_read((const char*)&p[off]); break;
    case 2: ret = uint16_read((const char*)&p[off]); break;
    case 1: ret = p[off]; break;
  }
  return ret;
}
