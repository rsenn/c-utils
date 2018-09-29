/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "lib/elf.h"
#include "lib/uint16.h"
#include "lib/uint32.h"
#include "lib/uint64.h"
#include <stdio.h>
#include <string.h>

int
main(int argc, char** argv) {
  char* content;
  size_t length;
  Elf64_Shdr* dynstr = NULL;
  Elf64_Shdr* dynsym = NULL;
  uint16 text_section_header_index = 0;

  if(argc < 2) {
    g_print("Usage: %s XXX.so\n", argv[0]);
    return 0;
  }

  g_file_get_contents(argv[1], &content, &length, NULL);

  {
    Elf64_Ehdr* header;
    size_t section_offset;
    uint16 section_header_size;
    uint16 i, n_headers;
    Elf64_Shdr* section_name_header;
    size_t section_name_header_offset;
    const gchar* section_names;

    header = (Elf64_Ehdr*)content;
    if(memcmp(header->e_ident, ELFMAG, SELFMAG) != 0) {
      g_print("not ELF\n");
      return -1;
    }

    header = (Elf64_Ehdr*)content;
    if(header->e_type != ELF_ET_DYN) {
      g_print("not shared library\n");
      return -1;
    }

    section_offset = header->e_shoff;
    section_header_size = header->e_shentsize;
    n_headers = header->e_shnum;

    section_name_header_offset = header->e_shoff + (header->e_shstrndx * header->e_shentsize);
    section_name_header = (Elf64_Shdr*)(content + section_name_header_offset);
    section_names = content + section_name_header->sh_offset;

    for(i = 0; i < n_headers; i++) {
      Elf64_Shdr* section_header = NULL;
      size_t offset;
      const gchar* section_name;

      offset = section_offset + (section_header_size * i);
      section_header = (Elf64_Shdr*)(content + offset);
      section_name = section_names + section_header->sh_name;

      if(g_str_equal(section_name, ".dynstr")) {
        dynstr = section_header;
      } else if(g_str_equal(section_name, ".dynsym")) {
        dynsym = section_header;
      } else if(g_str_equal(section_name, ".text")) {
        text_section_header_index = i;
      }
    }

    if(!dynsym) {
      g_print(".dynsym section is not found\n");
      return -1;
    }

    if(!dynstr) {
      g_print(".dynstr section is not found\n");
      return -1;
    }

    if(text_section_header_index == 0) {
      g_print(".text section is not found\n");
      return -1;
    }
  }

  {
    guint i, n_entries;
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
      Elf64_Sym* symbol;
      uint64 name_index;
      unsigned char info;
      uint16 section_header_index;
      size_t offset;

      offset = symbol_section_offset + (i * symbol_entry_size);
      symbol = (Elf64_Sym*)(content + offset);
      name_index = symbol->st_name;
      info = symbol->st_info;
      section_header_index = symbol->st_shndx;

      if((info & STT_FUNC) && (ELF_ELF64_ST_BIND(info) & STB_GLOBAL) &&
         (section_header_index == text_section_header_index)) {
        const gchar* name;

        name = content + name_section_offset + name_index;
        g_print("found: %s\n", name);
      }
    }
  }

  g_free(content);

  return 0;
}
