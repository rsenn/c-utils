/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include <glib.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <stdio.h>
#include <string.h>

int
main(int argc, char** argv) {
  gchar* content;
  gsize length;
  gsize offset;
  uint32_t i, n_commands;
  uint32_t section_index = 0, text_section_index = 0;

  if(argc < 2) {
    g_print("Usage: %s XXX.dylib\n", argv[0]);
    return 0;
  }

  g_file_get_contents(argv[1], &content, &length, NULL);

  {
    struct mach_header* header;

    header = (struct mach_header*)content;
    if(header->magic != MH_MAGIC) {
      g_print("not Mach-O 32\n");
      return -1;
    }

    offset = sizeof(*header);
    n_commands = header->ncmds;
  }

  for(i = 0; i < n_commands; i++) {
    struct load_command* load;

    load = (struct load_command*)(content + offset);
    switch(load->cmd) {
      case LC_SEGMENT: {
        struct segment_command* segment;
        struct section* section;
        gint j;

        segment = (struct segment_command*)(content + offset);
        if(!g_str_equal(segment->segname, "__TEXT")) {
          section_index += segment->nsects;
          break;
        }

        section = (struct section*)(content + offset + sizeof(*segment));
        for(j = 0; j < segment->nsects; j++, section++) {
          section_index++;
          if(g_str_equal(section->sectname, "__text")) text_section_index = section_index;
        }
        break;
      }
      case LC_SYMTAB: {
        struct symtab_command* table;
        struct nlist* symbol;
        gchar* string_table;
        gint j;

        table = (struct symtab_command*)(content + offset);
        symbol = (struct nlist*)(content + table->symoff);
        string_table = content + table->stroff;
        for(j = 0; j < table->nsyms; j++, symbol++) {
          gboolean defined_in_section = FALSE;

          if((symbol->n_type & N_TYPE) == N_SECT) defined_in_section = TRUE;

          if(defined_in_section && symbol->n_sect == text_section_index && symbol->n_type & N_EXT) {
            gchar* name;
            int32_t string_offset;

            string_offset = symbol->n_un.n_strx;
            name = string_table + string_offset;
            g_print("found: %s\n", name + 1);
          }
        }
        break;
      }
      default: break;
    }
    offset += load->cmdsize;
  }

  g_free(content);

  return 0;
}
