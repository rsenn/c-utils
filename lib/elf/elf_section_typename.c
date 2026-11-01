#include "../elf.h"

const char*
elf_section_typename(uint32 sh_type) {
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
  /* a corrupt file can carry any type; the table used to be indexed unchecked */
  if(sh_type >= sizeof(types) / sizeof(types[0]))
    return "<unknown>";
  return types[sh_type];
}
