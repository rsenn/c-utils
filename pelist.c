/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "lib/getopt.h"
#include "lib/buffer.h"
#include "lib/mmap.h"
#include "lib/pe.h"
#include "lib/str.h"
#include <assert.h>
#include <stdlib.h>

static int list_imports, list_exports, list_deps, list_sections;
static int print_export_dir, print_data_dir;

void pe_dump_sections(uint8* base);

void
pe_print_data_directory(buffer* b, pe_data_directory* data_dir) {
  buffer_puts(b, "virtual_address: 0x");
  buffer_putxlonglong0(b, data_dir->virtual_address, sizeof(data_dir->virtual_address) * 2);
  buffer_puts(b, " size: 0x");
  buffer_putxlonglong0(b, data_dir->size, sizeof(data_dir->size) * 2);
  buffer_putnlflush(b);
}

void
pe_print_export_directory(buffer* b, pe_export_directory* export_dir) {
  buffer_puts(b, "characteristics: ");
  buffer_putxlonglong0(b, export_dir->characteristics, sizeof(export_dir->characteristics) * 2);
  buffer_puts(b, "\ntime_date_stamp: ");
  buffer_putxlonglong0(b, export_dir->time_date_stamp, sizeof(export_dir->time_date_stamp) * 2);
  buffer_puts(b, "\nmajor_version: ");
  buffer_putxlonglong0(b, export_dir->major_version, sizeof(export_dir->major_version) * 2);
  buffer_puts(b, "\nminor_version: ");
  buffer_putxlonglong0(b, export_dir->minor_version, sizeof(export_dir->minor_version) * 2);
  buffer_puts(b, "\nname: ");
  buffer_putxlonglong0(b, export_dir->name, sizeof(export_dir->name) * 2);
  buffer_puts(b, "\nbase: ");
  buffer_putxlonglong0(b, export_dir->base, sizeof(export_dir->base) * 2);
  buffer_puts(b, "\nnumber_of_functions: ");
  buffer_putxlonglong0(b, export_dir->number_of_functions, sizeof(export_dir->number_of_functions) * 2);
  buffer_puts(b, "\nnumber_of_names: ");
  buffer_putxlonglong0(b, export_dir->number_of_names, sizeof(export_dir->number_of_names) * 2);
  buffer_puts(b, "\naddress_of_functions: ");
  buffer_putxlonglong0(b, export_dir->address_of_functions, sizeof(export_dir->address_of_functions) * 2);
  buffer_puts(b, "\naddress_of_names: ");
  buffer_putxlonglong0(b, export_dir->address_of_names, sizeof(export_dir->address_of_names) * 2);
  buffer_puts(b, "\naddress_of_name_ordinals: ");
  buffer_putxlonglong0(b, export_dir->address_of_name_ordinals, sizeof(export_dir->address_of_name_ordinals) * 2);
  buffer_putnlflush(b);
}

void
pe_dump_exports(uint8* base) {
  int i;
  pe_data_directory* export_dir = &pe_header_datadir(base)[PE_DIRECTORY_ENTRY_EXPORT];
  pe_section_header* text;
  uint32 fnaddr, *nameptr, *fnptr, mintextptr, maxtextptr;

  pe_export_directory* exports = pe_rva2ptr(base, export_dir->virtual_address);
  // pe_print_export_directory(buffer_1, exports);

  if(!(text = pe_get_section(base, ".text"))) {
    buffer_putsflush(buffer_2, "no .text section\n");
    return;
  }

  nameptr = (uint32*)pe_rva2ptr(base, exports->address_of_names);
  fnptr = (uint32*)pe_rva2ptr(base, exports->address_of_functions);
  mintextptr = text->virtual_address;
  maxtextptr = mintextptr + text->size_of_raw_data;

  buffer_puts(buffer_1, "EXPORTS\n");

  for(i = 0; i < exports->number_of_names; i++) {
    fnaddr = fnptr[i];
    if(mintextptr < fnaddr && fnaddr < maxtextptr) {
      buffer_puts(buffer_1, "  ");
      buffer_puts(buffer_1, pe_rva2ptr(base, nameptr[i]));
      buffer_putnlflush(buffer_1);
    }
  }
}

void
pe_dump_imports(uint8* base) {
  int i, j, n;
  pe_data_directory* import_dir = &pe_header_datadir(base)[PE_DIRECTORY_ENTRY_IMPORT];
  pe_import_descriptor* imports = pe_rva2ptr(base, import_dir->virtual_address);

  // n = import_dir->size / sizeof(pe_import_descriptor) - 1;

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
 uint32 thunk;
    if(name[0] == '\0') break;

    thunk = uint32_get(&imports[i].original_first_thunk);

    for(j = 0;; ++j) {
      const char* sym = NULL;
      uint16 ordinal;
      int64 rva;

      if(!(rva = pe_thunk(base, thunk, j))) break;

      if(rva < 0) {
        rva <<= 1;
        rva >>= 1;
        ordinal = rva;
      } else {
        pe_import_by_name* name_import;

        if((name_import = pe_rva2ptr(base, rva))) {
          sym = name_import->name;
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
usage(char* av0) {
  buffer_putm(buffer_1,
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
              "\n");
  buffer_flush(buffer_1);
}

int
main(int argc, char** argv) {
  uint8* base = NULL;
  size_t filesize;

  int index, c;

  struct longopt opts[] = {
      {"help", 0, NULL, 'h'},
      {"imports", 0, &list_imports, 'i'},
      {"exports", 0, &list_exports, 'e'},
      {"deps", 0, &list_deps, 'd'},
      {"sections", 0, &list_sections, 's'},
      {"export-directory", 0, &print_export_dir, 'E'},
      {"data-directory", 0, &print_data_dir, 'D'},
  };

  for(;;) {
    c = getopt_long(argc, argv, "hieds", opts, &index);
    if(c == -1) break;

    switch(c) {
      case 'h': usage(argv[0]); return 0;
      case 'i':
      case 'e':
      case 'd':
      case 's':
      case 'E':
      case 'D': break;
      default: {
		  usage(argv[0]);
        return 1;
      }
    }
  }

  base = (uint8*)mmap_private(argv[1], &filesize);

  {
    pe32_nt_headers* nt_headers = pe_header_nt(base);

    if(nt_headers->signature != PE_NT_SIGNATURE) {
      buffer_putsflush(buffer_2, "not PE\n");
      return -1;
    }

    // if(!(nt_headers->coff_header.characteristics & PE_FILE_DLL)) {
    // buffer_putsflush(buffer_2, "not DLL\n");
    // return -1;
    // }
    if(list_sections) pe_dump_sections(base);

    if(list_exports) pe_dump_exports(base);
    if(list_imports) pe_dump_imports(base);

    if(print_export_dir) {
      pe_data_directory* data_dir = &pe_header_datadir(base)[PE_DIRECTORY_ENTRY_EXPORT];
      pe_export_directory* export_dir = pe_rva2ptr(base, data_dir->virtual_address);

      pe_print_export_directory(buffer_2, export_dir);
    }
    if(print_data_dir) {
      pe_data_directory* data_dir = pe_header_datadir(base);

      pe_print_data_directory(buffer_2,  data_dir);
    }
  }

  mmap_unmap(base, filesize);

  return 0;
}

void
pe_dump_sections(uint8* base) {
  int i, n;
  pe_section_header* sections = pe_header_sections(base, &n);

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
    buffer_putxlonglong0(buffer_1, sections[i].physical_address, sizeof(sections[i].physical_address) * 2);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, sections[i].virtual_address, sizeof(sections[i].virtual_address) * 2);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, sections[i].size_of_raw_data, sizeof(sections[i].size_of_raw_data) * 2);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, sections[i].pointer_to_raw_data, sizeof(sections[i].pointer_to_raw_data) * 2);
    buffer_putnlflush(buffer_1);
  }
}
