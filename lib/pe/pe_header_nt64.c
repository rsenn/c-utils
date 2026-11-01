// #define NO_UINT16_MACROS 1
#include "../uint16.h"
#include "../pe.h"

pe64_nt_headers*
pe_header_nt64(void* pe) {
  return PE_64(pe) ? (pe64_nt_headers*)pe_header_nt(pe) : (pe64_nt_headers*)0;
}
