#include "../pe.h"

uint64
pe_thunk(void* pe, void* ptr, int64 index) {
  uint64 ret;
  switch(PE_TYPE(pe)) {
    case PE_MAGIC_PE32: {
      uint32 u = uint32_get(((uint32*)ptr + index));
      ret = ((u & 0x80000000LL) << 32) | (u & 0x7fffffff);
      break;
    }
    case PE_MAGIC_PE64: {
      ret = uint64_get(((uint64*)ptr + index));
      break;
    }
  }
  return ret;
}