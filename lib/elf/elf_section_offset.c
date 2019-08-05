#include "../elf.h"

void*
elf_section_offset(void* elf, int sn) {
  range r = elf_section_headers(elf);
  return (char*)elf + ELF_GET(elf, range_index(&r, sn), shdr, sh_offset);
}

