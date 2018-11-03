#include "../elf.h"

const char*
elf_section_type(int i) {
  static const char* const types[] = {"<0>",
                                      "PROGBITS",
                                      "SYMTAB",
                                      "STRTAB",
                                      "RELA",
                                      "HASH",
                                      "DYNAMIC",
                                      "NOTE",
                                      "NOBITS",
                                      "REL",
                                      "SHLIB",
                                      "DYNSYM",
                                      "INIT_ARRAY",
                                      "FINI_ARRAY",
                                      "PREINIT_ARRAY",
                                      "GROUP",
                                      "SYMTAB_SHNDX",
                                      "NUM"};
  return types[i];
}
