/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "lib/elf.h"
#include "lib/mmap.h"
#include "lib/uint16.h"
#include "lib/uint32.h"
#include "lib/uint64.h"
#include <stdio.h>
#include <string.h>

int
main(int argc, char** argv) {
  char* content;
  size_t length;
  elf64_shdr* dynstr = NULL;
  elf64_shdr* dynsym = NULL;
  uint16 text_section_header_index = 0;

  if(argc < 2) {
    printf("Usage: %s XXX.so\n", argv[0]);
    return 0;
  }

  content = mmap_private(argv[1], &length);

  {
    elf64_ehdr* header;
    size_t section_offset;
    uint16 section_header_size;
    uint16 i, n_headers;
    elf64_shdr* section_name_header;
    size_t section_name_header_offset;
    const char* section_names;

    header = (elf64_ehdr*)content;
    if(memcmp(header->e_ident, ELF_ELFMAG, ELF_SELFMAG) != 0) {
      printf("not ELF\n");
      return -1;
    }

    header = (elf64_ehdr*)content;
    if(header->e_type != ELF_ET_DYN) {
      printf("not shared library\n");
      return -1;
    }

    section_offset = header->e_shoff;
    section_header_size = header->e_shentsize;
    n_headers = header->e_shnum;

    section_name_header_offset = header->e_shoff + (header->e_shstrndx * header->e_shentsize);
    section_name_header = (elf64_shdr*)(content + section_name_header_offset);
    section_names = content + section_name_header->sh_offset;

    for(i = 0; i < n_headers; i++) {
      elf64_shdr* section_header = NULL;
      size_t offset;
      const char* section_name;

      offset = section_offset + (section_header_size * i);
      section_header = (elf64_shdr*)(content + offset);
      section_name = section_names + section_header->sh_name;

      if(!strcmp(section_name, ".dynstr")) {
        dynstr = section_header;
      } else if(!strcmp(section_name, ".dynsym")) {
        dynsym = section_header;
      } else if(!strcmp(section_name, ".text")) {
        text_section_header_index = i;
      }
    }

    if(!dynsym) {
      printf(".dynsym section is not found\n");
      return -1;
    }

    if(!dynstr) {
      printf(".dynstr section is not found\n");
      return -1;
    }

    if(text_section_header_index == 0) {
      printf(".text section is not found\n");
      return -1;
    }
  }

  {
    size_t i, n_entries;
    size_t symbol_section_offset;
    size_t symbol_entry_size;
    size_t name_section_offset;

    symbol_section_offset = dynsym->sh_offset;
    symbol_entry_size = dynsym->sh_entsize;
    name_section_offset = dynstr->sh_offset;
    if(symbol_entry_size > 0)
      n_entries = dynsym->sh_size / symbol_entry_size;
    else
      n_entries = 0;

    for(i = 0; i < n_entries; i++) {
      elf64_sym* symbol;
      uint64 name_index;
      unsigned char info;
      uint16 section_header_index;
      size_t offset;

      offset = symbol_section_offset + (i * symbol_entry_size);
      symbol = (elf64_sym*)(content + offset);
      name_index = symbol->st_name;
      info = symbol->st_info;
      section_header_index = symbol->st_shndx;

      if((info & ELF_STT_FUNC) && (ELF_ELF64_ST_BIND(info) & ELF_STB_GLOBAL) &&
         (section_header_index == text_section_header_index)) {
        const char* name;

        name = content + name_section_offset + name_index;
        printf("found: %s\n", name);
      }
    }
  }

  free(content);

  return 0;
}
