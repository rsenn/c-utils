#include "../elf.h"
#include "../range.h"
#include "../str.h"

int
elf_section_index(void* elf, const char* sname) {
  range r = elf_section_headers(elf);
  void* section;
  uint32 i = 0;

  range_foreach(&r, section) {
    uint32 name = ELF_GET(elf, section, shdr, sh_name);

    if(str_equal(sname, &(elf_shstrtab(elf)[name])))
      return i;
    ++i;
  }
  return -1;
}
