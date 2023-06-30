/* -*- Mode: C; indent-tabs-mode: nil;
 * c-basic-offset: 4 -*- */

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
put_value(buffer* b, const char* name, uint32 v) {
  buffer_puts(b, name);
  buffer_puts(b, ": ");
  put_hex(b, v);

  buffer_putnlflush(b);
}

void
coff_print_func(buffer* b, void* coff, coff_symtab_entry* fn) {
  range ln;
  coff_line_number* p;
  const char* strtab;
  int i;
  uint16 line_number;

  coff_symtab_entry* aux = coff_index_symtab(coff, uint32_get(&fn->func.tag_index));
  coff_symtab_entry* bfef = coff_index_symtab(coff, uint32_get(&fn->func.tag_index) + 1);
  coff_section_header* shdr = coff_get_section(coff, aux->e.scnum);

  if(shdr == NULL)
    return;
  ln = coff_line_numbers(coff, shdr);
  strtab = coff_get_strtab(coff, NULL);
  i = 0;

  line_number = bfef->bfef.source_line_number;

  buffer_putnlflush(b);
  buffer_puts(b, "Line number entries: ");
  buffer_putulong(b, range_size(&ln));
  buffer_puts(b, ", Function start line number: ");
  buffer_putulong(b, line_number);
  buffer_putnlflush(b);

  range_foreach(&ln, p) {

    buffer_putulong0(b, i++, 3);
    buffer_putspace(b);
    if(p->line) {
      buffer_puts(b, "line: ");
      buffer_putulong(b, p->line);
      buffer_puts(b, ", addr: ");
      buffer_putulong(b, p->addr);

      buffer_puts(b, ", offset: ");
      buffer_putxlong0(b, p->addr + uint32_get(&shdr->pointer_to_raw_data), 8);
    } else {
      coff_symtab_entry* sym = coff_index_symtab(coff, p->fname);

      buffer_puts(b, "fname: ");
      buffer_puts(b, coff_symbol_name(coff, sym));
    }
    buffer_putnlflush(b);
  }

  buffer_putnlflush(b);
}

static const char* coff_symtab_mchp_types[] = {"null",
                                               "void",
                                               "character",
                                               "short integer",
                                               "integer",
                                               "long integer",
                                               "floating point",
                                               "double length floating point",
                                               "structure",
                                               "union",
                                               "enumeration",
                                               "member of enumeration",
                                               "unsigned character",
                                               "unsigned short",
                                               "unsigned integer",
                                               "unsigned long"};

void
coff_list_symbols(buffer* b, void* coff) {
  range symtab;
  const char* strtab = coff_get_strtab(coff, NULL);
  coff_file_header* fhdr = coff_header_file(coff);

  char microchip = (fhdr->machine == COFF_FILE_MACHINE_MICROCHIP_V1 || fhdr->machine == COFF_FILE_MACHINE_MICROCHIP_V2);
  coff_symtab_entry* e;
  long i = 0;

  symtab = coff_symbol_table(coff);

  buffer_puts(b,
              "number of symbol table "
              "entries: ");
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

    if(microchip) {
      coff_symtab_entry_microchip* entry = (coff_symtab_entry_microchip*)e;

      if(entry->zeroes != 0)
        stralloc_copyb(&name, entry->name, sizeof(entry->name));
      else
        stralloc_copys(&name, &strtab[entry->offset]);

      stralloc_nul(&name);
      if(((uint16)(uint8)name.s[0]) > 127 || ((uint16)(uint8)name.s[0]) < 32)
        name.s[0] = '\0';

      buffer_putlong0(b, i, 3);
      buffer_putspace(b);
      put_hex(b, entry->value);
      buffer_putspace(b);
      buffer_putspad(b, name.s, 64);
      buffer_puts(b, "0x");
      buffer_putxlong0(b, (long)(uint16)entry->scnum, 4);
      buffer_putspace(b);
      buffer_putspad(b, coff_symtab_mchp_types[entry->type] ? coff_symtab_mchp_types[entry->type] : "", 16);
      buffer_putspace(b);
      buffer_putlong0(b, (long)(uint32)entry->numaux, 2);

      buffer_putspace(b);
      buffer_putnlflush(b);

    } else {

      if(e->e.zeroes != 0)
        stralloc_copyb(&name, e->e.name, sizeof(e->e.name));
      /* else
         stralloc_copys(&name,
         &strtab[e->e.offset]);*/

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

        if(!str_diffn(name.s, ".bf", 3) || !str_diffn(name.s, ".ef", 3)) {
          buffer_puts(b, "Aux .bf/.ef def: ");
          buffer_puts(b, ".source_line_number: ");
          buffer_putulong(b, aux->bfef.source_line_number);
          buffer_puts(b,
                      ", "
                      ".pointer_to_next_"
                      "function: ");
          buffer_putulong(b, aux->bfef.pointer_to_next_function);
          buffer_putnlflush(b);

        } else if(e->e.type & 0x20 && e->e.scnum > 0 && e->e.sclass == COFF_C_EXT) {
          buffer_puts(b,
                      "\t\t\tAux "
                      "function def: ");
          buffer_puts(b,
                      "\n\t\t\t.bf_tag_"
                      "index: ");
          buffer_putulong(b, aux->func.tag_index);

          buffer_puts(b, "\n\t\t\t.code_size: ");
          buffer_putulong(b, aux->func.code_size);
          buffer_puts(b,
                      "\n\t\t\t.pointer_to_"
                      "line_number: 0x");
          buffer_putxlong0(b, aux->func.pointer_to_line_number, 8);
          buffer_puts(b,
                      "\n\t\t\t.pointer_to_"
                      "next_function: ");
          buffer_putulong(b, aux->func.pointer_to_next_function);
          buffer_putnlflush(b);

          coff_print_func(b, coff, aux);
        } else if(e->e.sclass == COFF_C_EXT && e->e.scnum == COFF_SECTION_UNDEF && e->e.value == 0) {
          buffer_puts(b, "Aux weak def: ");
          buffer_puts(b, ".tag_index: ");
          buffer_putulong(b, aux->weak.tag_index);
          buffer_puts(b, ", .characteristics: 0x");
          buffer_putxlong0(b, aux->weak.characteristics, sizeof(aux->weak.characteristics) * 2);

          buffer_putnlflush(b);
        } else if(e->e.sclass == COFF_C_FILE) {
        } else if(e->e.sclass == COFF_C_STATIC) {
          buffer_puts(b,
                      "\n\t\t\tAux "
                      "section def:");
          buffer_puts(b, "\n\t\t\t.length:\t\t");
          buffer_putulong(b, aux->section.length);
          buffer_puts(b,
                      "\n\t\t\t.number_of_"
                      "relocations:\t");
          buffer_putulong(b, aux->section.number_of_relocations);
          buffer_puts(b,
                      "\n\t\t\t.number_of_line_"
                      "numbers:");
          buffer_putulong(b, aux->section.number_of_line_numbers);
          buffer_puts(b,
                      "\n\t\t\t.check_"
                      "sum:\t\t");
          buffer_putxlong0(b, aux->section.check_sum, sizeof(aux->section.check_sum) * 2);
          buffer_puts(b, "\n\t\t\t.number:\t\t");
          buffer_putulong(b, aux->section.number);
          buffer_puts(b,
                      "\n\t\t\t."
                      "selection:\t\t");
          buffer_putulong(b, aux->section.selection);
          buffer_putnlflush(b);
        }
      }
      e = range_plus(&symtab, e, numaux);
      i += numaux;

      ++i;
    }
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
                       "  -h, --help              Show "
                       "this help\n",
                       "  -i, --imports           List "
                       "imports\n",
                       "  -e, --exports           List "
                       "exports\n",
                       "  -d, --deps              List "
                       "DLL dependencies\n",
                       "  -s, --sections          List "
                       "PE32 sections\n",
                       "  -E, --export-directory  Print "
                       "export directory\n",
                       "  -D, --data-directory    Print "
                       "data directory\n",
                       "  -O, --optional-header   Print "
                       "optional header\n",
                       "\n",
                       NULL);
  buffer_flush(buffer_1);
}

int
main(int argc, char** argv) {
  uint8* base = NULL;
  size_t filesize;

  int c, index = 0;

  struct unix_longopt opts[] = {{"help", 0, NULL, 'h'},
                                {"imports", 0, &list_imports, 'i'},
                                {"exports", 0, &list_exports, 'e'},
                                {"deps", 0, &list_deps, 'd'},
                                {"sections", 0, &list_sections, 's'},
                                {"export-directory", 0, &print_export_dir, 'E'},
                                {"data-directory", 0, &print_data_dir, 'D'},
                                {"optional-header", 0, &print_opt_header, 'O'},
                                {0, 0, 0, 0}};

  errmsg_iam(argv[0]);

  buffer_puts(buffer_1, "Number of arguments: ");
  buffer_putlong(buffer_1, argc);
  buffer_putnlflush(buffer_1);

  for(;;) {
    c = unix_getopt_long(argc, argv, "hiedsEDO", opts, &index);
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

  for(; argv[unix_optind]; ++unix_optind) {
    base = (uint8*)mmap_read(argv[unix_optind], &filesize);

    if(base) {
      coff_file_header* header = coff_header_file(base);
      put_value(buffer_2, "COFF_FILE_HEADER\nmachine", uint16_get(&header->machine));
      put_value(buffer_2, "number_of_sections", uint16_get(&header->number_of_sections));
      put_value(buffer_2, "time_date_stamp", uint32_get(&header->time_date_stamp));
      put_value(buffer_2, "pointer_to_symbol_table", uint32_get(&header->pointer_to_symbol_table));
      put_value(buffer_2, "number_of_symbols", uint32_get(&header->number_of_symbols));
      put_value(buffer_2, "size_of_optional_header", uint16_get(&header->size_of_optional_header));
      put_value(buffer_2, "characteristics", uint16_get(&header->characteristics));

      buffer_putnlflush(buffer_2);

      if(header->machine != COFF_FILE_MACHINE_I386 && header->machine != COFF_FILE_MACHINE_AMD64 && header->machine != COFF_FILE_MACHINE_MICROCHIP_V1 &&
         header->machine != COFF_FILE_MACHINE_MICROCHIP_V2) {
        buffer_putsflush(buffer_2, "not COFF\n");
        return -1;
      }

      {
        coff_opt_header* opthdr = coff_header_opt(base);

        if(opthdr->magic == COFF_OPT_MAGIC_MICROCHIP_V1) {
          coff_opt_header_microchip* opthdr_mchp = (void*)opthdr;

          put_value(buffer_2, "COFF_OPT_HEADER\nmagic", uint16_get(&opthdr_mchp->magic));
          put_value(buffer_2, "vstamp", uint16_get(&opthdr_mchp->vstamp));
          put_value(buffer_2,
                    "proc_type",
                    /*uint32_get*/
                    (opthdr_mchp->proc_type));
          put_value(buffer_2,
                    "rom_width_bits",
                    /*uint32_get*/
                    (opthdr_mchp->rom_width_bits));
          put_value(buffer_2,
                    "ram_width_bits",
                    /*uint32_get*/
                    (opthdr_mchp->ram_width_bits));
        }

        // if(!(nt_headers->coff_header.characteristics
        // & COFF_FILE_DLL)) {
        // buffer_putsflush(buffer_2,
        // "not DLL\n"); return -1;
        // }
        //
        /*      if(print_opt_header)
                coff_dump_opthdr(buffer_1,
           base);
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
          coff_data_directory* data_dir
        =
        &coff_header_datadir(base)[COFF_DIRECTORY_ENTRY_EXPORT];
          coff_export_directory*
        export_dir = coff_rva2ptr(base,
        data_dir->virtual_address);

          coff_print_export_directory(buffer_2,
        base, export_dir);
        }
        if(print_data_dir) {
          uint32 num_dirs;
          coff_data_directory* data_dir
        = coff_get_datadir(base,
        &num_dirs);

            coff_print_data_directories(buffer_2,
        base, data_dir, num_dirs);
        }
        */
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
