/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "lib/buffer.h"
#include "lib/mmap.h"
#include "lib/pe.h"
#include "lib/str.h"
#include <stdlib.h>
#include <string.h>

void pe_dump_sections(uint8* content);

pe_section_header*
pe_get_section(void* base, const char* name) {
  int i, n;
  pe_section_header* sections = pe_header_sections(base, &n);

  for(i = 0; i < n; i++) {
    if(str_equal(name, sections[i].name))
      return &sections[i];
  }

  return NULL;
}

int64
pe32_rva2off(uint8* base, uint32 rva) {
  int i, n;
  pe_section_header* sections = pe_header_sections(base, &n);
 
  if(rva < sections[0].virtual_address)
    return rva;

  for(i = 0; i < n; i++) {
    uint32 addr = sections[i].virtual_address;
    uint32 size = sections[i].size_of_raw_data;

    if(rva >= addr && rva < addr + size)
      return rva - addr + sections[i].pointer_to_raw_data;
  }
  return -1;
}

int64
pe64_rva2off(void* base, uint64 rva) {
  int i, n;
  pe_section_header* sections = pe_header_sections(base, &n);
 
  if(rva < sections[0].virtual_address)
    return rva;

  for(i = 0; i < n; i++) {
    uint64 addr = sections[i].virtual_address;
    uint64 size = sections[i].size_of_raw_data;

    if(rva >= addr && rva < addr + size)
      return rva - addr + sections[i].pointer_to_raw_data;
  }
  return -1;
}

void*
pe32_rva2ptr(void* base, uint32 rva) {
  int64 off = pe32_rva2off(base, rva);
  if(off == -1) return NULL;
  return base + off;
}

void*
pe64_rva2ptr(uint8* base, uint64 rva) {
  int64 off = pe64_rva2off(base, rva);
  if(off == -1) return NULL;
  return base + off;
}

uint32
pe32_off2rva(uint8* base, int64 off) {
  int i;
  pe32_nt_headers* header = pe32_get_header(base);
  pe_section_header* sections = PE32_FIRST_SECTION(header);
 
  if((uint32)off < sections[0].pointer_to_raw_data)
    return off;

  for(i = 0; i < header->coff_header.number_of_sections; i++) {
    uint32 offset = sections[i].pointer_to_raw_data;
    uint32 bytes = sections[i].size_of_raw_data;

    if((uint32)off >= offset && (uint32)off < offset + bytes)
      return (uint32)off - offset + sections[i].virtual_address;
  }
  return 0;
}

void
pe_print_export_directory(buffer* b, pe_export_directory* export_dir) {
   buffer_puts(b, "characteristics: ");
    buffer_putxlonglong0(b, export_dir->characteristics, sizeof(export_dir->characteristics) * 2);
    buffer_puts(b, "\ntime_date_stamp: ");
    buffer_putxlonglong0(b, export_dir->time_date_stamp, sizeof(export_dir->time_date_stamp) * 2);
    buffer_puts(b, "\nmajor_version: ");
    buffer_putxlonglong0(b, export_dir->major_version, sizeof(export_dir->major_version) * 2);
    buffer_puts(b, "\nminor_version: ");
    buffer_putxlonglong0(b, export_dir->minor_version, sizeof(export_dir->minor_version) * 2);
    buffer_puts(b, "\nname: ");
    buffer_putxlonglong0(b, export_dir->name, sizeof(export_dir->name) * 2);
    buffer_puts(b, "\nbase: ");
    buffer_putxlonglong0(b, export_dir->base, sizeof(export_dir->base) * 2);
    buffer_puts(b, "\nnumber_of_functions: ");
    buffer_putxlonglong0(b, export_dir->number_of_functions, sizeof(export_dir->number_of_functions) * 2);
    buffer_puts(b, "\nnumber_of_names: ");
    buffer_putxlonglong0(b, export_dir->number_of_names, sizeof(export_dir->number_of_names) * 2);
    buffer_puts(b, "\naddress_of_functions: ");
    buffer_putxlonglong0(b, export_dir->address_of_functions, sizeof(export_dir->address_of_functions) * 2);
    buffer_puts(b, "\naddress_of_names: ");
    buffer_putxlonglong0(b, export_dir->address_of_names, sizeof(export_dir->address_of_names) * 2);
    buffer_puts(b, "\naddress_of_name_ordinals: ");
    buffer_putxlonglong0(b, export_dir->address_of_name_ordinals, sizeof(export_dir->address_of_name_ordinals) * 2);
    buffer_putnlflush(b);
}

void
pe32_dump_exports(uint8* content) {
  int i;
  pe32_nt_headers* nt_headers = pe32_get_header(content);
  pe_data_directory* export_dir = &nt_headers->optional_header.data_directory[PE_DIRECTORY_ENTRY_EXPORT];
  pe_section_header *text;
  uint32 fnaddr, *nameptr, *fnptr, mintextptr, maxtextptr;

  pe_export_directory* exports = pe32_rva2ptr(content, export_dir->virtual_address);
  pe_print_export_directory(buffer_1, exports);

  if(!(text = pe_get_section(content, ".text"))) {
    buffer_putsflush(buffer_2, "no .text section\n");
    return;
  }
  
  nameptr = (uint32*)pe32_rva2ptr(content, exports->address_of_names);
  fnptr = (uint32*)pe32_rva2ptr(content, exports->address_of_functions);
  mintextptr = text->virtual_address;
  maxtextptr = mintextptr + text->size_of_raw_data;

  for(i = 0; i < exports->number_of_names; i++) {
    fnaddr = fnptr[i];
    if(mintextptr < fnaddr && fnaddr < maxtextptr) {
      buffer_puts(buffer_1, pe32_rva2ptr(content, nameptr[i]));
      buffer_putnlflush(buffer_1);
    }
  }
}

void
pe64_dump_exports(uint8* content) {
  int i;
  pe64_nt_headers* nt_headers = pe64_get_header(content);
  pe_data_directory* export_dir = &nt_headers->optional_header.data_directory[PE_DIRECTORY_ENTRY_EXPORT];
  pe_section_header *text;
  uint64 fnaddr, *nameptr, *fnptr, mintextptr, maxtextptr;

  pe_export_directory* exports = pe64_rva2ptr(content, export_dir->virtual_address);
  pe_print_export_directory(buffer_1, exports);

  if(!(text = pe_get_section(content, ".text"))) {
    buffer_putsflush(buffer_2, "no .text section\n");
    return;
  }
  
  nameptr = (uint64*)pe64_rva2ptr(content, exports->address_of_names);
  fnptr = (uint64*)pe64_rva2ptr(content, exports->address_of_functions);
  mintextptr = text->virtual_address;
  maxtextptr = mintextptr + text->size_of_raw_data;

  for(i = 0; i < exports->number_of_names; i++) {
    fnaddr = fnptr[i];
    if(mintextptr < fnaddr && fnaddr < maxtextptr) {
      buffer_puts(buffer_1, pe64_rva2ptr(content, nameptr[i]));
      buffer_putnlflush(buffer_1);
    }
  }
}

int
main(int argc, char** argv) {
  uint8* content = NULL;
  size_t filesize;

  if(argc < 2) {
    buffer_putm(buffer_1, "Usage: ", str_basename(argv[0]), " XXX.dll\n");
    buffer_flush(buffer_1);
    return 0;
  }

  content = (uint8*)mmap_private(argv[1], &filesize);

  {
    pe32_nt_headers* nt_headers = pe32_get_header(content);

    if(nt_headers->signature != PE_NT_SIGNATURE) {
      buffer_putsflush(buffer_2, "not PE\n");
      return -1;
    }

    if(!(nt_headers->coff_header.characteristics & PE_FILE_DLL)) {
      buffer_putsflush(buffer_2, "not DLL\n");
      return -1;
    }
    pe_dump_sections(content);

    if(nt_headers->optional_header.magic == PE_MAGIC_PE64)
      pe64_dump_exports(content);
    else 
      pe32_dump_exports(content);
  }

  mmap_unmap(content, filesize);

  return 0;
}

void
pe_dump_sections(uint8* content) {
  int i, n;
  pe_section_header* sections = pe_get_sections(content, &n);

  buffer_putspad(buffer_1, "section name", 16);
  buffer_putspace(buffer_1);
  buffer_putspad(buffer_1, "vsize", sizeof(sections[i].physical_address) * 2);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "rva", sizeof(sections[i].virtual_address) * 2);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "rawsize", sizeof(sections[i].size_of_raw_data) * 2);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "pointer", sizeof(sections[i].pointer_to_raw_data) * 2);
  buffer_putnlflush(buffer_1);

  for(i = 0; i < header->coff_header.number_of_sections; i++) {
    const char* section_name = (const char*)sections[i].name;

    buffer_putspad(buffer_1, section_name, 16);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, sections[i].physical_address, sizeof(sections[i].physical_address) * 2);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, sections[i].virtual_address, sizeof(sections[i].virtual_address) * 2);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, sections[i].size_of_raw_data, sizeof(sections[i].size_of_raw_data) * 2);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, sections[i].pointer_to_raw_data, sizeof(sections[i].pointer_to_raw_data) * 2);
    buffer_putnlflush(buffer_1);
  }
}
