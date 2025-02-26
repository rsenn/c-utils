/* -*- Mode: C; indent-tabs-mode: nil;
 * c-basic-offset: 4 -*- */

#include "lib/uint32.h"
#include "lib/macho.h"
#include "lib/mmap.h"
#include "lib/str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char** argv) {
  char* content;
  size_t length;
  size_t offset;
  uint32 i, n_commands;
  uint32 section_index = 0, text_section_index = 0;

  if(argc < 2) {
    printf("Usage: %s XXX.macho_dylib\n", argv[0]);
    return 0;
  }

  content = mmap_private(argv[1], &length);

  {
    macho_mach_header* header;

    header = (macho_mach_header*)content;

    if(header->magic != MACHO_MH_MAGIC) {
      printf("not Mach-O 32\n");
      return -1;
    }

    offset = sizeof(*header);
    n_commands = header->ncmds;
  }

  for(i = 0; i < n_commands; i++) {
    macho_load_command* load;

    load = (macho_load_command*)(content + offset);

    switch(load->cmd) {
      case MACHO_LC_SEGMENT: {
        macho_segment_command* segment;
        macho_section* section;
        size_t j;

        segment = (macho_segment_command*)(content + offset);

        if(!!str_diff(segment->segname, "__TEXT")) {
          section_index += segment->nsects;
          break;
        }

        section = (macho_section*)(content + offset + sizeof(macho_segment_command));

        for(j = 0; j < segment->nsects; j++, section++) {
          section_index++;

          if(!str_diff(section->sectname, "__text"))
            text_section_index = section_index;
        }
        break;
      }
      case MACHO_LC_SYMTAB: {
        macho_symtab_command* table;
        macho_nlist* symbol;
        char* string_table;
        size_t j;

        table = (macho_symtab_command*)(content + offset);
        symbol = (macho_nlist*)(content + table->symoff);
        string_table = content + table->stroff;

        for(j = 0; j < table->nsyms; j++, symbol++) {
          int defined_in_section = 0;

          if((symbol->n_type & MACHO_N_TYPE) == MACHO_N_SECT)
            defined_in_section = 1;

          if(defined_in_section && symbol->n_sect == text_section_index && symbol->n_type & MACHO_N_EXT) {
            char* name;
            int32 string_offset;

            string_offset = symbol->n_strx;
            name = string_table + string_offset;
            printf("found: %s\n", name + 1);
          }
        }
        break;
      }
      default: break;
    }
    offset += load->cmdsize;
  }

  free(content);

  return 0;
}
