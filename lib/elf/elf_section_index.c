#include "../elf.h"
#include "../range.h"

int
elf_section_index(void* elf, uint32 sh_type) {
  range r = elf_section_headers(elf);
  void* section;
  uint32 i = 0;

  range_foreach(&r, section) {
    uint32 type = uint32_get(ELF_ADDR(elf, section, shdr, sh_type));
    
    if(type == sh_type)  return i;
    ++i;
  }
  return -1;
}

