/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "lib/unix.h"
#include "lib/uint64.h"
#include "lib/buffer.h"
#include "lib/mmap.h"
#include "lib/coff.h"
#include "lib/str.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/errmsg.h"
#include <assert.h>
#include <stdlib.h>

int list_imports, list_exports, list_deps, list_sections;
static int print_export_dir, print_data_dir, print_opt_header;

void coff_dump_sections(uint8* base);

#define COFF_DUMP_FIELD(base, ptr, st, field) \
  buffer_putspad(b, #field, 30), \
  buffer_puts(b, " 0x"), \
  buffer_putxint640(b, COFF_GET(base, ptr, st, field), COFF_SIZE(base, st, field) * 2), \
  buffer_putnlflush(b)
/*
void
coff_dump_opthdr(buffer* b, uint8* base) {
  void* opthdr = coff_header_opt(base);

  COFF_DUMP_FIELD(base, opthdr, opt_header, magic);
  COFF_DUMP_FIELD(base, opthdr, opt_header, major_linker_version);
  COFF_DUMP_FIELD(base, opthdr, opt_header, minor_linker_version);
  COFF_DUMP_FIELD(base, opthdr, opt_header, size_of_code);
  COFF_DUMP_FIELD(base, opthdr, opt_header, size_of_initialized_data);
  COFF_DUMP_FIELD(base, opthdr, opt_header, size_of_uninitialized_data);
  COFF_DUMP_FIELD(base, opthdr, opt_header, address_of_entry_point);
  COFF_DUMP_FIELD(base, opthdr, opt_header, base_of_code);
  COFF_DUMP_FIELD(base, opthdr, opt_header, image_base);
  COFF_DUMP_FIELD(base, opthdr, opt_header, section_alignment);
  COFF_DUMP_FIELD(base, opthdr, opt_header, file_alignment);
  COFF_DUMP_FIELD(base, opthdr, opt_header, major_operating_system_version);
  COFF_DUMP_FIELD(base, opthdr, opt_header, minor_operating_system_version);
  COFF_DUMP_FIELD(base, opthdr, opt_header, major_image_version);
  COFF_DUMP_FIELD(base, opthdr, opt_header, minor_image_version);
  COFF_DUMP_FIELD(base, opthdr, opt_header, major_subsystem_version);
  COFF_DUMP_FIELD(base, opthdr, opt_header, minor_subsystem_version);
  COFF_DUMP_FIELD(base, opthdr, opt_header, reserved1);
  COFF_DUMP_FIELD(base, opthdr, opt_header, size_of_image);
  COFF_DUMP_FIELD(base, opthdr, opt_header, size_of_headers);
  COFF_DUMP_FIELD(base, opthdr, opt_header, checksum);
  COFF_DUMP_FIELD(base, opthdr, opt_header, subsystem);
  COFF_DUMP_FIELD(base, opthdr, opt_header, dll_characteristics);
  COFF_DUMP_FIELD(base, opthdr, opt_header, size_of_stack_reserve);
  COFF_DUMP_FIELD(base, opthdr, opt_header, size_of_stack_commit);
  COFF_DUMP_FIELD(base, opthdr, opt_header, size_of_heap_reserve);
  COFF_DUMP_FIELD(base, opthdr, opt_header, size_of_heap_commit);
  COFF_DUMP_FIELD(base, opthdr, opt_header, loader_flags);
  COFF_DUMP_FIELD(base, opthdr, opt_header, number_of_rva_and_sizes);
}


void
coff_print_data_directories(buffer* b, uint8* base, coff_data_directory* data_dirs, size_t n) {
  size_t i;

  for(i = 0; i < n; ++i) {
    const char* name = coff_datadir_name(i);
    buffer_putspad(b, name ? name : "", 12);
    buffer_putspace(b);
    buffer_putxlong0(b,
                      coff_rva2offset(base, uint32_get(&data_dirs[i].virtual_address)),
                      sizeof(data_dirs[i].virtual_address) * 2);
    buffer_putspace(b);
    buffer_putxlong0(b, uint32_get(&data_dirs[i].size), sizeof(data_dirs[i].size) * 2);
    buffer_putnlflush(b);
  }
}

void
coff_print_export_directory(buffer* b, uint8* base, coff_export_directory* export_dir) {
  const char* name = coff_rva2ptr(base, uint32_get(&export_dir->name));
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
  buffer_putxlong0(b,
                   coff_rva2offset(base, uint32_get(&export_dir->address_of_functions)),
                   sizeof(export_dir->address_of_functions) * 2);
  buffer_puts(b, "\naddress_of_names: ");
  buffer_putxlong0(b,
                   coff_rva2offset(base, uint32_get(&export_dir->address_of_names)),
                   sizeof(export_dir->address_of_names) * 2);
  buffer_puts(b, "\naddress_of_name_ordinals: ");
  buffer_putxlong0(b,
                   coff_rva2offset(base, uint32_get(&export_dir->address_of_name_ordinals)),
                   sizeof(export_dir->address_of_name_ordinals) * 2);
  buffer_putnlflush(b);
}

void
coff_dump_exports(uint8* base) {
  int i;
  coff_data_directory* export_dir = &coff_header_datadir(base)[COFF_DIRECTORY_ENTRY_EXPORT];
  coff_section_header* text;
  uint32 fnaddr, *nameptr, *fnptr, mintextptr, maxtextptr;
  uint16* ordptr;

  coff_export_directory* exports = coff_rva2ptr(base, uint32_get(&export_dir->virtual_address));
  // coff_print_export_directory(buffer_1, exports);

  if(!(text = coff_get_section(base, ".text"))) {
    buffer_putsflush(buffer_2, "no .text section\n");
    return;
  }

  nameptr = (uint32*)coff_rva2ptr(base, uint32_get(&exports->address_of_names));
  fnptr = (uint32*)coff_rva2ptr(base, uint32_get(&exports->address_of_functions));
  ordptr = (uint16*)coff_rva2ptr(base, uint32_get(&exports->address_of_name_ordinals));

  mintextptr = uint32_get(&text->virtual_address);
  maxtextptr = mintextptr + uint32_get(&text->size_of_raw_data);

  buffer_puts(buffer_1, "EXPORTS\n");

  for(i = 0; i < uint32_get(&exports->number_of_names); i++) {
    uint16 ordinal = uint16_get(&ordptr[i]);
    fnaddr = fnptr[ordinal];

    if(mintextptr < fnaddr && fnaddr < maxtextptr) {
      buffer_puts(buffer_1, "  ");
      buffer_puts(buffer_1, coff_rva2ptr(base, coff_thunk(base, nameptr, i)));
      buffer_putnlflush(buffer_1);
    }
  }
}

void
coff_dump_imports(uint8* base) {
  int i, j, n;
  coff_data_directory* import_dir = &coff_header_datadir(base)[COFF_DIRECTORY_ENTRY_IMPORT];
  coff_import_descriptor* imports = coff_rva2ptr(base, uint32_get(&import_dir->virtual_address));

  // n = import_dir->size / sizeof(coff_import_descriptor) - 1;

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
    const char* name = coff_rva2ptr(base, uint32_get(&imports[i].name));
    void* thunk;
    if(name[0] == '\0')
      break;

    thunk = coff_rva2ptr(base, uint32_get(&imports[i].first_thunk));

    for(j = 0;; ++j) {
      const char* sym = NULL;
      uint16 ordinal;
      int64 rva;

      if(!(rva = coff_thunk(base, thunk, j)))
        break;

      if(rva < 0) {
        rva <<= 1;
        rva >>= 1;
        ordinal = rva;
      } else {
        coff_import_by_name* name_import;

        if((name_import = coff_rva2ptr(base, rva))) {
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
                       "  -i, --imports           List imports\n",
                       "  -e, --exports           List exports\n",
                       "  -d, --deps              List DLL dependencies\n",
                       "  -s, --sections          List PE32 sections\n",
                       "  -E, --export-directory  Print export directory\n",
                       "  -D, --data-directory    Print data directory\n",
                       "  -O, --optional-header   Print optional header\n",
                       "\n",
                       0);
  buffer_flush(buffer_1);
}

int
main(int argc, char** argv) {
  uint8* base = NULL;
  size_t filesize;

  int c, index = 0;

  struct longopt opts[] = {{"help", 0, NULL, 'h'},
                           {"imports", 0, &list_imports, 'i'},
                           {"exports", 0, &list_exports, 'e'},
                           {"deps", 0, &list_deps, 'd'},
                           {"sections", 0, &list_sections, 's'},
                           {"export-directory", 0, &print_export_dir, 'E'},
                           {"data-directory", 0, &print_data_dir, 'D'},
                           {"optional-header", 0, &print_opt_header, 'O'},
                           {0}};

  errmsg_iam(argv[0]);

  buffer_puts(buffer_1, "Number of arguments: ");
  buffer_putlong(buffer_1, argc);
  buffer_putnlflush(buffer_1);

  for(;;) {
    c = getopt_long(argc, argv, "hiedsEDO", opts, &index);
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
      default: {
        usage(argv[0]);
        return 1;
      }
    }
  }

  if(!(list_deps | list_exports | list_imports | list_sections))
    list_imports = list_exports = 1;

  for(; argv[optind]; ++optind) {
    base = (uint8*)mmap_read(argv[optind], &filesize);

    if(base) {
      coff_file_header* header = coff_header_file(base);

      if(header->machine != COFF_FILE_MACHINE_I386 && header->machine != COFF_FILE_MACHINE_AMD64) {
        buffer_putsflush(buffer_2, "not COFF\n");
        return -1;
      }

      // if(!(nt_headers->coff_header.characteristics & COFF_FILE_DLL)) {
      // buffer_putsflush(buffer_2, "not DLL\n");
      // return -1;
      // }
      //
/*      if(print_opt_header)
        coff_dump_opthdr(buffer_1, base);
*/
      if(list_sections)
        coff_dump_sections(base);
  /*    if(list_imports)
        coff_dump_imports(base);

      if(nt_headers->coff_header.characteristics & COFF_FILE_DLL) {
        if(list_exports)
          coff_dump_exports(base);
      }

      if(print_export_dir) {
        coff_data_directory* data_dir = &coff_header_datadir(base)[COFF_DIRECTORY_ENTRY_EXPORT];
        coff_export_directory* export_dir = coff_rva2ptr(base, data_dir->virtual_address);

        coff_print_export_directory(buffer_2, base, export_dir);
      }
      if(print_data_dir) {
        uint32 num_dirs;
        coff_data_directory* data_dir = coff_get_datadir(base, &num_dirs);

          coff_print_data_directories(buffer_2, base, data_dir, num_dirs);
      }
*/
      mmap_unmap(base, filesize);
    } else {
      errmsg_warn("ERROR opening '", argv[optind], "': ", 0);
      return 127;
    }
  }

  return 0;
}

void
coff_dump_sections(uint8* base) {
  uint16 i, n;
  coff_section_header* sections = coff_header_sections(base, &n);

  buffer_putspad(buffer_1, "section name", 16);
  buffer_putspace(buffer_1);
  buffer_putspad(buffer_1, "vsize", sizeof(sections[i].physical_address) * 2);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "rva", sizeof(sections[i].virtual_address) * 2);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "rawsize", sizeof(sections[i].size_of_raw_data) * 2);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "pointer", sizeof(sections[i].pointer_to_raw_data) * 2);
  buffer_putnlflush(buffer_1);

  for(i = 0; i < n; i++) {
    buffer_putspad(buffer_1, sections[i].name, 16);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlong0(buffer_1, uint32_get(&sections[i].physical_address), sizeof(sections[i].physical_address) * 2);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlong0(buffer_1, uint32_get(&sections[i].virtual_address), sizeof(sections[i].virtual_address) * 2);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlong0(buffer_1, uint32_get(&sections[i].size_of_raw_data), sizeof(sections[i].size_of_raw_data) * 2);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlong0(buffer_1,
                      uint32_get(&sections[i].pointer_to_raw_data),
                      sizeof(sections[i].pointer_to_raw_data) * 2);
    buffer_putnlflush(buffer_1);
  }
}
