// #define NO_UINT16_MACROS 1
#include "../uint16.h"
#include "../pe.h"

pe_data_directory*
pe_header_datadir(void* pe) {
  uint8* opthdr = (uint8*)pe_header_opt(pe);
  return (pe_data_directory*)(PE_32(pe) ? opthdr + 96 : opthdr + 112);
}
