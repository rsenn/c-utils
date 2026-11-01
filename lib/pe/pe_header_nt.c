// #define NO_UINT16_MACROS 1
#include "../uint16.h"
#include "../pe.h"

void*
pe_header_nt(void* pe) {
  pe_dos_header* dos = (pe_dos_header*)pe;
  uint32 lfanew;
  lfanew = uint32_get(&dos->e_lfanew);
  return (unsigned char*)dos + lfanew;
}
