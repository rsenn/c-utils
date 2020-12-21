#include "../elf.h"

const char*
elf_shstrtab(void* elf) {
  void* shdr = (char*)elf + ELF_GET(elf, elf, ehdr, e_shoff);
  void* sh_strtab = (char*)shdr + ELF_GET(elf, elf, ehdr, e_shstrndx) *
                                      ELF_GET(elf, elf, ehdr, e_shentsize);
  return (char*)elf + ELF_GET(elf, sh_strtab, shdr, sh_offset);
}
