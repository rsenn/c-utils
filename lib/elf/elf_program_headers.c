#include "../elf.h"
#include "../range.h"

range
elf_program_headers(void* elf) {
  range r;
  range_init(&r, (char*)elf + ELF_GET(elf, elf, ehdr, e_phoff), ELF_GET(elf, elf, ehdr, e_phnum), ELF_GET(elf, elf, ehdr, e_phentsize));
  return r;
}
