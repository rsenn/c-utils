// #define NO_UINT16_MACROS 1
#include "../uint16.h"
#include "../pe.h"

pe_coff_header*
pe_header_coff(void* pe) {
  uint32* sign = (uint32*)pe_header_nt(pe);
  return (pe_coff_header*)&sign[1];
}
