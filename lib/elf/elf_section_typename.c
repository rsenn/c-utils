#include "../elf.h"

const char*
elf_section_typename(uint32 sh_type) {
  static const char* const types[] = {
      "<0>", "PROGBITS", "SYMTAB", "STRTAB", "RELA", "HASH", "DYNAMIC", "NOTE", "NOBITS", "REL", "SHLIB", "DYNSYM", "INIT_ARRAY", "FINI_ARRAY", "PREINIT_ARRAY", "GROUP", "SYMTAB_SHNDX", "NUM"};
  return types[sh_type];
}
