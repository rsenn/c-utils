#include "../elf.h"

size_t
elf_section_size(void* elf, int sn) {
  range r = elf_section_headers(elf);
  return ELF_GET(elf, range_index(&r, sn), shdr, sh_size);
}

