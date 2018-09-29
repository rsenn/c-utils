/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/pe.h"

int
main(int argc, char** argv) {
  char* content = NULL;
  pe32_nt_headers* nt_headers;
  pe_section_header* text_section = NULL;
  pe_section_header* edata_section = NULL;

  if(argc < 2) {
    printf("Usage: %s XXX.dll\n", argv[0]);
    return 0;
  }

  {
    FILE* file;
    char buffer[4096];
    size_t bytes, total_bytes = 0;

    file = fopen(argv[1], "rb");
    if(!file) {
      perror("failed to fopen()");
      return -1;
    }
    while(!feof(file)) {
      char* original_content;

      bytes = fread(buffer, 1, sizeof(buffer), file);
      total_bytes += bytes;
      original_content = content;
      content = realloc(content, total_bytes);
      if(!content) {
        free(original_content);
        fclose(file);
        perror("failed to realloc()");
        return -1;
      }
      memcpy(content + total_bytes - bytes, buffer, bytes);
    }
    fclose(file);
  }

  {
    pe_dos_header* dos_header;

    dos_header = (pe_dos_header*)content;
    nt_headers = (pe32_nt_headers*)(content + dos_header->e_lfanew);
    if(nt_headers->signature != PE_NT_SIGNATURE) {
      printf("not PE\n");
      return -1;
    }

    if(!(nt_headers->file_header.characteristics & PE_FILE_DLL)) {
      printf("not DLL\n");
      return -1;
    }
  }

  {
    int i;
    pe_section_header* first_section;

    first_section = PE_FIRST_SECTION(nt_headers);
    for(i = 0; i < nt_headers->file_header.number_of_sections; i++) {
      const char* section_name;

      section_name = (const char*)((first_section + i)->name);
      if(strcmp(".edata", section_name) == 0) {
        edata_section = first_section + i;
      } else if(strcmp(".text", section_name) == 0) {
        text_section = first_section + i;
      }
    }

    if(!text_section) {
      printf("no .text section\n");
      return -1;
    }

    if(!edata_section) {
      printf("no .edata section\n");
      return -1;
    }
  }

  {
    int i;
    pe_export_directory* export_directory;
    const char* base_address;
    uint64* name_addresses;
    uint64* function_addresses;
    uint64 min_text_section_address, max_text_section_address;

    export_directory = (pe_export_directory*)(content + edata_section->pointer_to_raw_data);

    base_address = content + edata_section->pointer_to_raw_data - edata_section->virtual_address;
    name_addresses = (uint64*)(base_address + export_directory->address_of_names);
    function_addresses = (uint64*)(base_address + export_directory->address_of_functions);
    min_text_section_address = text_section->virtual_address;
    max_text_section_address = min_text_section_address + text_section->size_of_raw_data;
    for(i = 0; i < export_directory->number_of_names; i++) {
      const char* name;
      uint64 function_address;

      name = base_address + name_addresses[i];
      function_address = function_addresses[i];
      if(min_text_section_address < function_address && function_address < max_text_section_address)
        printf("found: %s\n", name);
    }
  }

  free(content);

  return 0;
}
