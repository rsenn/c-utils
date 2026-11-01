// #define NO_UINT16_MACROS 1
#include "../uint16.h"
#include "../pe.h"

pe64_opt_header*
pe_header_opt64(void* pe) {
  pe64_opt_header* hdr = (pe64_opt_header*)pe_header_opt(pe);
  return PE_64(pe) ? hdr : 0;
}
