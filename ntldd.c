/*
    ntldd - lists dynamic dependencies of a module

    Copyright (C) 2010 LRN

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
Code is mostly written after
"An In-Depth Look into the Win32 Portable Executable File Format"
MSDN Magazine articles
*/

//#include <windows.h>

//#include "lib/imagehlp.h"
#include "lib/getopt.h"
#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/str.h"
#include "lib/uint64.h"

//#include <winnt.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <inttypes.h>

#include "libntldd.h"

#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif

void
printversion() {
  buffer_puts(buffer_1, "ntldd ");
  buffer_putlong(buffer_1, NTLDD_VERSION_MAJOR);
  buffer_puts(buffer_1, ".");
  buffer_putlong(buffer_1, NTLDD_VERSION_MINOR);
  buffer_puts(buffer_1, " \n\
Copyright (C) 2010-2015 LRN\n\
This is free software; see the source for conditions. There is NO\n\
warranty; not event for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n\
Written by LRN.");
  buffer_putnlflush(buffer_1);
}

void
printhelp(char* argv0) {
  buffer_putm(buffer_1, "Usage: ", argv0, " [OPTION]... FILE...\n\
OPTIONS:\n\
--version         Displays version\n\
-v, --verbose         Does not work\n\
-u, --unused          Does not work\n\
-d, --data-relocs     Does not work\n\
-r, --function-relocs Does not work\n\
-R, --recursive       Lists dependencies recursively,\n\
                        eliminating duplicates\n\
-D, --search-dir      Additional search directory\n\
--list-exports        Lists exports of a module (single file only)\n\
--list-imports        Lists imports of modules\n\
--help                Displays this message\n\
\n\
Use -- option to pass filenames that start with `--' or `-'\n\
For bug reporting instructions, please see:\n\
<somewhere>.");
  buffer_putnlflush(buffer_1);
}

int
print_image_links(int first,
                  int verbose,
                  int unused,
                  int datarelocs,
                  int functionrelocs,
                  struct dep_tree_element* self,
                  int recursive,
                  int list_exports,
                  int list_imports,
                  int depth) {
  uint64 i;
  int unresolved = 0;
  self->flags |= DEPTREE_VISITED;

  if(list_exports) {
    for(i = 0; i < self->exports_len; i++) {
      struct export_table_item* item = &self->exports[i];

      buffer_putnspace(buffer_1, depth * 2);
      buffer_puts(buffer_1, "[");
      buffer_putulong0(buffer_1, item->ordinal, 3);
      buffer_puts(buffer_1, "] ");
      buffer_putspad(buffer_1, item->name, 16);
      buffer_puts(buffer_1, " (0x");
      buffer_putxlonglong0(buffer_1, item->address_offset, 8);
      buffer_putm(buffer_1, item->forward_str ? " ->" : "", item->forward_str ? item->forward_str : "", " <");
      buffer_putulong(buffer_1, item->section_index);
      buffer_puts(buffer_1, ">");
      buffer_putnlflush(buffer_1);
      /*
            printf("%*s[%u] %s (0x%lx)%s%s <%d>\n",
                   depth,
                   depth > 0 ? " " : "",
                   item->ordinal,
                   item->name,
                   (long)item->address_offset,
                   item->forward_str ? " ->" : "",
                   item->forward_str ? item->forward_str : "",
                   item->section_index);*/
    }
    return 0;
  }
  if(self->flags & DEPTREE_UNRESOLVED) {
    if(!first) {
      buffer_putsflush(buffer_1, " => not found\n");
    } else {
      buffer_putm(buffer_2, self->module, ": not found\n");
      buffer_flush(buffer_2);
    }
    unresolved = 1;
  }

  if(!unresolved && !first) {
    if(str_case_diff(self->module, self->resolved_module) == 0) {
      buffer_puts(buffer_1, " (0x");
      buffer_putxlonglong0(buffer_1, self->mapped_address, 8);
    } else {
      buffer_putm(buffer_1, " => ", self->resolved_module, " (0x");
      buffer_putxlonglong0(buffer_1, self->mapped_address, 8);
    }
    buffer_putsflush(buffer_1, ")\n");
  }

  if(list_imports) {
    for(i = 0; i < self->imports_len; i++) {
      struct import_table_item* item = &self->imports[i];

      buffer_puts(buffer_1, "\t");
      buffer_putnspace(buffer_1, depth * 2);
      buffer_putxlonglong0(buffer_1, item->orig_address, 8);
      buffer_putspace(buffer_1);
      buffer_putxlonglong0(buffer_1, item->address, 8);
      buffer_putspace(buffer_1);
      buffer_putulong0(buffer_1, item->ordinal, 3);
      buffer_putspace(buffer_1);
      buffer_puts(buffer_1, item->name ? item->name : "<NULL>");
      buffer_putspace(buffer_1);
      buffer_puts(buffer_1, item->mapped ? "" : "<UNRESOLVED>");
      buffer_putspace(buffer_1);
      buffer_puts(buffer_1, item->dll == NULL ? "<MODULE MISSING>" : item->dll->module ? item->dll->module : "<NULL>");
      buffer_putnlflush(buffer_1);
      /*printf("\t%*s%llx %llx %3d %s %s %s\n",
             depth,
             depth > 0 ? " " : "",
             (long long)item->orig_address,
             (long long)item->address,
             item->ordinal,
             item->name ? item->name : "<NULL>",
             item->mapped ? "" : "<UNRESOLVED>",
             item->dll == NULL ? "<MODULE MISSING>" : item->dll->module ? item->dll->module : "<NULL>");*/
    }
  }

  if(unresolved) return -1;

  if(first || recursive) {
    for(i = 0; i < self->childs_len; i++) {
      if(!(self->childs[i]->flags & DEPTREE_VISITED)) {
        buffer_putnspace(buffer_1, depth * 2);
        buffer_puts(buffer_1, self->childs[i]->module);
        print_image_links(0,
                          verbose,
                          unused,
                          datarelocs,
                          functionrelocs,
                          self->childs[i],
                          recursive,
                          list_exports,
                          list_imports,
                          depth + 1);
      }
    }
  }
  return 0;
}
void
add_path(strlist* sp, const char* path) {
  char s = (strchr(path, '\\') && strchr(path, ':') && strchr(path, ";")) ? ';' : ':';
  stralloc dir;
  strlist tmp;
  stralloc_init(&dir);
  strlist_init(&tmp, '\0');
  strlist_froms(&tmp, path, s);

  __strlist_foreach(&tmp, path) {
    stralloc_copys(&dir, path);
    stralloc_catc(&dir, path[0]);
    strlist_push_unique_sa(sp, &dir);
  }
  strlist_free(&tmp);
}

int
main(int argc, char** argv) {
  int i;
  int verbose = 0;
  int unused = 0;
  int datarelocs = 0;
  int functionrelocs = 0;
  int skip = 0;
  int files = 0;
  int recursive = 0;
  int list_exports = 0;
  int list_imports = 0;
  int files_start = -1;
  int files_count = 0;
  int c;
  int digit_optind = 0;
  const char* rel_to = NULL;
  int index = 0;
  struct longopt opts[] = {
      {"help", 0, NULL, 'h'},
   {"verbose", 0, &verbose, 'v'},
  {"unused", 0, &unused, 'u'},
  {"data-relocs", 0, &datarelocs, 'd'},
  {"function-relocs", 0, &functionrelocs, 'r'},
  {"recursive", 0, &recursive, 'R'},
  {"list-exports", 0, &list_exports, 'e'},
  {"list-imports", 0, &list_imports, 'i'},
  {"version", 0, NULL, 'V'},

  };


  strlist sp;
  strlist_init(&sp, '\0');
  // byte_zero(&sp, sizeof(sp));
  // sp.path = calloc(1, sizeof(char*));
  for(;;) {
    c = getopt_long(argc, argv, "hvudrRei", opts, &index);
    if(c == -1) break;
    switch(c) {
      case 'h': /*usage(argv[0]); */ return 0;
      case 'v':
      case 'u':
      case 'd':
      case 'r':
      case 'R':
      case 'e':
      case 'i':
                break;
      case 'V':
      printversion(); break;
      default: /*usage(argv[0]);*/ return 1;
    }
  }
/*
  for(i = 1; i < argc; i++) {
    if(str_equal(argv[i], "--version"))
      printversion();
    else if(str_equal(argv[i], "-v") || str_equal(argv[i], "--verbose"))
      verbose = 1;
    else if(str_equal(argv[i], "-u") || str_equal(argv[i], "--unused"))
      unused = 1;
    else if(str_equal(argv[i], "-d") || str_equal(argv[i], "--data-relocs"))
      datarelocs = 1;
    else if(str_equal(argv[i], "-r") || str_equal(argv[i], "--function-relocs"))
      functionrelocs = 1;
    else if(str_equal(argv[i], "-R") || str_equal(argv[i], "--recursive"))
      recursive = 1;
    else if(str_equal(argv[i], "-e") || str_equal(argv[i], "--list-exports"))
      list_exports = 1;
    else if(str_equal(argv[i], "-i") || str_equal(argv[i], "--list-imports"))
      list_imports = 1;
    else if((str_equal(argv[i], "-D") || str_equal(argv[i], "--search-dir")) && i < argc - 1) {
      char* add_dirs = argv[i + 1];
      add_path(&sp, add_dirs);
      i++;
    } else if(str_equal(argv[i], "--help")) {
      printhelp(argv[0]);
      skip = 1;
      break;
    } else if(str_equal(argv[i], "--")) {
      files = 1;
    } else if(str_len(argv[i]) > 1 && argv[i][0] == '-' && (argv[i][1] == '-' || str_len(argv[i]) == 2) && !files) {
      buffer_putm(buffer_2, "Unrecognized option `", argv[i], "'\n", "Try `ntldd --help' for more information");
      buffer_putnlflush(buffer_2);
      skip = 1;
      break;
    } else if(files_start < 0) {
      skip = 0;
      files_start = i;
      break;
    }
  }*/
  skip = 0;
      files_start = optind;
  {
    const char* pathenv = getenv("PATH");
    if(pathenv) add_path(&sp, pathenv);
  }

  if(!skip && files_start > 0) {
    files_count = argc - files_start;
    for(i = 0; i < files_count; ++i) {
      char* p;
      char buff[MAX_PATH];
      str_copyn(buff, argv[files_start + i], sizeof(buff));
      p = str_basename(buff);
      if(p) *p = '\0';
      strlist_push_unique(&sp, buff);
    }
    buffer_puts(buffer_2, "PATH=");
    strlist_dump(buffer_2, &sp);
    {
      int multiple = files_start + 1 < argc;
      struct dep_tree_element root;
      byte_zero(&root, sizeof(struct dep_tree_element));
      for(i = files_start; i < argc; i++) {
        struct dep_tree_element* child = (struct dep_tree_element*)malloc(sizeof(struct dep_tree_element));
        byte_zero(child, sizeof(struct dep_tree_element));
        child->module = str_dup(argv[i]);

        add_dep(&root, child);
        {
          char** stack = NULL;
          uint64 stack_len = 0;
          uint64 stack_size = 0;
          build_tree_config cfg;
          byte_zero(&cfg, sizeof(cfg));
          cfg.machine_type = -1;
          cfg.on_self = 0;
          cfg.datarelocs = datarelocs;
          cfg.recursive = recursive;
          cfg.functionrelocs = functionrelocs;
          cfg.stack = &stack;
          cfg.stack_len = &stack_len;
          cfg.stack_size = &stack_size;
          cfg.search_paths = &sp;

          build_dep_tree(&cfg, argv[i], &root, child);
        }
      }

      clear_dep_status(&root, DEPTREE_VISITED | DEPTREE_PROCESSED);
      for(i = files_start; i < argc; i++) {
        if(multiple) {
          buffer_puts(buffer_1, argv[i]);
          buffer_putsflush(buffer_1, ":\n");
        }
        print_image_links(1,
                          verbose,
                          unused,
                          datarelocs,
                          functionrelocs,
                          root.childs[i - files_start],
                          recursive,
                          list_exports,
                          list_imports,
                          0);
      }
    }
  }
  return 0;
}
