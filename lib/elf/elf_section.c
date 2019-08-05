#include "../elf.h"
#include "../range.h"

range
elf_section(void* elf, void* shdr) {
  range r;
  r.start = (char*)elf + ELF_GET(elf, shdr, shdr, sh_offset);
  r.end = r.start + ELF_GET(elf, shdr, shdr, sh_size);
  r.elem_size = 1;
  return r;
}

