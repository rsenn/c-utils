/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <stdio.h>
#include <string.h>
#include <windows.h>

int
main(int argc, char** argv) {
  char* content = NULL;
  IMAGE_NT_HEADERS* nt_headers;
  IMAGE_SECTION_HEADER* text_section = NULL;
  IMAGE_SECTION_HEADER* edata_section = NULL;

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
    IMAGE_DOS_HEADER* dos_header;

    dos_header = (IMAGE_DOS_HEADER*)content;
    nt_headers = (IMAGE_NT_HEADERS*)(content + dos_header->e_lfanew);
    if(nt_headers->Signature != IMAGE_NT_SIGNATURE) {
      printf("not PE\n");
      return -1;
    }

    if(!(nt_headers->FileHeader.Characteristics & IMAGE_FILE_DLL)) {
      printf("not DLL\n");
      return -1;
    }
  }

  {
    WORD i;
    IMAGE_SECTION_HEADER* first_section;

    first_section = IMAGE_FIRST_SECTION(nt_headers);
    for(i = 0; i < nt_headers->FileHeader.NumberOfSections; i++) {
      const char* section_name;

      section_name = (const char*)((first_section + i)->Name);
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
    WORD i;
    IMAGE_EXPORT_DIRECTORY* export_directory;
    const char* base_address;
    ULONG* name_addresses;
    ULONG* function_addresses;
    DWORD min_text_section_address, max_text_section_address;

    export_directory = (IMAGE_EXPORT_DIRECTORY*)(content + edata_section->PointerToRawData);

    base_address = content + edata_section->PointerToRawData - edata_section->VirtualAddress;
    name_addresses = (ULONG*)(base_address + export_directory->AddressOfNames);
    function_addresses = (ULONG*)(base_address + export_directory->AddressOfFunctions);
    min_text_section_address = text_section->VirtualAddress;
    max_text_section_address = min_text_section_address + text_section->SizeOfRawData;
    for(i = 0; i < export_directory->NumberOfNames; i++) {
      const char* name;
      DWORD function_address;

      name = base_address + name_addresses[i];
      function_address = function_addresses[i];
      if(min_text_section_address < function_address && function_address < max_text_section_address)
        printf("found: %s\n", name);
    }
  }

  free(content);

  return 0;
}
