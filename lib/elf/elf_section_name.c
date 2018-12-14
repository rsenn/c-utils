#include "../elf.h"

const char*
elf_section_name(void* elf, int sn) {
  range r = elf_section_headers(elf);
  uint32 idx = ELF_GET(elf, range_index(&r, sn), shdr, sh_name);
  const char* name = &(elf_shstrtab(elf)[idx]);
  return name;
}
