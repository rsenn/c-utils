#include "../elf.h"

const char*
elf_strtab(void *elf) {
  void* shdr = (char*)elf + ELF_GET(elf, ehdr, e_shoff);
  void* sh_strtab = (char*)shdr + ELF_GET(elf, ehdr, e_shstrndx) * ELF_GET(elf, ehdr, e_shentsize);
  return (char*)elf + ELF_GET(sh_strtab, shdr, sh_offset);
}

