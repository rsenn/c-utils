#include "../elf.h"
#include "../range.h"

range
elf_section_headers(void* elf) {
  range r;
  range_init(&r,
             (char*)elf + ELF_GET(elf, elf, ehdr, e_shoff),
             ELF_GET(elf, elf, ehdr, e_shnum),
             ELF_GET(elf, elf, ehdr, e_shentsize));
  return r;
}

