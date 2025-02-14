/* -*- Mode: C; indent-tabs-mode: nil;
 * c-basic-offset: 4 -*- */

#include "lib/uint64.h"
#include "lib/scan.h"
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
#include "lib/errmsg.h"
#include <stdlib.h>
#include "lib/bool.h"

strlist flaglist;

void elf_dump_dynamic(range map);
void elf_dump_sections(range map);
void elf_dump_segments(range map);
void elf_dump_symbols(
    range map, range section, range text, const char* stname, int binding);
void elf_print_prefix(buffer* b);

int
hex64(buffer* b, uint64 num, int pad) {
  buffer_puts(b, "0x");
  buffer_putxlonglong0(b, num, pad - 2);
  return 0;
}

static int list_defined, list_undefined, list_needed;
static int print_offset_rva, print_rva_offset;
static const char* filename;
typedef int put64_function(buffer*, uint64, int);
typedef int putstr_function(buffer*, const char*, size_t);
static put64_function* putnum = &hex64;
static putstr_function* putstr = &buffer_putspad;
static unsigned word_size;
static unsigned radix = 16;
static uint64 offset, rva;

int
parse_offset(const char* arg, uint64* dest) {
  int ret = 0;
  if(str_start(arg, "0x"))
    ret = scan_xlonglong(arg + 2, dest) > 0;
  if(!ret)
    ret = scan_ulonglong(arg, dest) > 0;

  return ret;
}

#define RANGE_CHECK(ptr) \
  do { \
    if(!range_ptrinbuf2(map.start, map.end, ptr)) { \
      buffer_puts(buffer_2, "OUT of range: " #ptr); \
      buffer_putnlflush(buffer_2); \
      exit(120); \
    } \
  } while(0)

#define ELF_DUMP_FIELD(base, ptr, st, field) \
  buffer_putspad(b, #field, 30), buffer_puts(b, " "), \
      putnum(b, \
             ELF_GET(base, ptr, st, field), \
             ELF_SIZE(base, st, field) * 2), \
      buffer_putnlflush(b)

void
elf_print_prefix(buffer* b) {
  if(filename)
    buffer_putm_internal(b, filename, ":", NULL);
}

#define MACHINES \
  "NONE\0M32\0SPARC\0386\068K\088K\08" \
  "60\0MIPS\0S370\0PARISC\0VPP500\0SP" \
  "ARC32PLUS\0960\0PPC\0PPC64" \
  "\0S390\0V800\0FR20\0" \
  "RH32\0RCE\0ARM\0FAKE_" \
  "ALPHA\0SH\0SPARCV9\0TRICORE\0ARC\0" \
  "H8_300\0H8_300H\0H8S\0H8_500\0IA_" \
  "64\0COLDFIRE\068HC12\0MMA\0PCP\0NC" \
  "PU\0NDR1\0STARCORE\0ME16\0ST100\0T" \
  "INYJ\0X86_" \
  "64\0PDSP\0FX66\0ST9PLUS\0ST7\068HC" \
  "16\068HC11\068HC08\068HC05\0SVX\0S" \
  "T19\0VAX\0CRIS\0JAVELIN\0F" \
  "IREPATH\0ZSP\0MMIX\0" \
  "HUANY\0PRISM\0AVR\0FR30\0D10V\0D30" \
  "V\0V850\0M32R\0MN10300\0MN10200\0P" \
  "J\0OPENRISC\0ARC_" \
  "A5\0XTENSA\0ALPHA"
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
 * @brief elf_dump_dynamic  Dumps all
 * ELF dynamic entries
 * @param map               Pointer
 * range of the loaded ELF file
 */
void
elf_dump_dynamic(range map) {

  int i = 0, di = elf_section_find(map.start, ".dynamic");
  range dyn;
  void* entry;
  const char* dynstrtab = NULL;
  int col_width = ELF_BITS(map.start) / 4 + 2;
  static const char* const dynamic_types[] = {
      "NULL",          "NEEDED",
      "PLTRELSZ",      "PLTGOT",
      "HASH",          "STRTAB",
      "SYMTAB",        "RELA",
      "RELASZ",        "RELAENT",
      "STRSZ",         "SYMENT",
      "INIT",          "FINI",
      "SONAME",        "RPATH",
      "SYMBOLIC",      "REL",
      "RELSZ",         "RELENT",
      "PLTREL",        "DEBUG",
      "TEXTREL",       "JMPREL",
      "BIND_NOW",      "INIT_ARRAY",
      "FINI_ARRAY",    "INIT_ARRAYSZ",
      "FINI_ARRAYSZ",  "RUNPATH",
      "FLAGS",         "ENCODING",
      "PREINIT_ARRAY", "PREINIT_ARRAYSZ",
  };

  if(di == -1)
    return;

  dyn = elf_dynamic_section(map.start);

  range_foreach(&dyn, entry) {
    int64 tag = ELF_GET(map.start, entry, dyn, d_tag);
    uint64 val = ELF_GET(map.start, entry, dyn, d_un.d_val);

    if(tag == ELF_DT_STRTAB) {
#ifdef DEBUG_OUTPUT_
      buffer_puts(buffer_2, "ELF_DT_STRTAB:\n");
      putnum(buffer_2, val, col_width);
      buffer_putnlflush(buffer_2);
#endif

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
      putnum(buffer_1, tag, col_width);
    } else {
      buffer_putspad(buffer_1, dynamic_types[tag % ELF_DT_NUM], col_width);
    }

    if(tag == ELF_DT_NEEDED) {
      buffer_putspace(buffer_1);
      buffer_puts(buffer_1, (const char*)&dynstrtab[val]);
    } else {
      buffer_putspace(buffer_1);
      putnum(buffer_1, val, col_width);
    }
    buffer_putnlflush(buffer_1);

    if(tag == ELF_DT_NULL)
      break;
  }
}

/**
 * @brief elf_dump_needed  Dumps all
 * ELF dynamic entries
 * @param map               Pointer
 * range of the loaded ELF file
 */
void
elf_dump_needed(buffer* b, range map) {
  void* entry;
  int di = elf_section_find(map.start, ".dynamic");
  range dyn;
  const char* dynstrtab = NULL;

  if(di == -1)
    return;

  dyn = elf_dynamic_section(map.start);

  range_foreach(&dyn, entry) {
    int64 tag = ELF_GET(map.start, entry, dyn, d_tag);
    uint64 val = ELF_GET(map.start, entry, dyn, d_un.d_val);
    if(tag == ELF_DT_STRTAB) {
      dynstrtab = map.start + val;
      RANGE_CHECK(dynstrtab);
      break;
    }
  }

  range_foreach(&dyn, entry) {
    int64 tag = ELF_GET(map.start, entry, dyn, d_tag);
    uint64 val = ELF_GET(map.start, entry, dyn, d_un.d_val);

    if(tag == ELF_DT_NEEDED) {
      const char* value = (const char*)&dynstrtab[val];
      buffer_puts(b, value ? value : "(null)");
      buffer_putnlflush(b);
    }
    if(tag == ELF_DT_NULL)
      break;
  }
}

extern ssize_t buffer_putptr_size_2;

/**
 * @brief elf_dump_symbols
 * @param map               Pointer
 * range of the loaded ELF file
 * @param section           Symbol table
 * section
 * @param text              Code section
 * @param stname            String-table
 * section name
 * @param binding
 */
void
elf_dump_symbols(range map,
                 range section,
                 range text,
                 const char* stname,
                 int binding) {
  void* symbol;
  int si = elf_section_find(map.start, stname);
  const char* strtab = elf_section_offset(map.start, si);
  static const char* const binding_types[] = {
      "LOCAL",
      "GLOBAL",
      "WEAK",
  };
  static const char* const symbol_types[] = {
      "NOTYPE",
      "OBJECT",
      "FUNC",
      "SECTION",
      "FILE",
      "COMMON",
      "TLS",
  };
  range symtab = section;
  symtab.elem_size =
      ELF_BITS(map.start) == 64 ? sizeof(elf64_sym) : sizeof(elf32_sym);

  /*  buffer_putspad(buffer_1, "symbol
    name", 33); buffer_putspad(buffer_1,
    "value", ELF_BITS(map.start) / 4 + 2
    + 1); buffer_putspad(buffer_1,
    "size", col_width + 2
    + 1); if(binding < 0)
      buffer_putspad(buffer_1,
    "binding", 16);
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

    buffer_putptr_size_2 = ELF_BITS(map.start) / 4 + 2 + 1;

    if(!range_empty(&code))
      buffer_putptr(buffer_1, (char*)(ptrdiff_t)(code.start - map.start));
    else
      buffer_putnspace(buffer_1, buffer_putptr_size_2);

    if(ELF_ELF32_ST_BIND(info) == ELF_STB_GLOBAL)
      buffer_puts(buffer_1, !range_empty(&code) ? " T " : " U ");
    else
      buffer_puts(buffer_1, !range_empty(&code) ? " t " : " u ");

    buffer_putspad(buffer_1, &(strtab[name]), 32);
    /*buffer_puts(buffer_1, "");
    buffer_putxlong0(buffer_1, name, 8);
    */
    /*jjif(size) {
      buffer_putspace(buffer_1);
      putnum(buffer_1,
    value, col_width); }
    else { buffer_putnspace(buffer_1,
    col_width + 3);
    }

    if(size) {
      buffer_puts(buffer_1, "   ");
      buffer_putulong0(buffer_1, size,
    col_width); } else {
      buffer_putnspace(buffer_1,
    col_width + 3);
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

const char*
elf_section_flags(uint64 flags, strlist* list) {
  strlist_zero(list);
  list->sep = '|';

  if(flags & ELF_SHF_WRITE)
    strlist_push(list, "WRITE");
  if(flags & ELF_SHF_ALLOC)
    strlist_push(list, "ALLOC");
  if(flags & ELF_SHF_EXECINSTR)
    strlist_push(list, "EXECINSTR");
  if(flags & ELF_SHF_MERGE)
    strlist_push(list, "MERGE");
  if(flags & ELF_SHF_STRINGS)
    strlist_push(list, "STRINGS");
  if(flags & ELF_SHF_INFO_LINK)
    strlist_push(list, "INFO_LINK");
  if(flags & ELF_SHF_LINK_ORDER)
    strlist_push(list, "LINK_ORDER");
  if(flags & ELF_SHF_OS_NONCONFORMING)
    strlist_push(list, "OS_NONCONFORMING");
  if(flags & ELF_SHF_GROUP)
    strlist_push(list, "GROUP");
  if(flags & ELF_SHF_TLS)
    strlist_push(list, "TLS");
  if(flags & ELF_SHF_MASKOS)
    strlist_push(list, "MASKOS");
  if(flags & ELF_SHF_MASKPROC)
    strlist_push(list, "MASKPROC");
  if(flags & ELF_SHF_ARM_ENTRYSECT)
    strlist_push(list, "ARM_ENTRYSECT");
  if(flags & ELF_SHF_ARM_COMDEF)
    strlist_push(list, "ARM_COMDEF");
  stralloc_nul(&list->sa);
  return list->sa.s;
}

/**
 * @brief elf_dump_sections
 * @param map               Pointer
 * range of the loaded ELF file
 */
void
elf_dump_sections(range map) {
  int i, n;
  range sections = elf_section_headers(map.start);
  void* section;
  int col_width = ELF_BITS(map.start) / 4 + 2 + 1;

  buffer_putspad(buffer_1, "section name", 16);
  buffer_putspace(buffer_1);
  putstr(buffer_1, "addr", col_width);
  buffer_putspace(buffer_1);
  putstr(buffer_1, "size", col_width);
  buffer_putspace(buffer_1);
  putstr(buffer_1, "offset", col_width);
  buffer_putspace(buffer_1);
  putstr(buffer_1, "align", col_width);
  buffer_putspace(buffer_1);
  buffer_putspad(buffer_1, "type", 8);
  buffer_putspace(buffer_1);
  buffer_puts(buffer_1, "flags");
  buffer_putnlflush(buffer_1);

  range_foreach(&sections, section) {
    uint32 name = ELF_GET(map.start, section, shdr, sh_name);
    uint64 addr = ELF_GET(map.start, section, shdr, sh_addr);
    uint64 size = ELF_GET(map.start, section, shdr, sh_size);
    uint64 offs = ELF_GET(map.start, section, shdr, sh_offset);
    uint64 align = ELF_GET(map.start, section, shdr, sh_addralign);
    uint32 type = ELF_GET(map.start, section, shdr, sh_type) % ELF_SHT_NUM;
    uint64 flags = ELF_GET(map.start, section, shdr, sh_flags);

    if(!name && !addr && !size)
      continue;

    buffer_putspad(buffer_1, &(elf_shstrtab(map.start)[name]), 16);
    buffer_putspace(buffer_1);
    putnum(buffer_1, addr, col_width);
    buffer_putspace(buffer_1);
    putnum(buffer_1, size, col_width);
    buffer_putspace(buffer_1);
    putnum(buffer_1, offs, col_width);
    buffer_putspace(buffer_1);
    putnum(buffer_1, align, col_width);
    buffer_putspace(buffer_1);
    buffer_putspad(buffer_1, elf_section_typename(type), 8);
    buffer_putspace(buffer_1);
    buffer_puts(buffer_1, elf_section_flags(flags, &flaglist));

    buffer_putnlflush(buffer_1);

    if(type == ELF_SHT_SYMTAB || type == ELF_SHT_DYNSYM) {
      //    elf_dump_symbols(map.start,
      //    elf_section(map.start,
      //    section), type ==
      //    ELF_SHT_SYMTAB ?
      //    ".strtab" :
      //    ".dynstr");
    }
  }
}

/**
 * @brief elf_dump_segments
 * @param map               Pointer
 * range of the loaded ELF file
 */
void
elf_dump_segments(range map) {
  int i, n;
  range segments = elf_program_headers(map.start);
  void* segment;
  int col_width = ELF_BITS(map.start) / 4 + 2;

  if(range_size(&segments) == 0)
    return;

  putstr(buffer_1, "offset", col_width);
  buffer_putspace(buffer_1);
  putstr(buffer_1, "paddr", col_width);
  buffer_putspace(buffer_1);
  putstr(buffer_1, "vaddr", col_width);
  buffer_putspace(buffer_1);
  putstr(buffer_1, "filesz", col_width);
  buffer_putspace(buffer_1);
  putstr(buffer_1, "memsz", col_width);
  buffer_putspace(buffer_1);
  buffer_puts(buffer_1, "flags");
  buffer_putnlflush(buffer_1);

  range_foreach(&segments, segment) {
    uint64 offset = ELF_GET(map.start, segment, phdr, p_paddr);
    uint64 paddr = ELF_GET(map.start, segment, phdr, p_paddr);
    uint64 vaddr = ELF_GET(map.start, segment, phdr, p_vaddr);
    uint64 filesz = ELF_GET(map.start, segment, phdr, p_filesz);
    uint64 memsz = ELF_GET(map.start, segment, phdr, p_memsz);
    uint32 flags = ELF_GET(map.start, segment, phdr, p_flags);

    if(!paddr && !vaddr && !filesz)
      continue;
    putnum(buffer_1, offset, col_width);
    buffer_putspace(buffer_1);
    putnum(buffer_1, paddr, col_width);
    buffer_putspace(buffer_1);
    putnum(buffer_1, vaddr, col_width);
    buffer_putspace(buffer_1);
    putnum(buffer_1, filesz, col_width);
    buffer_putspace(buffer_1);
    putnum(buffer_1, memsz, col_width);
    buffer_putm_internal(buffer_1,
                         " ",
                         (flags & ELF_PF_R) ? "r" : "-",
                         (flags & ELF_PF_W) ? "w" : "-",
                         (flags & ELF_PF_W) ? "x" : "-",
                         NULL);
    buffer_putnlflush(buffer_1);
  }
}

/**
 * @brief usage  Show command line usage
 * @param av0
 */
void
usage(char* av0) {
  buffer_putm_internal(
      buffer_1,
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
      "  -S, --sections          Dump sections\n",
      "  -o, --offset-rva        Print RVA of given offset\n",
      "  -a, --rva-offset        Print offset of given RVA\n",
      "  -n, --needed            Print needed libraries\n\n",
      NULL);
  buffer_flush(buffer_1);
}

int
main(int argc, char** argv) {
  static range map;
  size_t filesize;
  static int dump_file_header = false, dump_sections = false,
             dump_segments = false;
  int i, c, index = 0;

  struct unix_longopt opts[] = {
      {"help", 0, NULL, 'h'},
      {"defined", 0, &list_defined, 'D'},
      {"undefined", 0, &list_undefined, 'U'},
      {"file-header", 0, 0, 'F'},
      {"sections", 0, &dump_sections, 'S'},
      {"segments", 0, &dump_segments, 'l'},
      {"radix", 1, 0, 'r'},
      {"offset-rva", 0, NULL, 'o'},
      {"rva-offset", 0, NULL, 'a'},
      {"needed", 0, &list_needed, 'n'},
      {0, 0, 0, 0},
  };
  strlist_init(&flaglist, '|');

  for(;;) {
    c = unix_getopt_long(argc, argv, "hDUFSlr:o:a:n", opts, &index);
    if(c == -1)
      break;
    if(c == '\0')
      continue;

    switch(c) {
      case 'h': {
        usage(argv[0]);
        return 0;
      }
      case 'n': list_needed = true; break;
      case 'D': list_defined = true; break;
      case 'U': list_undefined = true; break;
      case 'F': dump_file_header = true; break;
      case 'S': dump_sections = true; break;
      case 'l': dump_segments = true; break;
      case 'r': {
        radix = atoi(unix_optarg);
        break;
      }
      case 'o': {
        print_offset_rva = parse_offset(unix_optarg, &offset);
        break;
      }
      case 'a': {
        print_rva_offset = parse_offset(unix_optarg, &rva);
        break;
      }

      default: {
        usage(argv[0]);
        return 1;
      }
    }
  }

  if(!(list_defined | list_undefined))
    list_defined = list_undefined = 1;

  if(unix_optind == argc) {
    usage(argv[0]);
    return 0;
  }

  if(radix == 10)
    putnum = &buffer_putulonglongpad;

  for(i = unix_optind; argv[i]; ++i) {
    const char* interp;
    range symtab, text;

    filename = argv[i];
    if(argc - unix_optind >= 2) {
      buffer_putm_internal(buffer_2, filename, ":", NULL);
      buffer_putnlflush(buffer_2);
    }

    map.start = (char*)mmap_read(filename, &filesize);

    if(map.start == 0) {
      errmsg_warnsys("ERROR: ", "mmap '", filename, "'", 0);
      return 127;
    }

    map.end = map.start + filesize;

    // word_size = ELF_BITS(map.start);
#ifdef DEBUG_OUTPUT_
    buffer_puts(buffer_2, "map start: ");
    buffer_putptr(buffer_2, map.start);
    buffer_putnlflush(buffer_2);
#endif

    if(print_offset_rva) {
      uint64 rva = elf_offset_to_addr(map, offset);

      putnum(buffer_1, rva, rva > 0xffffffff ? 18 : 10);
      buffer_putnlflush(buffer_1);
      continue;
    }
    if(print_rva_offset) {
      uint64 offset = elf_address_to_offset(map, rva);

      putnum(buffer_1, offset, offset > 0xffffffff ? 18 : 10);
      buffer_putnlflush(buffer_1);
      continue;
    }

    if(list_needed) {
      elf_dump_needed(buffer_1, map);
      continue;
    }

    if(dump_file_header)
      elf_dump_header(buffer_1, map);

    interp = elf_get_section(map.start, ".interp", NULL);

    if(dump_sections)
      elf_dump_sections(map);
    if(dump_segments)
      elf_dump_segments(map);
    elf_dump_dynamic(map);

    if(interp) {
      buffer_putm_internal(buffer_1, "Interpreter: ", interp, NULL);
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
