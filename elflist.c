/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "lib/uint64.h"
#include "lib/buffer.h"
#include "lib/elf.h"
#include "lib/mmap.h"
#include "lib/str.h"
#include "lib/unix.h"
#include "lib/getopt.h"
#include "lib/range.h"
#include "lib/rangecheck.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/strlist.h"
#include <stdlib.h>

void elf_dump_dynamic(range map);
void elf_dump_sections(range map);
void elf_dump_segments(range map);
void elf_dump_symbols(range map, range section, range text, const char* stname, int binding);
void elf_print_prefix(buffer* b);

static int list_defined, list_undefined;
static const char* filename;

#define RANGE_CHECK(ptr)                                                                                               \
  do {                                                                                                                 \
    if(!range_ptrinbuf2(map.start, map.end, ptr)) {                                                                    \
      buffer_puts(buffer_2, "OUT of range: " #ptr);                                                                    \
      buffer_putnlflush(buffer_2);                                                                                     \
      exit(120);                                                                                                       \
    }                                                                                                                  \
  } while(0)

#define ELF_DUMP_FIELD(base, ptr, st, field) \
  buffer_putspad(b, #field, 30), \
  buffer_puts(b, " 0x"), \
  buffer_putxlonglong0(b, ELF_GET(base, ptr, st, field), ELF_SIZE(base, st, field) * 2), \
  buffer_putnlflush(b)

void
elf_print_prefix(buffer* b) {
  if(filename)
    buffer_putm_internal(b, filename, ":", 0);
}

#define MACHINES "NONE\0M32\0SPARC\0386\068K\088K\0860\0MIPS\0S370\0PARISC\0VPP500\0SPARC32PLUS\0960\0PPC\0PPC64\0S390\0V800\0FR20\0RH32\0RCE\0ARM\0FAKE_ALPHA\0SH\0SPARCV9\0TRICORE\0ARC\0H8_300\0H8_300H\0H8S\0H8_500\0IA_64\0COLDFIRE\068HC12\0MMA\0PCP\0NCPU\0NDR1\0STARCORE\0ME16\0ST100\0TINYJ\0X86_64\0PDSP\0FX66\0ST9PLUS\0ST7\068HC16\068HC11\068HC08\068HC05\0SVX\0ST19\0VAX\0CRIS\0JAVELIN\0FIREPATH\0ZSP\0MMIX\0HUANY\0PRISM\0AVR\0FR30\0D10V\0D30V\0V850\0M32R\0MN10300\0MN10200\0PJ\0OPENRISC\0ARC_A5\0XTENSA\0ALPHA"
#define TYPES "NONE\0REL\0EXEC\0DYN\0CORE\0"

void
elf_dump_header(buffer* b, range map) {
  static strlist machines, types;
  stralloc_copyb(&machines.sa, MACHINES, sizeof(MACHINES));
 stralloc_copyb(&types.sa, TYPES, sizeof(TYPES));

  ELF_DUMP_FIELD(map.start, map.start, ehdr, e_ident);
  ELF_DUMP_FIELD(map.start, map.start, ehdr, e_type);
  ELF_DUMP_FIELD(map.start, map.start, ehdr, e_machine);
  ELF_DUMP_FIELD(map.start, map.start, ehdr, e_version);
  ELF_DUMP_FIELD(map.start, map.start, ehdr, e_entry);
  ELF_DUMP_FIELD(map.start, map.start, ehdr, e_phoff);
  ELF_DUMP_FIELD(map.start, map.start, ehdr, e_shoff);
  ELF_DUMP_FIELD(map.start, map.start, ehdr, e_flags);
  ELF_DUMP_FIELD(map.start, map.start, ehdr, e_ehsize);
  ELF_DUMP_FIELD(map.start, map.start, ehdr, e_phentsize);
  ELF_DUMP_FIELD(map.start, map.start, ehdr, e_phnum);
  ELF_DUMP_FIELD(map.start, map.start, ehdr, e_shentsize);
  ELF_DUMP_FIELD(map.start, map.start, ehdr, e_shnum);
  ELF_DUMP_FIELD(map.start, map.start, ehdr, e_shstrndx);

}

/**
 * @brief usage  Show command line usage
 * @param av0
 */
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
                       "  -F, --file-header       Dump file header\n",
                       "\n",
                       0);
  buffer_flush(buffer_1);
}

int
main(int argc, char** argv) {
  static range map;
  size_t filesize;
  static int dump_file_header = 0;

  int c, index = 0;

  struct longopt opts[] = {{"help", 0, NULL, 'h'},
                           {"defined", 0, &list_defined, 'D'},
                           {"undefined", 0, &list_undefined, 'U'},
  {"file-header", 0, 0, 'F'},
                           {0}};

  for(;;) {
    c = getopt_long(argc, argv, "hDUF", opts, &index);
    if(c == -1)
      break;
    if(c == '\0')
      continue;

    switch(c) {
      case 'h': usage(argv[0]); return 0;
      case 'D': list_defined = 1; break;
      case 'U': list_undefined = 1; break;
      case 'F': dump_file_header = 1; break;
      default: {
        usage(argv[0]);
        return 1;
      }
    }
  }

  if(!(list_defined | list_undefined))
    list_defined = list_undefined = 1;

  if(optind == argc) {
    usage(argv[0]);
    return 0;
  }

  for(; argv[optind]; ++optind) {
    const char* interp;
    range symtab, text;

    filename = argv[optind];

    map.start = (char*)mmap_read(filename, &filesize);
    map.end = map.start + filesize;

    buffer_puts(buffer_2, "map start: ");
    buffer_putptr(buffer_2, map.start);
    buffer_putnlflush(buffer_2);

    if(dump_file_header)
      elf_dump_header(buffer_1, map);

    interp = elf_get_section(map.start, ".interp", NULL);

    elf_dump_sections(map);
    elf_dump_segments(map);
    elf_dump_dynamic(map);

    if(interp) {
      buffer_putm_2(buffer_1, "Interpreter: ", interp);
      buffer_putnlflush(buffer_1);
    }
    /*    elf_dump_imports(map.start);*/

    symtab = elf_get_symtab_r(map.start);
    text = elf_get_section_r(map.start, ".text");

    elf_dump_symbols(map, symtab, text, ".strtab", ELF_STB_GLOBAL);
    elf_dump_symbols(map, symtab, text, ".strtab", ELF_STB_LOCAL);

    mmap_unmap(map.start, map.end - map.start);
  }

  return 0;
}

/**
 * @brief elf_dump_dynamic  Dumps all ELF dynamic entries
 * @param map               Pointer range of the loaded ELF file
 */
void
elf_dump_dynamic(range map) {

  int i = 0, di = elf_section_find(map.start, ".dynamic");
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

  dyn = elf_dynamic_section(map.start);

  range_foreach(&dyn, entry) {
    int64 tag = ELF_GET(map.start, entry, dyn, d_tag);
    uint64 val = ELF_GET(map.start, entry, dyn, d_un.d_val);

    if(tag == ELF_DT_STRTAB) {
      buffer_puts(buffer_2, "ELF_DT_STRTAB:\n0x");
      buffer_putxlonglong0(buffer_2, val, ELF_BITS(map.start) / 4);
      buffer_putnlflush(buffer_2);
      //   buffer_puts(buffer_2, elf_)

      dynstrtab = map.start + val;

      RANGE_CHECK(dynstrtab);

      break;
    }

    if(tag == ELF_DT_NULL)
      break;
  }

  range_foreach(&dyn, entry) {
    int64 tag = ELF_GET(map.start, entry, dyn, d_tag);
    uint64 val = ELF_GET(map.start, entry, dyn, d_un.d_val);

    if(tag >= ELF_DT_NUM) {
      buffer_puts(buffer_1, "0x");
      buffer_putxlonglong0(buffer_1, tag, ELF_BITS(map.start) / 4);
    } else {
      buffer_putspad(buffer_1, dynamic_types[tag % ELF_DT_NUM], 18);
    }

    if(tag == ELF_DT_NEEDED) {
      buffer_putspace(buffer_1);
      buffer_puts(buffer_1, (const char*)&dynstrtab[val]);
    } else {
      buffer_puts(buffer_1, " 0x");
      buffer_putxlonglong0(buffer_1, val, ELF_BITS(map.start) / 4);
    }
    buffer_putnlflush(buffer_1);

    if(tag == ELF_DT_NULL)
      break;
  }
}

extern int buffer_putptr_size_2;

/**
 * @brief elf_dump_symbols
 * @param map               Pointer range of the loaded ELF file
 * @param section           Symbol table section
 * @param text              Code section
 * @param stname            String-table section name
 * @param binding
 */
void
elf_dump_symbols(range map, range section, range text, const char* stname, int binding) {
  void* symbol;
  int si = elf_section_find(map.start, stname);
  const char* strtab = elf_section_offset(map.start, si);
  static const char* const binding_types[] = {"LOCAL", "GLOBAL", "WEAK"};
  static const char* const symbol_types[] = {"NOTYPE", "OBJECT", "FUNC", "SECTION", "FILE", "COMMON", "TLS"};
  range symtab = section;
  symtab.elem_size = ELF_BITS(map.start) == 64 ? sizeof(elf64_sym) : sizeof(elf32_sym);

  /*  buffer_putspad(buffer_1, "symbol name", 33);
    buffer_putspad(buffer_1, "value", ELF_BITS(map.start) / 4 + 2 + 1);
    buffer_putspad(buffer_1, "size", ELF_BITS(map.start) / 4 + 2 + 1);
    if(binding < 0)
      buffer_putspad(buffer_1, "binding", 16);
    buffer_puts(buffer_1, "type");
    buffer_putnlflush(buffer_1);*/

  range_foreach(&symtab, symbol) {
    range code = elf_symbol_r(map.start, symbol);

    uint32 name = ELF_GET(map.start, symbol, sym, st_name);
    uint8 info = ELF_GET(map.start, symbol, sym, st_info);

    if(!strtab[name])
      continue;

    if(binding > -1 && ELF_ELF32_ST_BIND(info) != binding)
      continue;

    if(range_empty(&code) && !list_undefined)
      continue;
    if(!range_empty(&code) && !list_defined)
      continue;

    elf_print_prefix(buffer_1);

    buffer_putptr_size_2 = ELF_BITS(map.start) / 4;

    if(!range_empty(&code))
      buffer_putptr(buffer_1, (char*)(ptrdiff_t)(code.start - map.start));
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
      buffer_putxlonglong0(buffer_1, value, ELF_BITS(map.start) / 4);
    } else {
      buffer_putnspace(buffer_1, ELF_BITS(map.start) / 4 + 3);
    }

    if(size) {
      buffer_puts(buffer_1, "   ");
      buffer_putulong0(buffer_1, size, ELF_BITS(map.start) / 4);
    } else {
      buffer_putnspace(buffer_1, ELF_BITS(map.start) / 4 + 3);
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

/**
 * @brief elf_dump_sections
 * @param map               Pointer range of the loaded ELF file
 */
void
elf_dump_sections(range map) {
  int i, n;
  range sections = elf_section_headers(map.start);
  void* section;

  buffer_putspad(buffer_1, "section name", 16);
  buffer_putspace(buffer_1);
  buffer_putspad(buffer_1, "addr", ELF_BITS(map.start) / 4);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "size", ELF_BITS(map.start) / 4);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "offset", ELF_BITS(map.start) / 4);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "align", ELF_BITS(map.start) / 4);
  buffer_putnspace(buffer_1, 3);
  buffer_puts(buffer_1, "type");
  buffer_putnlflush(buffer_1);

  range_foreach(&sections, section) {
    uint32 name = ELF_GET(map.start, section, shdr, sh_name);
    uint64 addr = ELF_GET(map.start, section, shdr, sh_addr);
    uint64 size = ELF_GET(map.start, section, shdr, sh_size);
    uint64 offs = ELF_GET(map.start, section, shdr, sh_offset);
    uint64 align = ELF_GET(map.start, section, shdr, sh_addralign);
    uint32 type = ELF_GET(map.start, section, shdr, sh_type) % ELF_SHT_NUM;

    if(!name && !addr && !size)
      continue;

    buffer_putspad(buffer_1, &(elf_shstrtab(map.start)[name]), 16);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, addr, ELF_BITS(map.start) / 4);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, size, ELF_BITS(map.start) / 4);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, offs, ELF_BITS(map.start) / 4);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, align, ELF_BITS(map.start) / 4);
    buffer_putspace(buffer_1);
    buffer_puts(buffer_1, elf_section_typename(type));
    buffer_putnlflush(buffer_1);

    if(type == ELF_SHT_SYMTAB || type == ELF_SHT_DYNSYM) {
      //    elf_dump_symbols(map.start, elf_section(map.start, section), type == ELF_SHT_SYMTAB ? ".strtab" :
      //    ".dynstr");
    }
  }
}

/**
 * @brief elf_dump_segments
 * @param map               Pointer range of the loaded ELF file
 */
void
elf_dump_segments(range map) {
  int i, n;
  range segments = elf_program_headers(map.start);
  void* segment;

  if(range_size(&segments) == 0)
    return;

  buffer_putspad(buffer_1, "paddr", ELF_BITS(map.start) / 4);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "vaddr", ELF_BITS(map.start) / 4);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "filesz", ELF_BITS(map.start) / 4);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "memsz", ELF_BITS(map.start) / 4);
  buffer_putnspace(buffer_1, 3);
  buffer_puts(buffer_1, "flags");
  buffer_putnlflush(buffer_1);

  range_foreach(&segments, segment) {
    uint64 paddr = ELF_GET(map.start, segment, phdr, p_paddr);
    uint64 vaddr = ELF_GET(map.start, segment, phdr, p_vaddr);
    uint64 filesz = ELF_GET(map.start, segment, phdr, p_filesz);
    uint64 memsz = ELF_GET(map.start, segment, phdr, p_memsz);
    uint32 flags = ELF_GET(map.start, segment, phdr, p_flags);

    if(!paddr && !vaddr && !filesz)
      continue;

    buffer_puts(buffer_1, "0x");
    buffer_putxlonglong0(buffer_1, paddr, ELF_BITS(map.start) / 4);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, vaddr, ELF_BITS(map.start) / 4);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, filesz, ELF_BITS(map.start) / 4);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, memsz, ELF_BITS(map.start) / 4);
    buffer_putm_internal(buffer_1,
                         " ",
                         (flags & ELF_PF_R) ? "r" : "-",
                         (flags & ELF_PF_W) ? "w" : "-",
                         (flags & ELF_PF_W) ? "x" : "-",
                         0);
    buffer_putnlflush(buffer_1);
  }
}
