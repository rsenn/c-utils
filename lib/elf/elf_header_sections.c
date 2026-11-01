#include "../elf.h"

void*
elf_header_sections(void* elf) {
  uint8* base = elf;

  switch(base[ELF_EI_CLASS]) {
    case ELF_ELFCLASS32: {
      elf32_ehdr* ehdr = elf;
      return (uint8*)ehdr + ehdr->e_shoff;
    }
    case ELF_ELFCLASS64: {
      elf64_ehdr* ehdr = elf;
      return (uint8*)ehdr + ehdr->e_shoff;
    }
  }
  return 0;
}
