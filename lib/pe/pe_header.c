#include "../pe.h"

void*
pe_header_nt(void* pe) {
  pe_dos_header* dos = pe;
  uint32 lfanew;
  uint32_unpack(&dos->e_lfanew, &lfanew);
  return (unsigned char*)dos + lfanew;
}

uint32
pe_header_sig(void* pe) {
  return *(uint32*)pe_header_nt(pe);
}

pe32_nt_headers*
pe_header_nt32(void* pe) {
  return PE_32(pe) ? pe_header_nt(pe) : 0;
}

pe64_nt_headers*
pe_header_nt64(void* pe) {
  return PE_64(pe) ? pe_header_nt(pe) : 0;
}

pe_coff_header*
pe_header_coff(void* pe) {
  uint32* sign = pe_header_nt(pe);
  return (pe_coff_header*)&sign[1];
}

void*
pe_header_opt(void* pe) {
  pe_coff_header* coff = pe_header_coff(pe);
  return &coff[1];
}

pe32_opt_header*
pe_header_opt32(void* pe) {
  pe32_opt_header* hdr = pe_header_opt(pe);
  return PE_32(pe) ? hdr : 0;
}

pe64_opt_header*
pe_header_opt64(void* pe) {
  pe64_opt_header* hdr = pe_header_opt(pe);
  return PE_64(pe) ? hdr : 0;
}

pe_section_header*
pe_header_sections(void* pe, int* nsections) {
  pe_coff_header* coff = pe_header_coff(pe);
  unsigned char* opthdr = &coff[1];
  uint16 optsize;
  uint16_unpack(&coff->size_of_optional_header, &optsize);
  if(nsections) {
    uint16 num;
    uint16_unpack(&coff->number_of_sections, &num); 
    *nsections = num;
  }
  return (pe_section_header*)(opthdr + optsize);
}

pe_data_directory*
pe_header_datadir(void* pe) {
  uint8* opthdr = pe_header_opt(pe);
  return PE_32(pe) ? opthdr + 96 : opthdr + 112;
}
