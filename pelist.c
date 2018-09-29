/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "lib/buffer.h"
#include "lib/mmap.h"
#include "lib/pe.h"
#include "lib/str.h"
#include <stdlib.h>
#include <string.h>

void pe_dump_sections(uint8* content);

void*
pe_rva2ptr(void* base, uint32 rva) {
  int64 off = pe_rva2offset(base, rva);
  if(off == -1) return NULL;
  return (uint8*)base + off;
}

uint32
pe_offset2rva(uint8* base, int64 off) {
  int i, n;
  pe_section_header* sections = pe_header_sections(base, &n);

  if((uint32)off < sections[0].pointer_to_raw_data) return off;

  for(i = 0; i < n; i++) {
    uint32 start = sections[i].pointer_to_raw_data;
    uint32 end = start + sections[i].size_of_raw_data;

    if((uint32)off >= start && (uint32)off < end) return (uint32)off - start + sections[i].virtual_address;
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
pe_dump_exports(uint8* content) {
  int i;
  pe_data_directory* export_dir = &pe_header_datadir(content)[PE_DIRECTORY_ENTRY_EXPORT];
  pe_section_header* text;
  uint32 fnaddr, *nameptr, *fnptr, mintextptr, maxtextptr;

  pe_export_directory* exports = pe_rva2ptr(content, export_dir->virtual_address);
  // pe_print_export_directory(buffer_1, exports);

  if(!(text = pe_get_section(content, ".text"))) {
    buffer_putsflush(buffer_2, "no .text section\n");
    return;
  }

  nameptr = (uint32*)pe_rva2ptr(content, exports->address_of_names);
  fnptr = (uint32*)pe_rva2ptr(content, exports->address_of_functions);
  mintextptr = text->virtual_address;
  maxtextptr = mintextptr + text->size_of_raw_data;

  for(i = 0; i < exports->number_of_names; i++) {
    fnaddr = fnptr[i];
    if(mintextptr < fnaddr && fnaddr < maxtextptr) {
      buffer_puts(buffer_1, pe_rva2ptr(content, nameptr[i]));
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
    pe32_nt_headers* nt_headers = pe_header_nt(content);

    if(nt_headers->signature != PE_NT_SIGNATURE) {
      buffer_putsflush(buffer_2, "not PE\n");
      return -1;
    }

    if(!(nt_headers->coff_header.characteristics & PE_FILE_DLL)) {
      buffer_putsflush(buffer_2, "not DLL\n");
      return -1;
    }
    // pe_dump_sections(content);

    pe_dump_exports(content);
  }

  mmap_unmap(content, filesize);

  return 0;
}

void
pe_dump_sections(uint8* content) {
  int i, n;
  pe_section_header* sections = pe_header_sections(content, &n);

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

  for(i = 0; i < n; i++) {
    buffer_putspad(buffer_1, sections[i].name, 16);
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
