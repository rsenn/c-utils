#include "../elf.h"
#include "../range.h"
#include "../byte.h"

range
elf_dynamic_section(void* elf) {
  range dyn;
  int di = elf_section_index(elf, ".dynamic");
  byte_zero(&dyn, sizeof(range));
  if(di != -1) {
    dyn.start = elf_section_offset(elf, di);
    dyn.end = dyn.start + elf_section_size(elf, di);
    dyn.elem_size = ELF_BITS(elf) == 64 ? sizeof(elf64_dyn) : sizeof(elf32_dyn);
  }
  return dyn;
}
