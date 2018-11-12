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

#include "lib/windoze.h"
#include "lib/uint64.h"
#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/unix.h"
#include "lib/path_internal.h"
#include "lib/str.h"
#include "lib/stralloc.h"
#include "lib/pe.h"
#include "lib/mmap.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libntldd.h"

#if WINDOWS
#include <windows.h>
#endif

#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif

#ifndef RRF_RT_ANY
#define RRF_RT_ANY 0xffff
#endif

static stralloc cwd;

#if defined(__CYGWIN__) || defined(__MSYS__)
#include <sys/cygwin.h>
#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#if !defined(__MSYS__) && !defined(HAVE_CYGWIN_CONV_PATH)
#define HAVE_CYGWIN_CONV_PATH 1
#endif

#ifdef HAVE_CYGWIN_CONV_PATH
#define cygwin_conv_to_full_posix_path(from, to)                                                                       \
  cygwin_conv_path(CCP_WIN_A_TO_POSIX | CCP_ABSOLUTE, (from), (to), MAX_PATH)
#endif

void
pathconv(const char* path, stralloc* sa) {
  stralloc_ready(sa, MAX_PATH);
  cygwin_conv_to_full_posix_path(path, sa->s);
  sa->len = str_len(sa->s);
}
#else
void
pathconv(const char* path, stralloc* sa) {
  stralloc_copys(sa, path);
}
#endif

/*****************************************************************************
 * Function resize_array
 *****************************************************************************/
void
resize_array(void** data, uint64* data_size, size_t sizeof_data) {
  uint64 new_size = (*data_size) > 0 ? (*data_size) * 2 : 64;
  void* new_data;
  new_data = realloc(*data, new_size * sizeof_data);
  byte_zero(((unsigned char*)new_data) + (*data_size * sizeof_data), (new_size - (*data_size)) * sizeof_data);
  *data = new_data;
  *data_size = new_size;
}

#define resize_dep_list(ptr_deptree, ptr_deptree_size)                                                                 \
  resize_array((void**)ptr_deptree, ptr_deptree_size, sizeof(struct dep_tree_element*))
#define resize_import_list(ptr_import_list, ptr_import_list_size)                                                      \
  resize_array((void**)ptr_import_list, ptr_import_list_size, sizeof(struct import_table_item))
#define resize_stack(ptr_stack, ptr_stack_size) resize_array((void**)ptr_stack, ptr_stack_size, sizeof(char*))

void
add_dep(struct dep_tree_element* parent, struct dep_tree_element* child) {
  if(parent->childs_len >= parent->childs_size) {
    resize_dep_list(&parent->childs, &parent->childs_size);
  }
  parent->childs[parent->childs_len] = child;
  parent->childs_len += 1;
}

struct import_table_item*
add_import(struct dep_tree_element* self) {
  if(self->imports_len >= self->imports_size) {
    resize_import_list(&self->imports, &self->imports_size);
  }
  self->imports_len += 1;
  return &self->imports[self->imports_len - 1];
}

int
find_dep(struct dep_tree_element* root, char* name, struct dep_tree_element** result) {
  int ret = -1;
  uint64 i;
  if(root->flags & DEPTREE_VISITED) {
    return -2;
  }
  root->flags |= DEPTREE_VISITED;
  for(i = 0; i < root->childs_len; i++) {
    if(str_case_diff(root->childs[i]->module, name) == 0) {
      if(result != NULL)
        *result = root->childs[i];
      root->flags &= ~DEPTREE_VISITED;
      return (root->childs[i]->flags & DEPTREE_UNRESOLVED) ? 1 : 0;
    }
  }
  for(i = 0; i < root->childs_len && ret < 0; i++) {
    ret = find_dep(root->childs[i], name, result);
  }
  root->flags &= ~DEPTREE_VISITED;
  return ret;
}

int build_dep_tree(build_tree_config* cfg, char* name, struct dep_tree_element* root, struct dep_tree_element* self);

struct dep_tree_element*
process_dep(build_tree_config* cfg, uint32 name, struct dep_tree_element* root, struct dep_tree_element* self, int deep) {
  struct dep_tree_element* child = NULL;
  int found;
  int64 i;
  char* dllname = pe_rva2ptr(self->mapped_address, uint32_get(&name));
  if(dllname == NULL)
    return NULL;
  if(str_len(dllname) > 10 && str_case_diffn("api-ms-win", dllname, 10) == 0) {
    /* TODO: find a better way to identify api stubs. Versioninfo, maybe? */
    return NULL;
  }
  for(i = (int64)*cfg->stack_len - 1; i >= 0; i--) {
    if((*cfg->stack)[i] && str_case_diff((*cfg->stack)[i], dllname) == 0)
      return NULL;
    if(i == 0)
      break;
  }
  found = find_dep(root, dllname, &child);
  if(found < 0) {
    child = (struct dep_tree_element*)malloc(sizeof(struct dep_tree_element));
    byte_zero(child, sizeof(struct dep_tree_element));
    if(deep == 0) {
      child->module = str_dup(dllname);

      add_dep(self, child);
    }
  }
  if(deep == 1) {

    build_dep_tree(cfg, dllname, root, child);
  }
  return child;
}

/*
struct export_table_item *find_export_forward (struct dep_tree_element *self, char *dllname, char *export_name, uint32
export_ordinal)
{
  return NULL;
}
*/

int
clear_dep_status(struct dep_tree_element* self, uint64 flags) {
  uint64 i;
  for(i = 0; i < self->childs_len; i++) clear_dep_status(self->childs[i], flags);
  self->flags &= ~flags;
  return 0;
}

void
push_stack(char*** stack, uint64* stack_len, uint64* stack_size, char* name) {
  if(*stack_len >= *stack_size) {
    resize_stack(stack, stack_size);
  }
  (*stack)[*stack_len] = str_dup(name);
  (*stack_len) += 1;
}

void
pop_stack(char*** stack, uint64* stack_len, uint64* stack_size, char* name) {
  (*stack)[*stack_len] = NULL;
  (*stack_len) -= 1;
}

static uint64
thunk_data_u1_function(void* thunk_array, uint32 index, build_tree_config* cfg) {
  if(cfg->machine_type == PE_FILE_MACHINE_I386)
    return ((uint32*)thunk_array)[index];
  else
    return ((uint64*)thunk_array)[index];
}
/*
static void*
opt_header_get_dd_entry(void* opt_header, uint32 entry_type, build_tree_config* cfg) {
  if(cfg->machine_type == PE_FILE_MACHINE_I386)
    return &(((pe32_opt_header*)opt_header)->data_directory[entry_type]);
  else
    return &(((pe64_opt_header*)opt_header)->data_directory[entry_type]);
}*/

static void
build_dep_tree32or64(pe_loaded_image* img,
                     build_tree_config* cfg,
                     struct dep_tree_element* root,
                     struct dep_tree_element* self) {
  pe_data_directory* idata;
  pe_import_descriptor* iid;
  pe_export_directory* ied;
  pe_delayload_descriptor* idd;
  void *ith, *oith;
  void* opt_header = pe_header_opt(img->base);
  uint32 i, j;

  idata = &pe_get_datadir(img->base, NULL)[PE_DIRECTORY_ENTRY_EXPORT];
  if(idata->size > 0 && idata->virtual_address != 0) {
    ied = pe_rva2ptr(img->base, uint32_get(&idata->virtual_address));
    if(ied && ied->name != 0) {
      char* export_module = pe_rva2ptr(img->base, uint32_get(&ied->name));
      if(export_module != NULL) {
        if(self->export_module == NULL)
          self->export_module = str_dup(export_module);
      }
    }
    if(ied && ied->number_of_functions > 0) {
      uint32 *addrs, *names;
      uint16* ords;
      self->exports_len = uint32_get(&ied->number_of_functions);
      self->exports = (struct export_table_item*)malloc(sizeof(struct export_table_item) * self->exports_len);
      byte_zero(self->exports, sizeof(struct export_table_item) * self->exports_len);
      addrs = pe_rva2ptr(img->base, uint32_get(&ied->address_of_functions));
      ords = pe_rva2ptr(img->base, uint32_get(&ied->address_of_name_ordinals));
      names = pe_rva2ptr(img->base, uint32_get(&ied->address_of_names));
      for(i = 0; i < ied->number_of_names; i++) {
        self->exports[ords[i]].ordinal = ords[i] + ied->base;
        if(names[i] != 0) {
          char* s_name = pe_rva2ptr(img->base, uint32_get(&names[i]));
          if(s_name != NULL)
            self->exports[ords[i]].name = str_dup(s_name);
        }
      }
      for(i = 0; i < ied->number_of_functions; i++) {
        if(addrs[i] != 0) {
          int section_index = pe_rva2section(img->base, addrs[i]);
          if((idata->virtual_address <= addrs[i]) && (idata->virtual_address + idata->size > addrs[i])) {
            self->exports[i].address = NULL;
            self->exports[i].forward_str = str_dup(pe_rva2ptr(img->base, uint32_get(&addrs[i])));
          } else
            self->exports[i].address = pe_rva2ptr(img->base, uint32_get(&addrs[i]));
          self->exports[i].ordinal = i + ied->base;
          self->exports[i].section_index = section_index;
          self->exports[i].address_offset = addrs[i];
        }
      }
    }
  }

  idata = &pe_get_datadir(img->base, NULL)[PE_DIRECTORY_ENTRY_IMPORT];
  if(idata->size > 0 && idata->virtual_address != 0) {
    iid = pe_rva2ptr(img->base, uint32_get(&idata->virtual_address));
    if(iid)
      for(i = 0; iid[i].characteristics || iid[i].time_date_stamp || iid[i].forwarder_chain || iid[i].name ||
                 iid[i].first_thunk;
          i++) {
        struct dep_tree_element* dll;
        uint64 impaddress;
        dll = process_dep(cfg, iid[i].name, root, self, 0);
        if(dll == NULL)
          continue;
        ith = pe_rva2ptr(img->base, uint32_get(&iid[i].first_thunk));
        oith = pe_rva2ptr(img->base, uint32_get(&iid[i].original_first_thunk));
        for(j = 0; (impaddress = thunk_data_u1_function(ith, j, cfg)) != 0; j++) {
          struct import_table_item* imp = add_import(self);
          imp->dll = dll;
          imp->ordinal = -1;
          if(oith) {
          }
          imp->orig_address = thunk_data_u1_function(oith, j, cfg);
          if(cfg->on_self) {
            imp->address = impaddress;
          }
          if(oith && imp->orig_address & (1 << (sizeof(uint32) * 8 - 1))) {
            imp->ordinal = imp->orig_address & ~(1 << (sizeof(uint32) * 8 - 1));
          } else if(oith) {
            pe_import_by_name* byname = pe_rva2ptr(img->base, uint32_get(&imp->orig_address));
            if(byname != NULL)
              imp->name = str_dup((char*)byname->name);
          }
        }
      }
  }

  idata = &pe_get_datadir(img->base, NULL)[PE_DIRECTORY_ENTRY_DELAY_IMPORT];
  if(idata->size > 0 && idata->virtual_address != 0) {
    idd = pe_rva2ptr(img->base, uint32_get(&idata->virtual_address));
    if(idd)
      for(i = 0; idd[i].attributes.all_attributes || idd[i].dll_name_rva || idd[i].module_handle_rva ||
                 idd[i].import_address_table_rva || idd[i].import_name_table_rva ||
                 idd[i].bound_import_address_table_rva || idd[i].unload_information_table_rva || idd[i].time_date_stamp;
          i++) {
        struct dep_tree_element* dll;
        uint64 impaddress;
        dll = process_dep(cfg, idd[i].dll_name_rva, root, self, 0);
        if(dll == NULL)
          continue;
        if(idd[i].attributes.all_attributes & 0x00000001) {
          ith = pe_rva2ptr(img->base, uint32_get(&idd[i].import_address_table_rva));
          oith = pe_rva2ptr(img->base, uint32_get(&idd[i].import_name_table_rva));
        } else {
          ith = (void*)(uintptr_t)idd[i].import_address_table_rva;
          oith = (void*)(uintptr_t)idd[i].import_name_table_rva;
        }
        for(j = 0; (impaddress = thunk_data_u1_function(ith, j, cfg)) != 0; j++) {
          struct import_table_item* imp = add_import(self);
          imp->dll = dll;
          imp->ordinal = -1;
          if(oith)
            imp->orig_address = thunk_data_u1_function(oith, j, cfg);
          if(cfg->on_self) {
            imp->address = impaddress;
          }
          if(oith && imp->orig_address & (1 << (sizeof(uint32) * 8 - 1))) {
            imp->ordinal = imp->orig_address & ~(1 << (sizeof(uint32) * 8 - 1));
          } else if(oith) {
            pe_import_by_name* byname = pe_rva2ptr(img->base, uint32_get(&imp->orig_address));
            if(byname != NULL)
              imp->name = str_dup((char*)byname->name);
          }
        }
      }
  }

  idata = &pe_get_datadir(img->base, NULL)[PE_DIRECTORY_ENTRY_IMPORT];
  if(idata->size > 0 && idata->virtual_address != 0) {
    iid = pe_rva2ptr(img->base, uint32_get(&idata->virtual_address));
    if(iid)
      for(i = 0; iid[i].characteristics || iid[i].time_date_stamp || iid[i].forwarder_chain || iid[i].name ||
                 iid[i].first_thunk;
          i++)

        process_dep(cfg, iid[i].name, root, self, 1);
  }

  idata = &pe_get_datadir(img->base, NULL)[PE_DIRECTORY_ENTRY_DELAY_IMPORT];
  if(idata->size > 0 && idata->virtual_address != 0) {
    idd = pe_rva2ptr(img->base, uint32_get(&idata->virtual_address));
    if(idd)
      for(i = 0; idd[i].attributes.all_attributes || idd[i].dll_name_rva || idd[i].module_handle_rva ||
                 idd[i].import_address_table_rva || idd[i].import_name_table_rva ||
                 idd[i].bound_import_address_table_rva || idd[i].unload_information_table_rva || idd[i].time_date_stamp;
          i++)

        process_dep(cfg, idd[i].dll_name_rva, root, self, 1);
  }
}

char
try_map_and_load(char* name, char* path, pe_loaded_image* loaded_image, int required_machine_type) {
  char success = 0;
  size_t sz;
  stralloc sa;
  pe_dos_header* dhdr;

  stralloc_init(&sa);
  if(path)
    stralloc_copys(&sa, path);
  stralloc_cats(&sa, name);
  stralloc_nul(&sa);

  if(!path_exists(sa.s))
    return success;

#ifdef DEBUG_OPEN
  buffer_puts(buffer_2, "Filename: ");
  buffer_puts(buffer_2, sa.s);
  buffer_putnlflush(buffer_2);
#endif

  dhdr = (pe_dos_header*)mmap_read(sa.s, &sz);

  loaded_image->size_of_image = sz;

  if(dhdr) {
    loaded_image->base = (char*)dhdr;
    loaded_image->file_header = pe_header_nt(loaded_image->base);
    loaded_image->module_name = str_dup(sa.s);
    loaded_image->sections = pe_header_sections(loaded_image->base, &loaded_image->number_of_sections);
    success = 1;
  }

  stralloc_free(&sa);

  return success;
}

int
build_dep_tree(build_tree_config* cfg, char* name, struct dep_tree_element* root, struct dep_tree_element* self) {
  pe_loaded_image loaded_image;
  pe_loaded_image* img;
  pe_dos_header* dos;
  unsigned char* hmod = 0;
  char success = 0;

  uint32 i, j;

  if(self->flags & DEPTREE_PROCESSED) {
    return 0;
  }

  if(cfg->on_self) {
    // if(self->resolved_module == NULL)    self->resolved_module = str_dup(name);

    dos = (pe_dos_header*)hmod;
    loaded_image.file_header = pe_header_nt(hmod);
    loaded_image.sections = pe_header_sections(hmod, &loaded_image.number_of_sections);
    loaded_image.base = (void*)hmod;

    if(cfg->machine_type != -1 && (int)loaded_image.file_header->coff_header.machine != cfg->machine_type)
      return 1;
  } else {
    char* dir;
    success = FALSE;
    strlist_foreach_s(cfg->search_paths, dir) {
      success = try_map_and_load(str_basename(name), dir, &loaded_image, cfg->machine_type);
      if(success)
        break;
    }
    if(!success)
      success = try_map_and_load(name, NULL, &loaded_image, cfg->machine_type);
    if(!success) {
      self->flags |= DEPTREE_UNRESOLVED;
      return 1;
    }
    if(self->resolved_module == NULL)
      self->resolved_module = str_dup(loaded_image.module_name);
  }
  if(cfg->machine_type == -1)
    cfg->machine_type = (int)loaded_image.file_header->coff_header.machine;
  img = &loaded_image;

  push_stack(cfg->stack, cfg->stack_len, cfg->stack_size, name);

  self->mapped_address = loaded_image.base;
  self->flags |= DEPTREE_PROCESSED;

  build_dep_tree32or64(img, cfg, root, self);

  if(!cfg->on_self) {
    mmap_unmap(loaded_image.base, loaded_image.size_of_image);
  }

  /* Not sure if a forwarded export warrants an import. If it doesn't, then the dll to which the export is forwarded
  will NOT
   * be among the dependencies of this dll and it will be necessary to do yet another process_dep...
  for(i = 0; i < self->exports_len; i++)
  {
    if(self->exports[i]->forward_str != NULL && self-.exports[i]->forward == NULL)
    {
      char *forward_str_copy = NULL, *export_name = NULL, *rdot = NULL;
      uint32 export_ordinal = 0;
      forward_str_copy = str_dup (self->exports[i]->forward_str);
      rdot = strrchr (forward_str_copy, '.');
      if(rdot != NULL && rdot[1] != 0)
      {
        rdot[0] = 0;
        export_name = &rdot[1];
        if(export_name[0] == '#' && export_name[1] >= '0' && export_name[1] <= '9')
        {
          export_ordinal = strtol (&export_name[1], NULL, 10);
          export_name = NULL;
        }
        self->exports[i]->forward = find_export_forward (forward_str_copy, export_name, export_ordinal);
      }
      free (forward_str_copy);
    }
  }
  */
  for(i = 0; i < self->imports_len; i++) {
    if(self->imports[i].mapped == NULL && self->imports[i].dll != NULL &&
       (self->imports[i].name != NULL || self->imports[i].ordinal > 0)) {
      struct dep_tree_element* dll = self->imports[i].dll;
      for(j = 0; j < dll->exports_len; j++) {
        if((self->imports[i].name != NULL && dll->exports[j].name != NULL &&
            str_equal(self->imports[i].name, dll->exports[j].name)) ||
           (self->imports[i].ordinal > 0 && dll->exports[j].ordinal > 0 &&
            self->imports[i].ordinal == dll->exports[j].ordinal)) {
          self->imports[i].mapped = &dll->exports[j];
          break;
        }
      }
      /*
            if(self->imports[i].mapped == NULL)
              printf("Could not match %s (%d) in %s to %s\n", self->imports[i].name, self->imports[i].ordinal,
         self->module, dll->module);
      */
    }
  }
  /* By keeping items in the stack we turn it into a list of all
   * processed modules, this should be more effective at preventing
   * us from processing modules multiple times
   */
  /*pop_stack (stack, stack_len, stack_size, name);*/
  return 0;
}

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
  buffer_putm_3(buffer_1, "Usage: ", argv0, " [OPTION]... FILE...\n\
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
  size_t i;
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
      buffer_putxint640(buffer_1, item->address_offset, 8);
      buffer_putm_3(buffer_1, item->forward_str ? " ->" : "", item->forward_str ? item->forward_str : "", " <");
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
      buffer_putm_2(buffer_2, self->module, ": not found\n");
      buffer_flush(buffer_2);
    }
    unresolved = 1;
  }

  if(!unresolved && !first) {
    if(str_case_diff(self->module, self->resolved_module) == 0) {
      buffer_puts(buffer_1, " (");
      buffer_putptr(buffer_1, self->mapped_address);
    } else {
      buffer_putm_3(buffer_1, " => ", self->resolved_module, " (0x");
      buffer_putptr(buffer_1, self->mapped_address);
    }
    buffer_putsflush(buffer_1, ")\n");
  }

  if(list_imports) {
    for(i = 0; i < self->imports_len; i++) {
      struct import_table_item* item = &self->imports[i];

      buffer_puts(buffer_1, "\t");
      buffer_putnspace(buffer_1, depth * 2);
      buffer_putxint640(buffer_1, item->orig_address, 8);
      buffer_putspace(buffer_1);
      buffer_putxint640(buffer_1, item->address, 8);
      buffer_putspace(buffer_1);
      buffer_putulong0(buffer_1, item->ordinal, 3);
      buffer_putspace(buffer_1);
      buffer_puts(buffer_1, item->name ? item->name : "<0>");
      buffer_putspace(buffer_1);
      buffer_puts(buffer_1, item->mapped ? "" : "<UNRESOLVED>");
      buffer_putspace(buffer_1);
      buffer_puts(buffer_1, item->dll == 0 ? "<MODULE MISSING>" : item->dll->module ? item->dll->module : "<0>");
      buffer_putnlflush(buffer_1);
      /*printf("\t%*s%llx %llx %3d %s %s %s\n",
             depth,
             depth > 0 ? " " : "",
             (long long)item->orig_address,
             (long long)item->address,
             item->ordinal,
             item->name ? item->name : "<0>",
             item->mapped ? "" : "<UNRESOLVED>",
             item->dll == 0 ? "<MODULE MISSING>" : item->dll->module ? item->dll->module : "<0>");*/
    }
  }

  if(unresolved)
    return -1;

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

#if WINDOWS
/* registry_query("HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", "Path",
 */
int
registry_query(const char* key, const char* value, stralloc* sa) {
  HKEY hkey;
  DWORD len, ret, type;
  typedef LONG(WINAPI reggetvalue_fn)(HKEY, LPCSTR, LPCSTR, DWORD, DWORD*, void*, DWORD*);
  static reggetvalue_fn* api_fn;

  if(!api_fn) {
    HANDLE advapi;
    if((advapi = LoadLibraryA("advapi32.dll")) != INVALID_HANDLE_VALUE)
      api_fn = (reggetvalue_fn*)GetProcAddress(advapi, "RegGetValueA");
  }

  if(!api_fn)
    return -1;

  if(!str_diffn(key, "HKCU", 4) || !str_diffn(key, "HKEY_CURRENT_USER", 17)) {
    hkey = HKEY_CURRENT_USER;
  } else if(!str_diffn(key, "HKLM", 4) || !str_diffn(key, "HKEY_LOCAL_MACHINE", 19)) {
    hkey = HKEY_LOCAL_MACHINE;
  } else if(!str_diffn(key, "HKCR", 4) || !str_diffn(key, "HKEY_CLASSES_ROOT", 17)) {
    hkey = HKEY_LOCAL_MACHINE;
  } else if(!str_diffn(key, "HKU", 3) || !str_diffn(key, "HKEY_USERS", 10)) {
    hkey = HKEY_LOCAL_MACHINE;
  }

  stralloc_ready(sa, PATH_MAX * 64 + 1);
  len = sa->a;
  ret = (*api_fn)(hkey, strchr(key, '\\') + 1, value, RRF_RT_ANY, &type, sa->s, &len);
  sa->len = len;
  if(ret == ERROR_SUCCESS) {
    if(type == REG_EXPAND_SZ)
      stralloc_expand(sa);
    return sa->len = str_len(sa->s);
  }
  return 0;
}
#endif

void
add_path(strlist* sp, const char* path) {
  char s = ':';
  const char* sep = &path[0];

  if(sep[1] == ':' && (sep[2] == '\\' || sep[2] == '/')) {
    sep += 2;
    s = ';';
  }

  {
    stralloc dir;
    strlist tmp;
    stralloc_init(&dir);
    strlist_init(&tmp, '\0');
    strlist_froms(&tmp, path, s);

    strlist_foreach_s(&tmp, path) {
      pathconv(path, &dir);

      if(dir.s[0] == '/')
        sep = dir.s;

      stralloc_nul(&dir);
      if(!path_is_absolute(dir.s))
        stralloc_prepend(&dir, &cwd);

      if(!stralloc_endb(&dir, sep, 1))
        stralloc_catc(&dir, *sep);

      strlist_push_unique_sa(sp, &dir);
    }
    strlist_free(&tmp);
    stralloc_free(&dir);
  }
}

int
main(int argc, char** argv) {
  int i;
  static int verbose = 0, unused = 0, datarelocs = 0, functionrelocs = 0, recursive = 0, list_exports = 0,
             list_imports = 0;
  int skip = 0;
  int files = 0;
  int files_start = -1;
  int files_count = 0;
  int c;
  int digit_optind = 0;
  const char* rel_to = 0;
  int index = 0;
  const struct longopt opts[] = {{"help", 0, 0, 'h'},
                                 {"verbose", 0, &verbose, 'v'},
                                 {"unused", 0, &unused, 'u'},
                                 {"data-relocs", 0, &datarelocs, 'd'},
                                 {"function-relocs", 0, &functionrelocs, 'r'},
                                 {"recursive", 0, &recursive, 'R'},
                                 {"list-exports", 0, &list_exports, 'e'},
                                 {"list-imports", 0, &list_imports, 'i'},
                                 {"version", 0, 0, 'V'},
                                 {"search-dir", 0, 0, 'D'},
                                 {0}};

  strlist sp;
  strlist_init(&sp, '\0');

  path_getcwd(&cwd);
  stralloc_append(&cwd, PATHSEP_S);

  // byte_zero(&sp, sizeof(sp));
  // sp.path = calloc(1, sizeof(char*));
  for(;;) {
    c = getopt_long(argc, argv, "hvudrRei", opts, &index);
    if(c == -1)
      break;
    if(c == 0)
      continue;

    switch(c) {
      case 'h':
        printhelp(argv[0]);
        skip = 1;
        break;

      case 'v':
      case 'u':
      case 'd':
      case 'r':
      case 'R':
      case 'e':
      case 'i': break;
      case 'D': {
        add_path(&sp, optarg);
        break;
      }

      case 'V': printversion(); break;
      default:
        buffer_putm_4(buffer_2, "Unrecognized option `", argv[i], "'\n", "Try `ntldd --help' for more information");
        buffer_putnlflush(buffer_2);
        return 1;
    }
  }

  skip = 0;
  files_start = optind;
  {
    const char* pathenv = getenv("PATH");

    buffer_putm_2(buffer_2, "PATH=", pathenv);
    buffer_putnlflush(buffer_2);

    if(pathenv)
      add_path(&sp, pathenv);
  }

#if WINDOWS
  {
    const char* const keys[] = {"HKCU\\Environment",
                                "HKLM\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment",
                                0};
    int kidx;
    stralloc rpath;
    stralloc_init(&rpath);

    for(kidx = 0; keys[kidx]; ++kidx) {
      if(registry_query(keys[kidx], "Path", &rpath)) {
        buffer_putm_3(buffer_2, "Registry path [", keys[kidx], "]: ");
        buffer_putsa(buffer_2, &rpath);
        buffer_putnlflush(buffer_2);

        stralloc_nul(&rpath);
        add_path(&sp, rpath.s);
      }
    }
  }
#endif

  if(!skip && files_start > 0) {
    files_count = argc - files_start;
    for(i = 0; i < files_count; ++i) {
      char* p;
      char buff[MAX_PATH];
      str_copyn(buff, argv[files_start + i], sizeof(buff));
      p = str_basename(buff);
      if(p)
        *p = '\0';
      strlist_push_unique(&sp, buff);
    }
    buffer_puts(buffer_2, "pathlist=");
    strlist_dump(buffer_2, &sp);
    buffer_putnlflush(buffer_2);

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
          char** stack = 0;
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
