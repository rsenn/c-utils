#include "../pe.h"

pe_data_directory*
pe_get_datadir(void* pe, uint32* num) {
  char* opthdr = (char*)pe_header_opt(pe);

  if(num)
    *num = uint32_get(PE_ADDR(pe, opthdr, opt_header, number_of_rva_and_sizes));

  return (pe_data_directory*)(opthdr +
                              PE_OFFSET(pe, opt_header, data_directory));
}
