// #define NO_UINT16_MACROS 1
#include "../uint16.h"
#include "../pe.h"

pe32_nt_headers*
pe_header_nt32(void* pe) {
  return PE_32(pe) ? (pe32_nt_headers*)pe_header_nt(pe) : (pe32_nt_headers*)0;
}
