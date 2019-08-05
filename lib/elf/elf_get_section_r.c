#include "../elf.h"

range
elf_get_section_r(void* elf, const char* name) {
  range r;
  r.start = elf_get_section(elf, name, &r.elem_size);
  r.end = r.start + r.elem_size;
  return r;
}

