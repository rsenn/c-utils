// #define NO_UINT16_MACROS 1
#include "../uint16.h"
#include "../pe.h"

uint32
pe_header_sig(void* pe) {
  return *(uint32*)pe_header_nt(pe);
}
