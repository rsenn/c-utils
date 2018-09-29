/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "lib/buffer.h"
#include "lib/mmap.h"
#include "lib/pe.h"
#include <stdlib.h>
#include <string.h>

int
main(int argc, char** argv) {
  char* content = NULL;
  size_t filesize;

  if(argc < 2) {
    buffer_putm(buffer_1, "Usage: ", str_basename(argv[0]), " XXX.dll\n");
    buffer_flush(buffer_1);
    return 0;
  }

  content = mmap_private(argv[1], &filesize);

  {
    pe_dos_header* dos_header;
    pe32_nt_headers* nt_headers;
    pe_section_header* text_section = NULL;
    pe_section_header* edata_section = NULL;

    dos_header = (pe_dos_header*)content;
    nt_headers = (pe32_nt_headers*)(content + dos_header->e_lfanew);
    if(nt_headers->signature != PE_NT_SIGNATURE) {
      buffer_putsflush(buffer_2, "not PE\n");
      return -1;
    }

    if(!(nt_headers->file_header.characteristics & PE_FILE_DLL)) {
      buffer_putsflush(buffer_2, "not DLL\n");
      return -1;
    }

    {
      int i;
      pe_section_header* first_section;

      first_section = PE32_FIRST_SECTION(nt_headers);
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
        buffer_putsflush(buffer_2, "no .text section\n");
        return -1;
      }

      if(!edata_section) {
        buffer_putsflush(buffer_2, "no .edata section\n");
        return -1;
      }

      {
        int i;
        pe_export_directory* export_directory;
        const char* base_address;
        uint32* name_addresses;
        uint32* function_addresses;
        uint32 min_text_section_address, max_text_section_address;

        export_directory = (pe_export_directory*)(content + edata_section->pointer_to_raw_data);

        base_address = content + edata_section->pointer_to_raw_data - edata_section->virtual_address;
        name_addresses = (uint32*)(base_address + export_directory->address_of_names);
        function_addresses = (uint32*)(base_address + export_directory->address_of_functions);
        min_text_section_address = text_section->virtual_address;
        max_text_section_address = min_text_section_address + text_section->size_of_raw_data;

        for(i = 0; i < export_directory->number_of_names; i++) {
          const char* name;
          uint32 function_address;

          name = base_address + name_addresses[i];
          function_address = function_addresses[i];

          if(min_text_section_address < function_address && function_address < max_text_section_address) {
            buffer_puts(buffer_1, name);
            buffer_putnlflush(buffer_1);
          }
        }
      }
    }
  }

  mmap_unmap(content, filesize);

  return 0;
}
