// #define NO_UINT16_MACROS 1
#include "../uint16.h"
#include "../pe.h"

pe32_opt_header*
pe_header_opt32(void* pe) {
  pe32_opt_header* hdr = (pe32_opt_header*)pe_header_opt(pe);
  return PE_32(pe) ? hdr : 0;
}
