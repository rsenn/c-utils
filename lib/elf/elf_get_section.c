#include "../elf.h"

void*
elf_get_section(void* elf, const char* name, size_t* szp) {
  int si = elf_section_find(elf, name);

  if(si != -1) {
    if(szp)
      *szp = elf_section_size(elf, si);
    return elf_section_offset(elf, si);
  } else {
    if(szp)
      *szp = 0;
  }
  return 0;
}
