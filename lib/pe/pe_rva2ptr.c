#include "../pe.h"

void*
pe_rva2ptr(void* base, uint32 rva) {
  int64 off = pe_rva2offset(base, rva);
  if(off == -1) return 0;
  return (uint8*)base + off;
}
