#include "../elf.h"

void*
elf_get_symtab(void* elf, size_t* szp) {
  int si = elf_section_index(elf, ELF_SHT_SYMTAB);
  void* r = NULL;
  size_t n = 0;
  if(si != -1) {
    n = elf_section_size(elf, si);
    r = elf_section_offset(elf, si);
  }
  if(szp)
    *szp = n;
  return r;
}
