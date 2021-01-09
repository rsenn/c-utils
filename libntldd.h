#ifndef __LIBNTLDD_H__
#define __LIBNTLDD_H__

#include "lib/pe.h"
#include "lib/strlist.h"
#include "lib/uint16.h"
#include "lib/uint32.h"
#include "lib/uint64.h"

#ifdef __unix__
#define stricmp strcasecmp
#define strnicmp strncasecmp
#endif

#define NTLDD_VERSION_MAJOR 0
#define NTLDD_VERSION_MINOR 2

struct dep_tree_element;

struct export_table_item {
  void* address;
  char* name;
  uint16 ordinal;
  char* forward_str;
  struct export_table_item* forward;
  int section_index;
  uint32 address_offset;
};

struct import_table_item {
  uint64 orig_address;
  uint64 address;
  char* name;
  int ordinal;
  struct dep_tree_element* dll;
  struct export_table_item* mapped;
};

struct dep_tree_element {
  uint64 flags;
  char* module;
  char* export_module;
  char* resolved_module;
  void* mapped_address;
  struct dep_tree_element** childs;
  uint64 childs_size;
  uint64 childs_len;
  uint64 imports_len;
  uint64 imports_size;
  struct import_table_item* imports;
  uint64 exports_len;
  struct export_table_item* exports;
};

#define DEPTREE_VISITED 0x00000001
#define DEPTREE_UNRESOLVED 0x00000002
#define DEPTREE_PROCESSED 0x00000004

int clear_dep_status(struct dep_tree_element* self, uint64 flags);

void add_dep(struct dep_tree_element* parent, struct dep_tree_element* child);

typedef struct build_tree_config_t {
  int datarelocs;
  int functionrelocs;
  int recursive;
  int on_self;
  char*** stack;
  uint64* stack_len;
  uint64* stack_size;
  int machine_type;
  strlist* search_paths;
} build_tree_config;

int build_dep_tree(build_tree_config* cfg,
                   char* name,
                   struct dep_tree_element* root,
                   struct dep_tree_element* self);

#define FALSE 0
#define TRUE 1
typedef size_t uintptr_t;

#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif

typedef struct _soff_entry soff_entry;

struct _soff_entry {
  uint32 start;
  uint32 end;
  void* off;
};

#endif