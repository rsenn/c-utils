#include "../elf.h"

range
elf_symbol_r(void* elf, void* sym) {
  range r, t = elf_get_section_r(elf, ".text");
  
  r.start = t.start + ELF_GET(elf, sym, sym, st_value);
  r.elem_size = ELF_GET(elf, sym, sym, st_size);
  r.end = r.start + r.elem_size;

  return r;

}

