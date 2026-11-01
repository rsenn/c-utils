// #define NO_UINT16_MACROS 1
#include "../uint16.h"
#include "../pe.h"

void*
pe_header_opt(void* pe) {
  pe_coff_header* coff = pe_header_coff(pe);
  return &coff[1];
}
