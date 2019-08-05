#include "../elf.h"

range
elf_get_symtab_r(void* elf) {
  range r;
  r.start = elf_get_symtab(elf, &r.elem_size);
  r.end = r.start + r.elem_size;
  r.elem_size = ELF_STRUCT_SIZE(elf, sym);
  return r;
}

