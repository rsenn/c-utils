/*
    libntldd - builds a dependency tree of a module, with symbols

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

#ifdef _WIN32
#include <windows.h>
#endif

//#include <imagehlp.h>

//#include <winnt.h>

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <libgen.h>
#include <errno.h>
#include <limits.h>

#include "libntldd.h"
#include "uint64.h"
#include "mmap.h"
#include "str.h"
#include "lib/pe.h"

#define FALSE 0
#define TRUE 1

#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif

typedef struct _soff_entry soff_entry;

struct _soff_entry {
  uint32 start;
  uint32 end;
  void *off;
};

void *map_pointer(soff_entry *soffs, int soffs_len, uint32 in_ptr, int *section) {
  int i;
  for(i = 0; i < soffs_len; i++)
    if(soffs[i].start <= in_ptr && soffs[i].end >= in_ptr) {
      if(section != NULL)
        *section = i;
      if(soffs[i].off)
        return soffs[i].off + in_ptr;
    }
  return NULL;
}

/*
int find_section_id (pe_optional_header *oh, uint32 address, uint32 size)
{
  int i;
  for(i = 0; i < oh->number_of_rva_and_sizes; i++)
  {
    if(oh->data_directory[i].virtual_address == address &&
        oh->data_directory[i].size == size)
      return i;
  }
  return -1;
}
*/

int
find_section_by_raw_data(pe_loaded_image *img, uint32 address) {
  unsigned long i;
  for(i = 0; i < img->number_of_sections; i++) {
    uint32 start = img->sections[i].virtual_address;
    uint32 end = start + img->sections[i].size_of_raw_data;
    if(address >= start && address < end)
      return i;
  }
  return -1;
}

void
resize_array(void **data, uint64_t *data_size, size_t sizeof_data) {
  uint64_t new_size = (*data_size) > 0 ? (*data_size) * 2 : 64;
  void *new_data;
  new_data = realloc(*data, new_size * sizeof_data);
  memset(((unsigned char *) new_data) + (*data_size * sizeof_data), 0, (new_size - (*data_size)) * sizeof_data);
  *data = new_data;
  *data_size = new_size;
}

#define resize_dep_list(ptr_deptree, ptr_deptree_size) resize_array ((void **) ptr_deptree, ptr_deptree_size, sizeof(struct dep_tree_element *))
#define resize_import_list(ptr_import_list, ptr_import_list_size) resize_array ((void **) ptr_import_list, ptr_import_list_size, sizeof(struct import_table_item))
#define resize_stack(ptr_stack, ptr_stack_size) resize_array ((void **) ptr_stack, ptr_stack_size, sizeof(char *))

void
add_dep(struct dep_tree_element *parent, struct dep_tree_element *child) {
  if(parent->childs_len >= parent->childs_size) {
    resize_dep_list(&parent->childs, &parent->childs_size);
  }
  parent->childs[parent->childs_len] = child;
  parent->childs_len += 1;
}

struct import_table_item *add_import(struct dep_tree_element *self) {
  if(self->imports_len >= self->imports_size) {
    resize_import_list(&self->imports, &self->imports_size);
  }
  self->imports_len += 1;
  return &self->imports[self->imports_len - 1];
}

int
find_dep(struct dep_tree_element *root, char *name, struct dep_tree_element **result) {
  int ret = -1;
  uint64_t i;
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
    ret =
      find_dep(root->childs[i], name, result);
  }
  root->flags &= ~DEPTREE_VISITED;
  return ret;
}

int
build_dep_tree(build_tree_config* cfg, char *name, struct dep_tree_element *root, struct dep_tree_element *self);

struct dep_tree_element *process_dep(build_tree_config* cfg, soff_entry *soffs, int soffs_len, uint32 name, struct dep_tree_element *root, struct dep_tree_element *self, int deep) {
  struct dep_tree_element *child = NULL;
  int found;
  int64_t i;
  char *dllname = (char *)map_pointer(soffs, soffs_len, name, NULL);
  if(dllname == NULL)
    return NULL;
  if(strlen(dllname) > 10 && str_case_diffn("api-ms-win", dllname, 10) == 0) {
    /* TODO: find a better way to identify api stubs. Versioninfo, maybe? */
    return NULL;
  }
  for(i = (int64_t) * cfg->stack_len - 1; i >= 0; i--) {
    if((*cfg->stack)[i] && str_case_diff((*cfg->stack)[i], dllname) == 0)
      return NULL;
    if(i == 0)
      break;
  }
  found =
    find_dep(root, dllname, &child);
  if(found < 0) {
    child = (struct dep_tree_element *) malloc(sizeof(struct dep_tree_element));
    memset(child, 0, sizeof(struct dep_tree_element));
    if(deep == 0) {
      child->module = strdup(dllname);

      add_dep(self, child);
    }
  }
  if(deep == 1) {

    build_dep_tree(cfg, dllname, root, child);
  }
  return child;
}


/*
struct export_table_item *find_export_forward (struct dep_tree_element *self, char *dllname, char *export_name, uint32 export_ordinal)
{
  return NULL;
}
*/

int
clear_dep_status(struct dep_tree_element *self, uint64_t flags) {
  uint64_t i;
  for(i = 0; i < self->childs_len; i++)

    clear_dep_status(self->childs[i], flags);
  self->flags &= ~flags;
  return 0;
}

void
push_stack(char ***stack, uint64_t *stack_len, uint64_t *stack_size, char *name) {
  if(*stack_len >= *stack_size) {
    resize_stack(stack, stack_size);
  }
  (*stack)[*stack_len] = strdup(name);
  (*stack_len) += 1;
}

void
pop_stack(char ***stack, uint64_t *stack_len, uint64_t *stack_size, char *name) {
  (*stack)[*stack_len] = NULL;
  (*stack_len) -= 1;
}

static uint64_t thunk_data_u1_function(void *thunk_array, uint32 index, build_tree_config *cfg) {
  if(cfg->machine_type == PE_FILE_MACHINE_I386)
    return ((pe_thunk_data32 *) thunk_array)[index].u1.function;
  else
    return ((pe_thunk_data64 *) thunk_array)[index].u1.function;
}

static void *opt_header_get_dd_entry(void *opt_header, uint32 entry_type, build_tree_config *cfg) {
  if(cfg->machine_type == PE_FILE_MACHINE_I386)
    return &(((pe32_opt_header*) opt_header)->data_directory[entry_type]);
  else
    return &(((pe64_opt_header*) opt_header)->data_directory[entry_type]);
}

static void build_dep_tree32or64(pe_loaded_image *img, build_tree_config* cfg, struct dep_tree_element *root, struct dep_tree_element *self, soff_entry *soffs, int soffs_len) {
  pe_data_directory *idata;
  pe_import_descriptor *iid;
  pe_export_directory *ied;
  pe_delayload_descriptor *idd;
  void *ith, *oith;
  void *opt_header = &img->file_header->optional_header;
  uint32 i, j;

  idata = opt_header_get_dd_entry(opt_header, PE_DIRECTORY_ENTRY_EXPORT, cfg);
  if(idata->size > 0 && idata->virtual_address != 0) {
    int export_section = -2;
    ied = (pe_export_directory *)map_pointer(soffs, soffs_len, idata->virtual_address, &export_section);
    if(ied && ied->name != 0) {
      char *export_module = map_pointer(soffs, soffs_len, ied->name, NULL);
      if(export_module != NULL) {
        if(self->export_module == NULL)
          self->export_module = strdup(export_module);
      }
    }
    if(ied && ied->number_of_functions > 0) {
      uint32 *addrs, *names;
      uint16 *ords;
      int section = -1;
      self->exports_len = ied->number_of_functions;
      self->exports = (struct export_table_item *) malloc(sizeof(struct export_table_item) * self->exports_len);
      memset(self->exports, 0, sizeof(struct export_table_item) * self->exports_len);
      addrs = (uint32 *)map_pointer(soffs, soffs_len, ied->address_of_functions, NULL);
      ords = (uint16 *)map_pointer(soffs, soffs_len, ied->address_of_name_ordinals, NULL);
      names = (uint32 *)map_pointer(soffs, soffs_len, ied->address_of_names, NULL);
      for(i = 0; i < ied->number_of_names; i++) {
        self->exports[ords[i]].ordinal = ords[i] + ied->base;
        if(names[i] != 0) {
          char *s_name = (char *)map_pointer(soffs, soffs_len, names[i], NULL);
          if(s_name != NULL)
            self->exports[ords[i]].name = strdup(s_name);
        }
      }
      for(i = 0; i < ied->number_of_functions; i++) {
        if(addrs[i] != 0) {
          int section_index =
            find_section_by_raw_data(img, addrs[i]);
          if((idata->virtual_address <= addrs[i]) && (idata->virtual_address + idata->size > addrs[i])) {
            self->exports[i].address = NULL;
            self->exports[i].forward_str = strdup((char *)map_pointer(soffs, soffs_len, addrs[i], NULL));
          } else
            self->exports[i].address = map_pointer(soffs, soffs_len, addrs[i], &section);
          self->exports[i].ordinal = i + ied->base;
          self->exports[i].section_index = section_index;
          self->exports[i].address_offset = addrs[i];
        }
      }
    }
  }

  idata = opt_header_get_dd_entry(opt_header, PE_DIRECTORY_ENTRY_IMPORT, cfg);
  if(idata->size > 0 && idata->virtual_address != 0) {
    iid = (pe_import_descriptor *)map_pointer(soffs, soffs_len,
          idata->virtual_address, NULL);
    if(iid)
      for(i = 0; iid[i].characteristics || iid[i].time_date_stamp ||
          iid[i].forwarder_chain || iid[i].name || iid[i].first_thunk; i++) {
        struct dep_tree_element *dll;
        uint64_t impaddress;
        dll =
          process_dep(cfg, soffs, soffs_len, iid[i].name, root, self, 0);
        if(dll == NULL)
          continue;
        ith = (void *)map_pointer(soffs, soffs_len, iid[i].first_thunk, NULL);
        oith = (void *)map_pointer(soffs, soffs_len, iid[i].original_first_thunk, NULL);
        for(j = 0; (impaddress = thunk_data_u1_function(ith, j, cfg)) != 0; j++) {
          struct import_table_item *imp =
            add_import(self);
          imp->dll = dll;
          imp->ordinal = -1;
          if(oith);
          imp->orig_address = thunk_data_u1_function(oith, j, cfg);
          if(cfg->on_self) {
            imp->address = impaddress;
          }
          if(oith && imp->orig_address & (1 << (sizeof(uint32) * 8 - 1))) {
            imp->ordinal = imp->orig_address & ~(1 << (sizeof(uint32) * 8 - 1));
          } else if(oith) {
            pe_import_by_name *byname = (pe_import_by_name *)map_pointer(soffs, soffs_len, imp->orig_address, NULL);
            if(byname != NULL)
              imp->name = strdup((char *) byname->name);
          }
        }
      }
  }

  idata = opt_header_get_dd_entry(opt_header, PE_DIRECTORY_ENTRY_DELAY_IMPORT, cfg);
  if(idata->size > 0 && idata->virtual_address != 0) {
    idd = (pe_delayload_descriptor *)map_pointer(soffs, soffs_len, idata->virtual_address, NULL);
    if(idd)
      for(i = 0; idd[i].attributes.all_attributes || idd[i].dll_name_rva ||
          idd[i].module_handle_rva || idd[i].import_address_table_rva || idd[i].import_name_table_rva ||
          idd[i].bound_import_address_table_rva || idd[i].unload_information_table_rva ||
          idd[i].time_date_stamp; i++) {
        struct dep_tree_element *dll;
        uint64_t impaddress;
        dll =
          process_dep(cfg, soffs, soffs_len, idd[i].dll_name_rva, root, self, 0);
        if(dll == NULL)
          continue;
        if(idd[i].attributes.all_attributes & 0x00000001) {
          ith = (void *)map_pointer(soffs, soffs_len, idd[i].import_address_table_rva, NULL);
          oith = (void *)map_pointer(soffs, soffs_len, idd[i].import_name_table_rva, NULL);
        } else {
          ith = (void *)(uintptr_t)idd[i].import_address_table_rva;
          oith = (void *)(uintptr_t)idd[i].import_name_table_rva;
        }
        for(j = 0; (impaddress = thunk_data_u1_function(ith, j, cfg)) != 0; j++) {
          struct import_table_item *imp =
            add_import(self);
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
            pe_import_by_name *byname = (pe_import_by_name *)map_pointer(soffs, soffs_len, imp->orig_address, NULL);
            if(byname != NULL)
              imp->name = strdup((char *) byname->name);
          }
        }
      }
  }

  idata = opt_header_get_dd_entry(opt_header, PE_DIRECTORY_ENTRY_IMPORT, cfg);
  if(idata->size > 0 && idata->virtual_address != 0) {
    iid = (pe_import_descriptor *)map_pointer(soffs, soffs_len,
          idata->virtual_address, NULL);
    if(iid)
      for(i = 0; iid[i].characteristics || iid[i].time_date_stamp ||
          iid[i].forwarder_chain || iid[i].name || iid[i].first_thunk; i++)

        process_dep(cfg, soffs, soffs_len, iid[i].name, root, self, 1);
  }

  idata = opt_header_get_dd_entry(opt_header, PE_DIRECTORY_ENTRY_DELAY_IMPORT, cfg);
  if(idata->size > 0 && idata->virtual_address != 0) {
    idd = (pe_delayload_descriptor *)map_pointer(soffs, soffs_len, idata->virtual_address, NULL);
    if(idd)
      for(i = 0; idd[i].attributes.all_attributes || idd[i].dll_name_rva ||
          idd[i].module_handle_rva || idd[i].import_address_table_rva || idd[i].import_name_table_rva ||
          idd[i].bound_import_address_table_rva || idd[i].unload_information_table_rva ||
          idd[i].time_date_stamp; i++)

        process_dep(cfg, soffs, soffs_len, idd[i].dll_name_rva, root, self, 1);
  }
}

char
try_map_and_load(char* name, char* path, pe_loaded_image* loaded_image, int required_machine_type) {
  char success = 0;
  size_t sz;
  pe_dos_header* dhdr = (pe_dos_header*)mmap_read(name, &sz);

  loaded_image->size_of_image = sz;

  if(dhdr) {
    loaded_image->mapped_address = (char*)dhdr;
    loaded_image->file_header = (pe_nt_headers64*)(loaded_image->mapped_address + dhdr->e_lfanew);
    loaded_image->number_of_sections = loaded_image->file_header->file_header.number_of_sections;
    loaded_image->module_name = basename(name);
    loaded_image->sections = (section_header*) &((pe_nt_headers64*)loaded_image->file_header)[1];
    success = 1;
  }





  /*MapAndLoad(name, path, loaded_image, FALSE, TRUE);
  if(!success && errno == ENOENT)
    success = MapAndLoad(name, path, loaded_image, TRUE, TRUE);
  if(success && required_machine_type != -1 && (int)loaded_image->file_header->file_header.machine != required_machine_type) {
    UnMapAndLoad(loaded_image);
    return FALSE;
  }*/

  return success;
}

int
build_dep_tree(build_tree_config* cfg, char *name, struct dep_tree_element *root, struct dep_tree_element *self) {
  pe_loaded_image loaded_image;
  pe_loaded_image *img;
  pe_dos_header *dos;
  unsigned char* hmod;
  char success = 0;

  uint32 i, j;
  int soffs_len;
  soff_entry *soffs;

  if(self->flags & DEPTREE_PROCESSED) {
    return 0;
  }

  if(cfg->on_self) {
    char modpath[MAX_PATH];
    /*success = GetModuleHandleExA(0x2, name, &hmod);*/
    if(!success)
      return 1;
    /*if(GetModuleFileNameA(hmod, modpath, MAX_PATH) == 0)
      return 1;*/
    if(self->resolved_module == NULL)
      self->resolved_module = strdup(modpath);

    dos = (pe_dos_header *) hmod;
    loaded_image.file_header = (pe_nt_headers64 *)((char *) hmod + dos->e_lfanew);
    loaded_image.sections = (section_header *)((char *) hmod + dos->e_lfanew + sizeof(pe_nt_headers64));
    loaded_image.number_of_sections = loaded_image.file_header->file_header.number_of_sections;
    loaded_image.mapped_address = (void *) hmod;
    if(cfg->machine_type != -1 && (int)loaded_image.file_header->file_header.machine != cfg->machine_type)
      return 1;
  } else {
    success = FALSE;
    for(i = 0; i < cfg->search_paths->count && !success; ++i) {
      success =
        try_map_and_load(name, cfg->search_paths->path[i], &loaded_image, cfg->machine_type);
    }
    if(!success)
      success =
        try_map_and_load(name, NULL, &loaded_image, cfg->machine_type);
    if(!success) {
      self->flags |= DEPTREE_UNRESOLVED;
      return 1;
    }
    if(self->resolved_module == NULL)
      self->resolved_module = strdup(loaded_image.module_name);
  }
  if(cfg->machine_type == -1)
    cfg->machine_type = (int)loaded_image.file_header->file_header.machine;
  img = &loaded_image;


  push_stack(cfg->stack, cfg->stack_len, cfg->stack_size, name);

  self->mapped_address = loaded_image.mapped_address;

  self->flags |= DEPTREE_PROCESSED;

  soffs_len = img->number_of_sections;
  soffs = (soff_entry *) malloc(sizeof(soff_entry) * (soffs_len + 1));
  for(i = 0; i < img->number_of_sections; i++) {
    soffs[i].start = img->sections[i].virtual_address;
    soffs[i].end = soffs[i].start + img->sections[i].misc.virtual_size;
    if(cfg->on_self)
      soffs[i].off = img->mapped_address/* + img->sections[i].virtual_address*/;
    else if(img->sections[i].pointer_to_raw_data != 0)
      soffs[i].off = img->mapped_address + img->sections[i].pointer_to_raw_data -
                     img->sections[i].virtual_address;
    else
      soffs[i].off = NULL;
  }
  soffs[img->number_of_sections].start = 0;
  soffs[img->number_of_sections].end = 0;
  soffs[img->number_of_sections].off = 0;

  build_dep_tree32or64(img, cfg, root, self, soffs, soffs_len);
  free(soffs);

  if(!cfg->on_self) {
    mmap_unmap(loaded_image.mapped_address, loaded_image.size_of_image);
  }

  /* Not sure if a forwarded export warrants an import. If it doesn't, then the dll to which the export is forwarded will NOT
   * be among the dependencies of this dll and it will be necessary to do yet another process_dep...
  for(i = 0; i < self->exports_len; i++)
  {
    if(self->exports[i]->forward_str != NULL && self-.exports[i]->forward == NULL)
    {
      char *forward_str_copy = NULL, *export_name = NULL, *rdot = NULL;
      uint32 export_ordinal = 0;
      forward_str_copy = strdup (self->exports[i]->forward_str);
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
    if(self->imports[i].mapped == NULL && self->imports[i].dll != NULL && (self->imports[i].name != NULL || self->imports[i].ordinal > 0)) {
      struct dep_tree_element *dll = self->imports[i].dll;
      for(j = 0; j < dll->exports_len; j++) {
        if((self->imports[i].name != NULL && dll->exports[j].name != NULL && strcmp(self->imports[i].name, dll->exports[j].name) == 0) ||
            (self->imports[i].ordinal > 0 && dll->exports[j].ordinal > 0 && self->imports[i].ordinal == dll->exports[j].ordinal)) {
          self->imports[i].mapped = &dll->exports[j];
          break;
        }
      }
      /*
            if(self->imports[i].mapped == NULL)
              printf("Could not match %s (%d) in %s to %s\n", self->imports[i].name, self->imports[i].ordinal, self->module, dll->module);
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
