/* -*- Mode: C; indent-tabs-mode: nil;
 * c-basic-offset: 4 -*- */

#include "lib/unix.h"
#include "lib/uint64.h"
#include "lib/buffer.h"
#include "lib/mmap.h"
#include "lib/pe.h"
#include "lib/str.h"
#include "lib/fmt.h"
#include "lib/byte.h"
#include "lib/errmsg.h"
#include "lib/stralloc.h"
#include "lib/scan.h"
#include <assert.h>
#include <stdlib.h>

int list_imports, list_exports, list_deps, list_sections;
static int print_export_dir, print_data_dir, print_opt_header, print_offset_rva, print_rva_offset, print_range, print_as_rva;
static uint64 offset, rva;
static stralloc search;

void pe_dump_sections(uint8* base);

#define PE_DUMP_FIELD(base, ptr, st, field) \
  buffer_putspad(b, #field, 30), buffer_puts(b, " 0x"), buffer_putxlonglong0(b, PE_GET(base, ptr, st, field), PE_SIZE(base, st, field) * 2), buffer_putnlflush(b)

size_t
pe_end_offset(uint8* base) {
  uint16 i, n;
  pe_section_header* sections = pe_header_sections(base, &n);
  size_t start, length;

  start = uint32_get(&sections[n - 1].pointer_to_raw_data);
  length = uint32_get(&sections[n - 1].size_of_raw_data);

  return start + length;
}

size_t
pe_search_sections(uint8* base, const char* x, size_t len) {
  uint16 i, n;
  pe_section_header* sections = pe_header_sections(base, &n);
  size_t start, length, j, ret = 0;

  for(i = 0; i < n; i++) {
    start = uint32_get(&sections[i].pointer_to_raw_data);
    length = uint32_get(&sections[i].size_of_raw_data);
    j = 0;

    while(j < length) {
      j += byte_findb(base + start + j, length - j, x, len);
      if(j < length) {
        uint64 offset = start + j;

        if(print_as_rva)
          offset = pe_offset2rva(base, offset);

        buffer_puts(buffer_1, "0x");
        buffer_putxlonglong0(buffer_1, offset, offset > 0xffffffff ? 16 : 8);
        buffer_putnlflush(buffer_1);
        ret++;
        j++;
      }
    }
  }
  return ret;
}

void
pe_dump_opthdr(buffer* b, uint8* base) {
  void* opthdr = pe_header_opt(base);

  PE_DUMP_FIELD(base, opthdr, opt_header, magic);
  PE_DUMP_FIELD(base, opthdr, opt_header, major_linker_version);
  PE_DUMP_FIELD(base, opthdr, opt_header, minor_linker_version);
  PE_DUMP_FIELD(base, opthdr, opt_header, size_of_code);
  PE_DUMP_FIELD(base, opthdr, opt_header, size_of_initialized_data);
  PE_DUMP_FIELD(base, opthdr, opt_header, size_of_uninitialized_data);
  PE_DUMP_FIELD(base, opthdr, opt_header, address_of_entry_point);
  PE_DUMP_FIELD(base, opthdr, opt_header, base_of_code);
  PE_DUMP_FIELD(base, opthdr, opt_header, image_base);
  PE_DUMP_FIELD(base, opthdr, opt_header, section_alignment);
  PE_DUMP_FIELD(base, opthdr, opt_header, file_alignment);
  PE_DUMP_FIELD(base, opthdr, opt_header, major_operating_system_version);
  PE_DUMP_FIELD(base, opthdr, opt_header, minor_operating_system_version);
  PE_DUMP_FIELD(base, opthdr, opt_header, major_image_version);
  PE_DUMP_FIELD(base, opthdr, opt_header, minor_image_version);
  PE_DUMP_FIELD(base, opthdr, opt_header, major_subsystem_version);
  PE_DUMP_FIELD(base, opthdr, opt_header, minor_subsystem_version);
  PE_DUMP_FIELD(base, opthdr, opt_header, reserved1);
  PE_DUMP_FIELD(base, opthdr, opt_header, size_of_image);
  PE_DUMP_FIELD(base, opthdr, opt_header, size_of_headers);
  PE_DUMP_FIELD(base, opthdr, opt_header, checksum);
  PE_DUMP_FIELD(base, opthdr, opt_header, subsystem);
  PE_DUMP_FIELD(base, opthdr, opt_header, dll_characteristics);
  PE_DUMP_FIELD(base, opthdr, opt_header, size_of_stack_reserve);
  PE_DUMP_FIELD(base, opthdr, opt_header, size_of_stack_commit);
  PE_DUMP_FIELD(base, opthdr, opt_header, size_of_heap_reserve);
  PE_DUMP_FIELD(base, opthdr, opt_header, size_of_heap_commit);
  PE_DUMP_FIELD(base, opthdr, opt_header, loader_flags);
  PE_DUMP_FIELD(base, opthdr, opt_header, number_of_rva_and_sizes);
}

void
pe_print_data_directories(buffer* b, uint8* base, pe_data_directory* data_dirs, size_t n) {
  size_t i;

  for(i = 0; i < n; ++i) {
    const char* name = pe_datadir_name(i);
    buffer_putspad(b, name ? name : "", 12);
    buffer_putspace(b);
    buffer_putxlong0(b, pe_rva2offset(base, uint32_get(&data_dirs[i].virtual_address)), sizeof(data_dirs[i].virtual_address) * 2);
    buffer_putspace(b);
    buffer_putxlong0(b, uint32_get(&data_dirs[i].size), sizeof(data_dirs[i].size) * 2);
    buffer_putnlflush(b);
  }
}

void
pe_print_export_directory(buffer* b, uint8* base, pe_export_directory* export_dir) {
  const char* name = pe_rva2ptr(base, uint32_get(&export_dir->name));
  buffer_puts(b, "characteristics: ");
  buffer_putxlong0(b, export_dir->characteristics, sizeof(export_dir->characteristics) * 2);
  buffer_puts(b, "\ntime_date_stamp: ");
  buffer_putxlong0(b, export_dir->time_date_stamp, sizeof(export_dir->time_date_stamp) * 2);
  buffer_puts(b, "\nmajor_version: ");
  buffer_putulong(b, export_dir->major_version);
  buffer_puts(b, "\nminor_version: ");
  buffer_putulong(b, export_dir->minor_version);
  buffer_puts(b, "\nname: ");
  buffer_puts(b, name ? name : "(null)");
  buffer_puts(b, "\nbase: ");
  buffer_putulong(b, uint32_get(&export_dir->base));
  buffer_puts(b, "\nnumber_of_functions: ");
  buffer_putulong(b, uint32_get(&export_dir->number_of_functions));
  buffer_puts(b, "\nnumber_of_names: ");
  buffer_putulong(b, uint32_get(&export_dir->number_of_names));
  buffer_puts(b, "\naddress_of_functions: ");
  buffer_putxlong0(b, pe_rva2offset(base, uint32_get(&export_dir->address_of_functions)), sizeof(export_dir->address_of_functions) * 2);
  buffer_puts(b, "\naddress_of_names: ");
  buffer_putxlong0(b, pe_rva2offset(base, uint32_get(&export_dir->address_of_names)), sizeof(export_dir->address_of_names) * 2);
  buffer_puts(b, "\naddress_of_name_ordinals: ");
  buffer_putxlong0(b, pe_rva2offset(base, uint32_get(&export_dir->address_of_name_ordinals)), sizeof(export_dir->address_of_name_ordinals) * 2);
  buffer_putnlflush(b);
}

void
pe_dump_exports(uint8* base) {
  size_t i;
  pe_data_directory* export_dir = &pe_header_datadir(base)[PE_DIRECTORY_ENTRY_EXPORT];
  pe_section_header* text;
  uint32 fnaddr, *nameptr, *fnptr, mintextptr, maxtextptr;
  uint16* ordptr;
  char* dllname;

  pe_export_directory* exports = pe_rva2ptr(base, uint32_get(&export_dir->virtual_address));
  // pe_print_export_directory(buffer_1,
  // exports);

  if(!(text = pe_get_section(base, ".text"))) {
    buffer_putsflush(buffer_2, "no .text section\n");
    return;
  }

  nameptr = (uint32*)pe_rva2ptr(base, uint32_get(&exports->address_of_names));
  fnptr = (uint32*)pe_rva2ptr(base, uint32_get(&exports->address_of_functions));
  ordptr = (uint16*)pe_rva2ptr(base, uint32_get(&exports->address_of_name_ordinals));

  mintextptr = uint32_get(&text->virtual_address);
  maxtextptr = mintextptr + uint32_get(&text->size_of_raw_data);
  if((dllname = pe_dllname(base)))
    buffer_putm_internal(buffer_1, "LIBRARY ", dllname, "\n", NULL);

  buffer_puts(buffer_1, "EXPORTS\n");

  for(i = 0; i < uint32_get(&exports->number_of_names); i++) {
    const char* s;
    uint16 ordinal = uint16_get(&ordptr[i]);
    fnaddr = fnptr[ordinal];
    buffer_puts(buffer_1, "  ");

    /* if(mintextptr < fnaddr && fnaddr
     * < maxtextptr)*/
    {
      s = pe_rva2ptr(base,
                     uint32_get(&nameptr[i])); // pe_rva2ptr(base,
                                               // pe_thunk(base,
                                               // nameptr,
                                               // i));
      buffer_puts(buffer_1, s ? s : "<null>");
    }

    buffer_puts(buffer_1, " @ ");
    buffer_putulong(buffer_1, ordinal);
    buffer_putnlflush(buffer_1);
  }
}

void
pe_dump_imports(uint8* base) {
  int i, j, n;
  pe_data_directory* import_dir = &pe_header_datadir(base)[PE_DIRECTORY_ENTRY_IMPORT];
  pe_import_descriptor* imports = pe_rva2ptr(base, uint32_get(&import_dir->virtual_address));

  // n = import_dir->size /
  // sizeof(pe_import_descriptor) - 1;

  if(list_imports) {
    buffer_puts(buffer_1, "IMPORTS\n");

  } else {
    buffer_putspad(buffer_1, "symbol", 64);
    buffer_putspace(buffer_1);
    buffer_putspad(buffer_1, "ordinal", 5);
    buffer_putnspace(buffer_1, 3);
    buffer_puts(buffer_1, "dll");
    buffer_putnlflush(buffer_1);
  }

  for(i = 0; imports[i].original_first_thunk; ++i) {
    const char* name = pe_rva2ptr(base, uint32_get(&imports[i].name));
    void* thunk;
    if(name[0] == '\0')
      break;

    thunk = pe_rva2ptr(base, uint32_get(&imports[i].first_thunk));

    for(j = 0;; ++j) {
      const char* sym = NULL;
      uint16 ordinal = 0;
      int64 rva;

      if(!(rva = pe_thunk(base, thunk, j)))
        break;

      if(rva < 0) {
        rva <<= 1;
        rva >>= 1;
        ordinal = rva;
      } else {
        pe_import_by_name* name_import;

        if((name_import = pe_rva2ptr(base, rva))) {
          sym = (const char*)name_import->name;
          ordinal = uint16_get(&name_import->hint);
        }
      }

      if(list_imports) {
        buffer_puts(buffer_1, "  ");
      }

      buffer_putspad(buffer_1, sym ? sym : "", 64);

      if(list_imports) {
        buffer_putspace(buffer_1);
      } else {
        buffer_putnspace(buffer_1, 3);
        buffer_putulong0(buffer_1, ordinal, 5);
        buffer_putnspace(buffer_1, 3);
      }

      buffer_puts(buffer_1, name);
      buffer_putnlflush(buffer_1);
    }
  }
}

void
pe_dump_dependencies(uint8* base) {
  int i;
  pe_data_directory* import_dir = &pe_header_datadir(base)[PE_DIRECTORY_ENTRY_IMPORT];
  pe_import_descriptor* imports = pe_rva2ptr(base, uint32_get(&import_dir->virtual_address));

  for(i = 0; imports[i].original_first_thunk; ++i) {
    const char* name = pe_rva2ptr(base, uint32_get(&imports[i].name));

    buffer_puts(buffer_1, name);
    buffer_putnlflush(buffer_1);
  }
}

int
parse_offset(const char* arg, uint64* dest) {
  int ret = 0;
  if(str_start(arg, "0x"))
    ret = scan_xlonglong(arg + 2, dest) > 0;
  if(!ret)
    ret = scan_ulonglong(arg, dest) > 0;

  return ret;
}

int
parse_search(const char* arg, stralloc* dest) {
  int ret = 0;
  uint64 n;
  ret = scan_xlonglong(str_start(arg, "0x") ? arg + 2 : arg, &n);
  if(ret > 0) {
    ret = (ret + 1) / 2;
    stralloc_ready(dest, 16);
    uint64_pack(dest->s, n);
    dest->len = ret;
  } else {
    stralloc_copys(dest, arg);
    ret = dest->len;
  }
  return ret;
}

void
put_search(buffer* b, const stralloc* search) {
  size_t i;
  for(i = 0; i < search->len; i++) {
    if(i > 0)
      buffer_putspace(b);
    buffer_putxlong0(b, search->s[i], 2);
  }
}

void
usage(char* av0) {
  const char* me = str_basename(av0);
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       me,
                       " [OPTIONS] <file...>\n"
                       "\n"
                       "Options:\n"
                       "\n"
                       "  -h, --help              Show this help\n"
                       "  -i, --imports           List imports\n"
                       "  -e, --exports           List exports\n"
                       "  -d, --deps              List DLL dependencies\n"
                       "  -s, --sections          List PE32 sections\n"
                       "  -E, --export-directory  Print export directory\n"
                       "  -D, --data-directory    Print data directory\n"
                       "  -O, --optional-header   Print optional header\n"
                       "  -r, --range             Print sections as ranges\n"
                       "  -o, --offset-rva        Print RVA of given offset\n"
                       "  -a, --rva-offset        Print offset of given RVA\n"
                       "  -S, --search BYTES      Search data\n"
                       "  -R, --rva               Search result as RVA\n"
                       "\n",
                       NULL);
  buffer_flush(buffer_1);
}

int
main(int argc, char** argv) {
  uint8* base = NULL;
  size_t filesize;

  int c, index = 0;

  static const struct unix_longopt opts[] = {
      {"help", 0, NULL, 'h'},
      {"imports", 0, &list_imports, 'i'},
      {"exports", 0, &list_exports, 'e'},
      {"deps", 0, &list_deps, 'd'},
      {"sections", 0, &list_sections, 's'},
      {"export-directory", 0, &print_export_dir, 'E'},
      {"data-directory", 0, &print_data_dir, 'D'},
      {"optional-header", 0, &print_opt_header, 'O'},
      {"range", 0, &print_range, 'r'},
      {"offset-rva", 0, NULL, 'o'},
      {"rva-offset", 0, NULL, 'a'},
      {"search", 0, NULL, 'S'},
      {"rva", 0, &print_as_rva, 'R'},
      {0, 0, 0, 0},
  };

  errmsg_iam(argv[0]);

#ifdef DEBUG_OUTPUT_
  buffer_puts(buffer_1, "Number of arguments: ");
  buffer_putlong(buffer_1, argc);
  buffer_putnlflush(buffer_1);
#endif

  for(;;) {
    c = unix_getopt_long(argc, argv, "hiedsEDOo:ra:S:R", opts, &index);
    if(c == -1)
      break;
    if(c == '\0')
      continue;

    switch(c) {
      case 'h': usage(argv[0]); return 0;
      case 'i': list_imports = 1; break;
      case 'e': list_exports = 1; break;
      case 'd': list_deps = 1; break;
      case 's': list_sections = 1; break;
      case 'E': print_export_dir = 1; break;
      case 'D': print_data_dir = 1; break;
      case 'O': print_opt_header = 1; break;
      case 'r': print_range = 1; break;
      case 'o': print_offset_rva = parse_offset(unix_optarg, &offset); break;
      case 'a': print_rva_offset = parse_offset(unix_optarg, &rva); break;
      case 'S': parse_search(unix_optarg, &search); break;
      case 'R': print_as_rva = 1; break;
      default: {
        usage(argv[0]);
        return 1;
      }
    }
  }

  if(!(list_deps | list_exports | list_imports | list_sections))
    list_imports = list_exports = 1;

  for(; argv[unix_optind]; ++unix_optind) {
    base = (uint8*)mmap_read(argv[unix_optind], &filesize);

    if(base) {
      pe32_nt_headers* nt_headers = pe_header_nt(base);

      if(nt_headers->signature != PE_NT_SIGNATURE) {
        buffer_putsflush(buffer_2, "not PE\n");
        return -1;
      }

      if(print_offset_rva) {
        uint64 rva = pe_offset2rva(base, offset);

        buffer_puts(buffer_1, "0x");

        buffer_putxlonglong0(buffer_1, rva, rva > 0xffffffff ? 16 : 8);
        buffer_putnlflush(buffer_1);
        continue;
      }
      if(print_rva_offset) {
        uint64 offset = pe_rva2offset(base, rva);

        buffer_puts(buffer_1, "0x");

        buffer_putxlonglong0(buffer_1, offset, offset > 0xffffffff ? 16 : 8);
        buffer_putnlflush(buffer_1);
        continue;
      }

      if(search.len) {
        uint64 num = pe_search_sections(base, search.s, search.len);

        if(num == 0) {
          buffer_puts(buffer_1, "Not found: ");
          put_search(buffer_1, &search);
          buffer_putnlflush(buffer_1);
        }
        continue;
      }

      // if(!(nt_headers->coff_header.characteristics
      // & PE_FILE_DLL)) {
      // buffer_putsflush(buffer_2, "not
      // DLL\n"); return -1;
      // }
      //
      if(print_opt_header)
        pe_dump_opthdr(buffer_1, base);

      if(list_sections)
        pe_dump_sections(base);
      if(list_imports)
        pe_dump_imports(base);
      if(list_deps)
        pe_dump_dependencies(base);

      if(nt_headers->coff_header.characteristics & PE_FILE_DLL) {
        if(list_exports)
          pe_dump_exports(base);
      }

      if(print_export_dir) {
        pe_data_directory* data_dir = &pe_header_datadir(base)[PE_DIRECTORY_ENTRY_EXPORT];
        pe_export_directory* export_dir = pe_rva2ptr(base, data_dir->virtual_address);

        pe_print_export_directory(buffer_2, base, export_dir);
      }
      if(print_data_dir) {
        uint32 num_dirs;
        pe_data_directory* data_dir = pe_get_datadir(base, &num_dirs);

        pe_print_data_directories(buffer_2, base, data_dir, num_dirs);
      }

      mmap_unmap((void*)base, filesize);
    } else {
      errmsg_warn("ERROR opening '", argv[unix_optind], "': ", 0);
      return 127;
    }
  }

  return 0;
}

void
pe_dump_sections(uint8* base) {
  uint16 i, n;
  pe_section_header* sections = pe_header_sections(base, &n);

  buffer_putspad(buffer_1, "section name", 16);
  buffer_putspace(buffer_1);
  if(!print_range) {
    buffer_putspad(buffer_1, "vsize", sizeof(sections[i].physical_address) * 2);
    buffer_putnspace(buffer_1, 3);
  }
  buffer_putspad(buffer_1, "rva", sizeof(sections[i].virtual_address) * 2);
  buffer_putnspace(buffer_1, 3);
  if(print_range) {
    buffer_putspad(buffer_1, "vend", sizeof(sections[i].physical_address) * 2);
    buffer_putnspace(buffer_1, 3);
  }
  if(!print_range) {
    buffer_putspad(buffer_1, "rawsize", sizeof(sections[i].size_of_raw_data) * 2);
    buffer_putnspace(buffer_1, 3);
  }
  buffer_putspad(buffer_1, "pointer", sizeof(sections[i].pointer_to_raw_data) * 2);
  if(print_range) {
    buffer_putnspace(buffer_1, 3);
    buffer_putspad(buffer_1, "raw end", sizeof(sections[i].size_of_raw_data) * 2);
  }
  buffer_putnlflush(buffer_1);

  for(i = 0; i < n; i++) {
    buffer_putspad(buffer_1, sections[i].name, 16);
    if(!print_range) {
      buffer_puts(buffer_1, " 0x");
      buffer_putxlong0(buffer_1, uint32_get(&sections[i].physical_address), sizeof(sections[i].physical_address) * 2);
    }
    buffer_puts(buffer_1, " 0x");
    buffer_putxlong0(buffer_1, uint32_get(&sections[i].virtual_address), sizeof(sections[i].virtual_address) * 2);
    if(print_range) {
      buffer_puts(buffer_1, " 0x");
      buffer_putxlong0(buffer_1, uint32_get(&sections[i].virtual_address) + uint32_get(&sections[i].physical_address), sizeof(sections[i].physical_address) * 2);
    }
    if(!print_range) {
      buffer_puts(buffer_1, " 0x");
      buffer_putxlong0(buffer_1, uint32_get(&sections[i].size_of_raw_data), sizeof(sections[i].size_of_raw_data) * 2);
    }
    buffer_puts(buffer_1, " 0x");
    buffer_putxlong0(buffer_1, uint32_get(&sections[i].pointer_to_raw_data), sizeof(sections[i].pointer_to_raw_data) * 2);
    if(print_range) {
      buffer_puts(buffer_1, " 0x");
      buffer_putxlong0(buffer_1, uint32_get(&sections[i].pointer_to_raw_data) + uint32_get(&sections[i].size_of_raw_data), sizeof(sections[i].size_of_raw_data) * 2);
    }
    buffer_putnlflush(buffer_1);
  }
}
