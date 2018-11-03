#include "../pe.h"

pe_data_directory*
pe_get_datadir(void* pe, size_t* num) {
  char* opthdr = pe_header_opt(pe);

  if(num)
    *num = PE_GET(pe, opthdr, opt_header, number_of_rva_and_sizes);

  return (pe_data_directory*)(opthdr + PE_OFFSET(pe, opt_header, data_directory));
}
