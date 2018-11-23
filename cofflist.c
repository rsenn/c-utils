/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "lib/unix.h"
#include "lib/uint64.h"
#include "lib/buffer.h"
#include "lib/mmap.h"
#include "lib/coff.h"
#include "lib/str.h"
#include "lib/stralloc.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/errmsg.h"

#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

int list_imports, list_exports, list_deps, list_sections;
static int print_export_dir, print_data_dir, print_opt_header;

void coff_dump_sections(uint8* base);

void
put_hex(buffer* b, uint32 v) {
  if(v) {
    buffer_puts(b, "0x");
    buffer_putxlong0(b, v, sizeof(v) * 2);
  } else {
    buffer_putnspace(b, sizeof(v) * 2 + 2);
  }
}

void
coff_list_symbols(buffer* b, void* coff) {
  range symtab;
  const char* strtab = coff_get_strtab(coff, NULL);
  coff_symtab_entry* e;
  long i = 0;

  symtab = coff_symbol_table(coff);

  buffer_puts(b, "number of symbol table entries: ");
  buffer_putulong(b, range_size(&symtab));
  buffer_putnlflush(b);

  buffer_puts(b, "size of symbol table entries: ");
  buffer_putulong(b, symtab.elem_size);
  buffer_putnlflush(b);

  buffer_putspad(b, "num", 4);
  buffer_putspad(b, "address", 11);
  buffer_putspad(b, "name", 64);
  buffer_putspad(b, "s#", 3);
  buffer_putspad(b, "class", 16);
  buffer_putspad(b, "#aux", 5);
  buffer_putspad(b, "size", 6);
  buffer_putnlflush(b);

  range_foreach(&symtab, e) {
    coff_symtab_entry* aux = e;
    coff_symtab_entry* fn = NULL;
    uint16 j, numaux = e->e.numaux;
    stralloc name;
    stralloc_init(&name);

    if(e->e.zeroes != 0)
      stralloc_copyb(&name, e->e.name, sizeof(e->e.name));
    else
      stralloc_copys(&name, &strtab[e->e.offset]);

    if(e->e.sclass == 0) {
      stralloc_zero(&name);
    } else if(e->e.type & 0x20 && e->e.scnum > 0 && e->e.sclass == COFF_C_EXT) {
      //   stralloc_cats(&name, "()");
      fn = range_plus(&symtab, e, 1);
    } 
    
    

    stralloc_nul(&name);


    if(!isspace(name.s[0]) && name.s[0] != 0x08) {
      const char* sclass = coff_sclass_name(e->e.sclass);
      buffer_putlong0(b, i, 3);
      buffer_putspace(b);
      put_hex(b, e->e.value);
      buffer_putspace(b);
      buffer_putspad(b, name.s, 64);
      buffer_putlong0(b, (long)(int16)e->e.scnum, 2);
      buffer_putspace(b);
      buffer_putspad(b, sclass ? sclass : "(null)", 16);
      buffer_putlong0(b, e->e.numaux, 4);
      if(fn) {
        buffer_putspace(b);
        buffer_putulong0(b, (unsigned long)fn->func.code_size, 6);
        buffer_putspace(b);
        buffer_putxlong0(b, (unsigned long)fn->func.tag_index, 6);
      }
      buffer_putnlflush(b);
    }

    for(j = 0; j < numaux; ++j) {
      aux = range_plus(&symtab, e, 1 + j);

      if(e->e.type & 0x20 && e->e.scnum > 0 && e->e.sclass == COFF_C_EXT) {
        buffer_puts(b, "Aux function def: ");
        buffer_puts(b, ".bf_tag_index: ");
        buffer_putulong(b, aux->func.tag_index);
        buffer_puts(b, ", .code_size: ");
        buffer_putulong(b, aux->func.code_size);
        buffer_puts(b, ", .pointer_to_line_number: ");
        buffer_putulong(b, aux->func.pointer_to_line_number);
        buffer_puts(b, ", .pointer_to_next_function: ");
        buffer_putulong(b, aux->func.pointer_to_next_function);
        buffer_putnlflush(b);
      } else if(e->e.sclass == COFF_C_EXT && e->e.scnum == COFF_SECTION_UNDEF && e->e.value == 0) {
        buffer_puts(b, "Aux weak def: ");
        buffer_puts(b, ".tag_index: ");
        buffer_putulong(b, aux->weak.tag_index);
        buffer_puts(b, ", .characteristics: 0x");
        buffer_putxlong0(b, aux->weak.characteristics, sizeof(aux->weak.characteristics)*2);

        buffer_putnlflush(b);
       } else if(e->e.sclass == COFF_C_FILE) {
       } else if(e->e.sclass == COFF_C_STATIC) {
        buffer_puts(b, "Aux section def: ");
        buffer_puts(b, ".length: ");
        buffer_putulong(b, aux->section.length);
        buffer_puts(b, ", .number_of_relocations: ");
        buffer_putulong(b, aux->section.number_of_relocations);
        buffer_puts(b, ", .number_of_line_numbers: ");
        buffer_putulong(b, aux->section.number_of_line_numbers);
        buffer_puts(b, ", .check_sum: ");
        buffer_putxlong0(b, aux->section.check_sum, sizeof(aux->section.check_sum) * 2);
        buffer_puts(b, ", .number: ");
        buffer_putulong(b, aux->section.number);
        buffer_puts(b, ", .selection: ");
        buffer_putulong(b, aux->section.selection);
        buffer_putnlflush(b);
       }
    }
    e = range_plus(&symtab, e, numaux);
    i += numaux;

    ++i;
  }
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
    list_sections = list_imports = list_exports = 1;

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

      if(list_imports || list_exports)
        coff_list_symbols(buffer_1, base);
      /*
      if(list_imports)
        coff_dump_imports(base);

       if(list_exports)
         coff_dump_exports(base);
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

  buffer_putspad(buffer_1, "num", 3);
  buffer_putspace(buffer_1);
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
    buffer_putlong0(buffer_1, i + 1, 3);
    buffer_putspace(buffer_1);
    buffer_putspad(buffer_1, sections[i].name, 16);
    buffer_putspace(buffer_1);
    put_hex(buffer_1, uint32_get(&sections[i].physical_address));
    buffer_putspace(buffer_1);
    put_hex(buffer_1, uint32_get(&sections[i].virtual_address));
    buffer_putspace(buffer_1);
    put_hex(buffer_1, uint32_get(&sections[i].size_of_raw_data));
    buffer_putspace(buffer_1);
    put_hex(buffer_1, uint32_get(&sections[i].pointer_to_raw_data));
    buffer_putnlflush(buffer_1);
  }
}