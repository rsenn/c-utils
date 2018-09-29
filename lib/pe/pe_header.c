#include "../pe.h"

uint32*
pe_header_sig(void* pe) {
  pe_dos_header* dos = pe;
  return (uint32*)((unsigned char*)dos + dos->e_lfanew);
}

void*
pe_header_nt(void* pe) {
  pe_dos_header* dos = pe;
  return (unsigned char*)dos + dos->e_lfanew;
}

pe32_nt_headers*
pe_header_nt32(void* pe) {
  pe32_nt_headers* nthdr = pe_header_nt(pe);
  return nthdr->optional_header.magic == PE_MAGIC_PE32 ? nthdr : NULL;
}

pe64_nt_headers*
pe_header_nt64(void* pe) {
  pe64_nt_headers* nthdr = pe_header_nt(pe);
  return nthdr->optional_header.magic == PE_MAGIC_PE64 ? nthdr : NULL;
}

pe_coff_header*
pe_header_coff(void* pe) {
  uint32* sign = pe_header_sig(pe);
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
  return hdr->magic == PE_MAGIC_PE32 ? hdr : NULL;
}

pe64_opt_header*
pe_header_opt64(void* pe) {
  pe64_opt_header* hdr = pe_header_opt(pe);
  return hdr->magic == PE_MAGIC_PE64 ? hdr : NULL;
}

pe_section_header*
pe_header_sections(void* pe, int* nsections) {
  pe_coff_header* coff = pe_header_coff(pe);
  unsigned char* opthdr = &coff[1];
  if(nsections) *nsections = coff->number_of_sections;
  return (pe_section_header*)(opthdr + coff->size_of_optional_header);
}

pe_data_directory*
pe_header_datadir(void* pe) {
  uint8* opthdr = pe_header_opt(pe);
  return (pe_data_directory*)(*(uint16*)opthdr == PE_MAGIC_PE32 ? opthdr + 96 : opthdr + 112);
}
