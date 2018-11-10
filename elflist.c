/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "lib/uint64.h"
#include "lib/buffer.h"
#include "lib/elf.h"
#include "lib/mmap.h"
#include "lib/str.h"
#include "lib/unix.h"
#include "lib/getopt.h"

void elf_dump_dynamic(char* base);
void elf_dump_sections(char* base);
void elf_dump_segments(char* base);
void elf_dump_symbols(char* base, range section, range text, const char* stname, int binding);
void elf_print_prefix(buffer* b);

static int list_defined, list_undefined;
static const char* filename;

void
elf_print_prefix(buffer* b) {
  if(filename)
    buffer_putm_internal(b, filename, ":", 0);
}

void
usage(char* av0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(av0),
                       " [OPTIONS] <file...>\n",
                       "\n",
                       "Options:\n",
                       "\n",
                       "  -h, --help              Show this help\n",
                       "  -D, --defined           List defined symbols\n",
                       "  -U, --undefined         List undefined symbols\n",
                       "\n",
                       0);
  buffer_flush(buffer_1);
}

int
main(int argc, char** argv) {
  char* base = NULL;
  size_t filesize;

  int c, index = 0;

  struct longopt opts[] = {{"help", 0, NULL, 'h'},
                           {"defined", 0, &list_defined, 'D'},
                           {"undefined", 0, &list_undefined, 'U'},
                           {0}};

  for(;;) {
    c = getopt_long(argc, argv, "hDU", opts, &index);
    if(c == -1)
      break;
    if(c == '\0')
      continue;

    switch(c) {
      case 'h': usage(argv[0]); return 0;
      case 'D': list_defined = 1; break;
      case 'U': list_undefined = 1; break;
      default: {
        usage(argv[0]);
        return 1;
      }
    }
  }

  if(!(list_defined | list_undefined))
    list_defined = list_undefined = 1;

  if(optind == argc) {
    buffer_putm_3(buffer_1, "Usage: ", str_basename(argv[0]), " XXX.dll\n");
    buffer_flush(buffer_1);
    return 0;
  }

  for(; argv[optind]; ++optind) {
    filename = argv[optind];

    base = (char*)mmap_private(filename, &filesize);

    const char* interp = elf_get_section(base, ".interp", NULL);

    elf_dump_sections(base);
    elf_dump_segments(base);
    elf_dump_dynamic(base);

    if(interp) {
      buffer_putm_2(buffer_1, "Interpreter: ", interp);
      buffer_putnlflush(buffer_1);
    }
    /*    elf_dump_imports(base);*/

    range symtab = elf_get_symtab_r(base);
    range text = elf_get_section_r(base, ".text");

    elf_dump_symbols(base, symtab, text, ".strtab", ELF_STB_GLOBAL);
    elf_dump_symbols(base, symtab, text, ".strtab", ELF_STB_LOCAL);

    mmap_unmap(base, filesize);
  }

  return 0;
}
void
elf_dump_dynamic(char* base) {

  int di = elf_section_find(base, ".dynamic");
  range dyn;
  void* entry;
  const char* dynstrtab = NULL;
  static const char* const dynamic_types[] = {"NULL",       "NEEDED",     "PLTRELSZ",      "PLTGOT",         "HASH",
                                              "STRTAB",     "SYMTAB",     "RELA",          "RELASZ",         "RELAENT",
                                              "STRSZ",      "SYMENT",     "INIT",          "FINI",           "SONAME",
                                              "RPATH",      "SYMBOLIC",   "REL",           "RELSZ",          "RELENT",
                                              "PLTREL",     "DEBUG",      "TEXTREL",       "JMPREL",         "BIND_NOW",
                                              "INIT_ARRAY", "FINI_ARRAY", "INIT_ARRAYSZ",  "FINI_ARRAYSZ",   "RUNPATH",
                                              "FLAGS",      "ENCODING",   "PREINIT_ARRAY", "PREINIT_ARRAYSZ"};

  if(di == -1)
    return;

  dyn = elf_dynamic_section(base);

  range_foreach(&dyn, entry) {
    int64 tag = ELF_GET(base, entry, dyn, d_tag);

    if(tag == ELF_DT_STRTAB) {
      dynstrtab = base + ELF_GET(base, entry, dyn, d_un.d_val);
      break;
    }

    if(tag == ELF_DT_NULL)
      break;
  }

  range_foreach(&dyn, entry) {
    int64 tag = ELF_GET(base, entry, dyn, d_tag);
    uint64 val = ELF_GET(base, entry, dyn, d_un.d_val);

    if(tag >= ELF_DT_NUM) {
      buffer_puts(buffer_1, "0x");
      buffer_putxint640(buffer_1, tag, ELF_BITS(base) / 4);
    } else {
      buffer_putspad(buffer_1, dynamic_types[tag % ELF_DT_NUM], 18);
    }

    if(tag == ELF_DT_NEEDED) {
      buffer_putspace(buffer_1);
      buffer_puts(buffer_1, (const char*)&dynstrtab[val]);
    } else {
      buffer_puts(buffer_1, " 0x");
      buffer_putxint640(buffer_1, val, ELF_BITS(base) / 4);
    }
    buffer_putnlflush(buffer_1);

    if(tag == ELF_DT_NULL)
      break;
  }
}

    extern int buffer_putptr_size_2;
void
elf_dump_symbols(char* base, range section, range text, const char* stname, int binding) {
  void* symbol;
  int si = elf_section_find(base, stname);
  const char* strtab = elf_section_offset(base, si);
  static const char* const binding_types[] = {"LOCAL", "GLOBAL", "WEAK"};
  static const char* const symbol_types[] = {"NOTYPE", "OBJECT", "FUNC", "SECTION", "FILE", "COMMON", "TLS"};
  range symtab = section;
  symtab.elem_size = ELF_BITS(base) == 64 ? sizeof(elf64_sym) : sizeof(elf32_sym);

/*  buffer_putspad(buffer_1, "symbol name", 33);
  buffer_putspad(buffer_1, "value", ELF_BITS(base) / 4 + 2 + 1);
  buffer_putspad(buffer_1, "size", ELF_BITS(base) / 4 + 2 + 1);
  if(binding < 0)
    buffer_putspad(buffer_1, "binding", 16);
  buffer_puts(buffer_1, "type");
  buffer_putnlflush(buffer_1);*/

  range_foreach(&symtab, symbol) {
    range code = elf_symbol_r(base, symbol);

    uint32 name = ELF_GET(base, symbol, sym, st_name);
    uint8 info = ELF_GET(base, symbol, sym, st_info);

    if(!strtab[name])
      continue;

    if(binding > -1 && ELF_ELF32_ST_BIND(info) != binding)
      continue;

    if(range_empty(&code)  && !list_undefined) continue; 
    if(!range_empty(&code)  && !list_defined) continue; 

    elf_print_prefix(buffer_1);
    
    buffer_putptr_size_2 = ELF_BITS(base) / 4;

    if(!range_empty(&code))
      buffer_putptr(buffer_1, code.start - base);
    else
      buffer_putnspace(buffer_1, buffer_putptr_size_2);
    
    if(ELF_ELF32_ST_BIND(info) == ELF_STB_GLOBAL) 
      buffer_puts(buffer_1, !range_empty(&code) ? " T " : " U ");
    else
      buffer_puts(buffer_1, !range_empty(&code) ? " t " : " u ");

    buffer_putspad(buffer_1, &(strtab[name]), 32);
    /*buffer_puts(buffer_1, "0x");
    buffer_putxlong0(buffer_1, name, 8);
    */
    /*jjif(size) {
      buffer_puts(buffer_1, " 0x");
      buffer_putxint640(buffer_1, value, ELF_BITS(base) / 4);
    } else {
      buffer_putnspace(buffer_1, ELF_BITS(base) / 4 + 3);
    }

    if(size) {
      buffer_puts(buffer_1, "   ");
      buffer_putulong0(buffer_1, size, ELF_BITS(base) / 4);
    } else {
      buffer_putnspace(buffer_1, ELF_BITS(base) / 4 + 3);
    }*/
    if(binding < 0) {
      buffer_putspace(buffer_1);
      buffer_putspad(buffer_1, binding_types[ELF_ELF32_ST_BIND(info)], 16);
    }
    buffer_putspace(buffer_1);
    buffer_puts(buffer_1, symbol_types[ELF_ELF32_ST_TYPE(info)]);

    buffer_putnlflush(buffer_1);
  }
}

void
elf_dump_sections(char* base) {
  int i, n;
  range sections = elf_section_headers(base);
  void* section;

  buffer_putspad(buffer_1, "section name", 16);
  buffer_putspace(buffer_1);
  buffer_putspad(buffer_1, "addr", ELF_BITS(base) / 4);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "size", ELF_BITS(base) / 4);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "offset", ELF_BITS(base) / 4);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "align", ELF_BITS(base) / 4);
  buffer_putnspace(buffer_1, 3);
  buffer_puts(buffer_1, "type");
  buffer_putnlflush(buffer_1);

  range_foreach(&sections, section) {
    uint32 name = ELF_GET(base, section, shdr, sh_name);
    uint64 addr = ELF_GET(base, section, shdr, sh_addr);
    uint64 size = ELF_GET(base, section, shdr, sh_size);
    uint64 offs = ELF_GET(base, section, shdr, sh_offset);
    uint64 align = ELF_GET(base, section, shdr, sh_addralign);
    uint32 type = ELF_GET(base, section, shdr, sh_type) % ELF_SHT_NUM;

    if(!name && !addr && !size)
      continue;

    buffer_putspad(buffer_1, &(elf_shstrtab(base)[name]), 16);
    buffer_puts(buffer_1, " 0x");
    buffer_putxint640(buffer_1, addr, ELF_BITS(base) / 4);
    buffer_puts(buffer_1, " 0x");
    buffer_putxint640(buffer_1, size, ELF_BITS(base) / 4);
    buffer_puts(buffer_1, " 0x");
    buffer_putxint640(buffer_1, offs, ELF_BITS(base) / 4);
    buffer_puts(buffer_1, " 0x");
    buffer_putxint640(buffer_1, align, ELF_BITS(base) / 4);
    buffer_putspace(buffer_1);
    buffer_puts(buffer_1, elf_section_typename(type));
    buffer_putnlflush(buffer_1);

    if(type == ELF_SHT_SYMTAB || type == ELF_SHT_DYNSYM) {
      //    elf_dump_symbols(base, elf_section(base, section), type == ELF_SHT_SYMTAB ? ".strtab" : ".dynstr");
    }
  }
}
void
elf_dump_segments(char* base) {
  int i, n;
  range segments = elf_program_headers(base);
  void* segment;

  if(range_size(&segments) == 0)
    return;

  buffer_putspad(buffer_1, "paddr", ELF_BITS(base) / 4);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "vaddr", ELF_BITS(base) / 4);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "filesz", ELF_BITS(base) / 4);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "memsz", ELF_BITS(base) / 4);
  buffer_putnspace(buffer_1, 3);
  buffer_puts(buffer_1, "flags");
  buffer_putnlflush(buffer_1);

  range_foreach(&segments, segment) {
    uint64 paddr = ELF_GET(base, segment, phdr, p_paddr);
    uint64 vaddr = ELF_GET(base, segment, phdr, p_vaddr);
    uint64 filesz = ELF_GET(base, segment, phdr, p_filesz);
    uint64 memsz = ELF_GET(base, segment, phdr, p_memsz);
    uint32 flags = ELF_GET(base, segment, phdr, p_flags);

    if(!paddr && !vaddr && !filesz)
      continue;

    buffer_puts(buffer_1, "0x");
    buffer_putxint640(buffer_1, paddr, ELF_BITS(base) / 4);
    buffer_puts(buffer_1, " 0x");
    buffer_putxint640(buffer_1, vaddr, ELF_BITS(base) / 4);
    buffer_puts(buffer_1, " 0x");
    buffer_putxint640(buffer_1, filesz, ELF_BITS(base) / 4);
    buffer_puts(buffer_1, " 0x");
    buffer_putxint640(buffer_1, memsz, ELF_BITS(base) / 4);
    buffer_putm_internal(buffer_1,
                         " ",
                         (flags & ELF_PF_R) ? "r" : "-",
                         (flags & ELF_PF_W) ? "w" : "-",
                         (flags & ELF_PF_W) ? "x" : "-",
                         0);
    buffer_putnlflush(buffer_1);
  }
}
