#ifndef PE_H
#define PE_H

#include "typedefs.h"
#include "uint64.h"
#include "uint32.h"
#include "uint16.h"
#include "uint8.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint32 virtual_address;
  uint32 size;
} pe_data_directory;

typedef struct __unaligned {
  uint32 characteristics;     /**< 0x00 */
  uint32 time_date_stamp;     /**< 0x04 */
  uint16 major_version;       /**< 0x08 */
  uint16 minor_version;       /**< 0x0a */
  uint32 name;                /**< 0x0c */
  uint32 base;                /**< 0x10 */
  uint32 number_of_functions; /**< 0x14 */
  uint32 number_of_names;     /**< 0x18 */
  uint32 address_of_functions;
  uint32 address_of_names;
  uint32 address_of_name_ordinals;
}
pe_export_directory;

typedef struct __unaligned {
  uint32 characteristics;
  uint32 time_date_stamp;
  uint16 major_version;
  uint16 minor_version;
  uint16 number_of_named_entries;
}
pe_resource_directory;

typedef struct __unaligned {
  union {
    struct {
      uint32 name_offset : 31;
      uint32 name_is_string : 1;
    };
    uint32 name;
    uint16 id;
  };
  union {
    uint32 offset_to_data;
    struct {
      uint32 offset_to_directory : 31;
      uint32 data_is_directory : 1;
    };
  };
}
pe_resource_directory_entry;

typedef struct __unaligned {
  uint16 length;
  char name_string[1];
}
pe_resource_directory_string;

typedef struct __unaligned {
  uint16 length;
  uint16 name_string[1];
}
pe_resource_dir_string_u;

typedef struct __unaligned {
  uint32 offset_to_data;
  uint32 size;
  uint32 code_page;
  uint32 reserved;
}
pe_resource_data_entry;

typedef struct __unaligned {
  uint32 size;
  uint32 time_date_stamp;
  uint16 major_version;
  uint16 minor_version;
  uint32 global_flags_clear;
  uint32 global_flags_set;
  uint32 critical_section_default_timeout;
  uint32 de_commit_free_block_threshold;
  uint32 de_commit_total_free_threshold;
  uint32 lock_prefix_table;
  uint32 maximum_allocation_size;
  uint32 virtual_memory_threshold;
  uint32 process_heap_flags;
  uint32 process_affinity_mask;
  uint16 csd_version;
  uint16 reserved1;
  uint32 edit_list;
  uint32 security_cookie;
  uint32 se_handler_table;
  uint32 se_handler_count;
}
pe32_load_config_directory;

typedef struct __unaligned {
  uint32 size;
  uint32 time_date_stamp;
  uint16 major_version;
  uint16 minor_version;
  uint32 global_flags_clear;
  uint32 global_flags_set;
  uint32 critical_section_default_timeout;
  uint64 de_commit_free_block_threshold;
  uint64 de_commit_total_free_threshold;
  uint64 lock_prefix_table;
  uint64 maximum_allocation_size;
  uint64 virtual_memory_threshold;
  uint64 process_affinity_mask;
  uint32 process_heap_flags;
  uint16 csd_version;
  uint16 reserved1;
  uint64 edit_list;
  uint64 security_cookie;
  uint64 se_handler_table;
  uint64 se_handler_count;
}
pe64_load_config_directory;

typedef struct __unaligned {
  uint32 characteristics;
  uint32 time_date_stamp;
  uint16 major_version;
  uint16 minor_version;
  uint16 number_of_named_entries;
  uint16 number_of_id_entries;
}
image_resource_directory;

typedef struct __unaligned {
  uint32 characteristics;
  uint32 time_date_stamp;
  uint16 major_version;
  uint16 minor_version;
  uint32 type;
  uint32 size_of_data;
  uint32 address_of_raw_data;
  uint32 pointer_to_raw_data;
}
pe_debug_directory;

typedef struct __unaligned {
  uint64 start_address_of_raw_data;
  uint64 end_address_of_raw_data;
  uint64 address_of_index;
  uint64 address_of_call_backs;
  uint32 size_of_zero_fill;
  uint32 characteristics;
}
pe64_tls_directory;

typedef struct __unaligned {
  uint32 start_address_of_raw_data;
  uint32 end_address_of_raw_data;
  uint32 address_of_index;
  uint32 address_of_call_backs;
  uint32 size_of_zero_fill;
  uint32 characteristics;
}
pe32_tls_directory;

typedef struct {
  union {
    uint32 characteristics;
    uint32 original_first_thunk;
  };
  uint32 time_date_stamp;
  uint32 forwarder_chain;
  uint32 name;
  uint32 first_thunk;
} pe_import_descriptor;

typedef struct {
  uint16 hint;
  unsigned char name[1];
} pe_import_by_name;

typedef struct {
  union {
    uint32 all_attributes;
    struct {
      int rva_based : 1;
      int reserved_attributes : 31;
    };
  } attributes;
  uint32 dll_name_rva;
  uint32 module_handle_rva;
  uint32 import_address_table_rva;
  uint32 import_name_table_rva;
  uint32 bound_import_address_table_rva;
  uint32 unload_information_table_rva;
  uint32 time_date_stamp;
} pe_delayload_descriptor;

typedef struct __unaligned {
  uint16 e_magic;
  uint16 e_cblp;
  uint16 e_cp;
  uint16 e_crlc;
  uint16 e_cparhdr;
  uint16 e_minalloc;
  uint16 e_maxalloc;
  uint16 e_ss;
  uint16 e_sp;
  uint16 e_csum;
  uint16 e_ip;
  uint16 e_cs;
  uint16 e_lfarlc;
  uint16 e_ovno;
  uint16 e_res[4];
  uint16 e_oemid;
  uint16 e_oeminfo;
  uint16 e_res2[10];
  uint32 e_lfanew; /* sizeof(image_dos_h_eader) + size of MS-DOS stub */
}
pe_dos_header;

typedef struct {
  uint16 machine; /* machine_type */
  uint16 number_of_sections;
  uint32 time_date_stamp;
  uint32 pointer_to_symbol_table;
  uint32 number_of_symbols;
  uint16 size_of_optional_header;
  uint16 characteristics; /* image_characteristics */
} pe_coff_header;

#define PE_SECTION_NAME_SIZE 8

/* Quoting pecoff_v8.docx: "Entries in the section table are numbered starting from one (1)". */
typedef struct __unaligned {
  char name[PE_SECTION_NAME_SIZE]; /* TODO: Should we use char instead? */
  union {
    uint32 physical_address; /* same value as next field */
    uint32 virtual_size;
  };
  uint32 virtual_address;
  uint32 size_of_raw_data;
  uint32 pointer_to_raw_data;
  uint32 pointer_to_relocations; /* always zero in executables */
  uint32 pointer_to_linenumbers; /* deprecated */
  uint16 number_of_relocations;
  uint16 number_of_linenumbers; /* deprecated */
  uint32 characteristics;       /* section_characteristics */
}
pe_section_header;

#define PE_SUBSYSTEM_UNKNOWN 0
#define PE_SUBSYSTEM_NATIVE 1
#define PE_SUBSYSTEM_WINDOWS_GUI 2
#define PE_SUBSYSTEM_WINDOWS_CUI 3
#define PE_SUBSYSTEM_OS2_CUI 5
#define PE_SUBSYSTEM_POSIX_CUI 7
#define PE_SUBSYSTEM_NATIVE_WINDOWS 8
#define PE_SUBSYSTEM_WINDOWS_CE_GUI 9
#define PE_SUBSYSTEM_EFI_APPLICATION 10
#define PE_SUBSYSTEM_EFI_BOOT_SERVICE_DRIVER 11
#define PE_SUBSYSTEM_EFI_RUNTIME_DRIVER 12
#define PE_SUBSYSTEM_EFI_ROM 13
#define PE_SUBSYSTEM_XBOX 14
#define PE_SUBSYSTEM_WINDOWS_BOOT_APPLICATION 16

#define PE_DIRECTORY_ENTRY_EXPORT 0
#define PE_DIRECTORY_ENTRY_IMPORT 1
#define PE_DIRECTORY_ENTRY_RESOURCE 2
#define PE_DIRECTORY_ENTRY_EXCEPTION 3
#define PE_DIRECTORY_ENTRY_SECURITY 4
#define PE_DIRECTORY_ENTRY_BASERELOC 5
#define PE_DIRECTORY_ENTRY_DEBUG 6
#define PE_DIRECTORY_ENTRY_ARCHITECTURE 7
#define PE_DIRECTORY_ENTRY_GLOBALPTR 8
#define PE_DIRECTORY_ENTRY_TLS 9
#define PE_DIRECTORY_ENTRY_LOAD_CONFIG 10
#define PE_DIRECTORY_ENTRY_BOUND_IMPORT 11
#define PE_DIRECTORY_ENTRY_IAT 12
#define PE_DIRECTORY_ENTRY_DELAY_IMPORT 13
#define PE_DIRECTORY_ENTRY_COMHEADER 14

#define PE_NUMBEROF_DIRECTORY_ENTRIES 16

/* r_eference: http://msdn.microsoft.com/en-us/library/windows/desktop/ms680339(v=vs.85).aspx */
typedef struct __unaligned {
  uint16 magic;
  unsigned char major_linker_version;
  unsigned char minor_linker_version;
  uint32 size_of_code;
  uint32 size_of_initialized_data;
  uint32 size_of_uninitialized_data;
  uint32 address_of_entry_point;
  uint32 base_of_code;
  uint32 base_of_data; /* only in PE32 */
  uint32 image_base;
  uint32 section_alignment;
  uint32 file_alignment;
  uint16 major_operating_system_version;
  uint16 minor_operating_system_version;
  uint16 major_image_version;
  uint16 minor_image_version;
  uint16 major_subsystem_version;
  uint16 minor_subsystem_version;
  uint32 reserved1;
  uint32 size_of_image;
  uint32 size_of_headers;
  uint32 checksum;
  uint16 subsystem; /* windows_subsystem */
  uint16 dll_characteristics;
  uint32 size_of_stack_reserve;
  uint32 size_of_stack_commit;
  uint32 size_of_heap_reserve;
  uint32 size_of_heap_commit;
  uint32 loader_flags;
  uint32 number_of_rva_and_sizes;
  pe_data_directory data_directory[PE_NUMBEROF_DIRECTORY_ENTRIES];
}
pe32_opt_header;

typedef struct __unaligned {
  uint16 magic;
  unsigned char major_linker_version;
  unsigned char minor_linker_version;
  uint32 size_of_code;
  uint32 size_of_initialized_data;
  uint32 size_of_uninitialized_data;
  uint32 address_of_entry_point;
  uint32 base_of_code;
  uint64 image_base;
  uint32 section_alignment;
  uint32 file_alignment;
  uint16 major_operating_system_version;
  uint16 minor_operating_system_version;
  uint16 major_image_version;
  uint16 minor_image_version;
  uint16 major_subsystem_version;
  uint16 minor_subsystem_version;
  uint32 reserved1;
  uint32 size_of_image;
  uint32 size_of_headers;
  uint32 checksum;
  uint16 subsystem; /* windows_subsystem */
  uint16 dll_characteristics;
  uint64 size_of_stack_reserve;
  uint64 size_of_stack_commit;
  uint64 size_of_heap_reserve;
  uint64 size_of_heap_commit;
  uint32 loader_flags; /* must be zero */
  uint32 number_of_rva_and_sizes;
  pe_data_directory data_directory[PE_NUMBEROF_DIRECTORY_ENTRIES];
}
pe64_opt_header;

typedef struct __unaligned {
  pe_dos_header dos_hdr;
  uint32 signature;
  pe_coff_header* coff_hdr;
  void* optional_hdr_ptr;
  uint32 num_directories;
  void* directories_ptr;
  uint16 num_sections;
  void* sections_ptr;
  uint64 entrypoint;
  uint64 imagebase;
}
pe_file_t;

typedef enum {
  PE_MAGIC_ROM = 0x107,
  PE_MAGIC_PE32 = 0x10b,
  PE_MAGIC_PE64 = 0x20b /* PE32+ */
} pe_type;

typedef enum {
  PE_OPTHDR_MAGIC = 0,
  PE_OPTHDR_SIZE_OF_CODE = 1,
  PE_OPTHDR_SIZE_OF_INITIALIZED_DATA = 2,
  PE_OPTHDR_SIZE_OF_UNINITIALIZED_DATA = 3,
  PE_OPTHDR_ADDRESS_OF_ENTRY_POINT = 4,
  PE_OPTHDR_BASE_OF_CODE = 5,
  PE_OPTHDR_IMAGE_BASE = 6,
  PE_OPTHDR_SECTION_ALIGNMENT = 7,
  PE_OPTHDR_FILE_ALIGNMENT = 8,
  PE_OPTHDR_MAJOR_OPERATING_SYSTEM_VERSION = 9,
  PE_OPTHDR_MINOR_OPERATING_SYSTEM_VERSION = 10,
  PE_OPTHDR_MAJOR_IMAGE_VERSION = 11,
  PE_OPTHDR_MINOR_IMAGE_VERSION = 12,
  PE_OPTHDR_MAJOR_SUBSYSTEM_VERSION = 13,
  PE_OPTHDR_MINOR_SUBSYSTEM_VERSION = 14,
  PE_OPTHDR_RESERVED1 = 15,
  PE_OPTHDR_SIZE_OF_IMAGE = 16,
  PE_OPTHDR_SIZE_OF_HEADERS = 17,
  PE_OPTHDR_CHECKSUM = 18,
  PE_OPTHDR_DLL_CHARACTERISTICS = 19,
  PE_OPTHDR_SIZE_OF_STACK_RESERVE = 20,
  PE_OPTHDR_SIZE_OF_STACK_COMMIT = 21,
  PE_OPTHDR_SIZE_OF_HEAP_RESERVE = 22,
  PE_OPTHDR_SIZE_OF_HEAP_COMMIT = 23,
  PE_OPTHDR_LOADER_FLAGS = 24,
  PE_OPTHDR_NUMBER_OF_RVA_AND_SIZES = 25,
  PE_OPTHDR_BASE_OF_DATA = 26,
} pe_opthdr_field;

typedef struct {
  uint64 flink, blink;
} list_entry;

#define PE_SIZEOF_SHORT_NAME 8
typedef struct __unaligned {
  char name[PE_SIZEOF_SHORT_NAME];
  union {
    uint32 physical_address;
    uint32 virtual_size;
  } misc;
  uint32 virtual_address;
  uint32 size_of_raw_data;
  uint32 pointer_to_raw_data;
  uint32 pointer_to_relocations;
  uint32 pointer_to_linenumbers;
  uint16 number_of_relocations;
  uint16 number_of_linenumbers;
  uint32 characteristics;
}
section_header;

typedef struct {
  union {
    uint64 forwarder_string;
    uint64 function;
    uint64 ordinal;
    uint64 address_of_data;
  } u1;
} pe64_thunk_data;

typedef struct {
  union {
    uint32 forwarder_string;
    uint32 function;
    uint32 ordinal;
    uint32 address_of_data;
  } u1;
} pe_thunk_data32;

// typedef struct {
//  union {
//    uint32 characteristics;
//    uint32 original_first_thunk;
//  };
//  uint32 time_date_stamp;
//
//  uint32 forwarder_chain;
//  uint32 name;
//  uint32 first_thunk;
//} pe_import_descriptor;

typedef struct __unaligned {
  uint32 signature;
  pe_coff_header coff_header;
  pe64_opt_header optional_header;
}
pe64_nt_headers;

typedef struct {
  uint32 signature;
  pe_coff_header coff_header;
  pe32_opt_header optional_header;
} pe32_nt_headers;

typedef struct {
  char* module_name;            /* 0x00 (PSTR) */
  fd_t h_file;                  /* 0x08 (HANDLE) */
  char* base;                   /* 0x10 (PUCHAR) */
  pe64_nt_headers* file_header; /* 0x18 (PIMAGE_NT_HEADERS64) */
  uint64 last_rva_section;      /* 0x20 (pe_section_header) */
  uint16 number_of_sections;    /* 0x28 (ULONG) */
  pe_section_header* sections;  /* 0x30 (pe_section_header) */
  uint32 characteristics;       /* 0x38 (ULONG) */
  char f_system_image;          /* 0x3c (BOOLEAN) */
  char fdos_image;              /* 0x3d (BOOLEAN) */
  char f_read_only;             /* 0x3e (BOOLEAN) */
  char version;                 /* 0x3f (unsigned char) */
  list_entry links;             /* 0x40 (LIST_ENTRY) */
  uint64 size_of_image;         /* 0x50 (unsigned long) */
} pe_loaded_image;

//
// Image architectures
//
#define PE_FILE_MACHINE_UNKNOWN 0
#define PE_FILE_MACHINE_I386 0x014c
#define PE_FILE_MACHINE_R3000 0x0162
#define PE_FILE_MACHINE_R4000 0x0166
#define PE_FILE_MACHINE_R10000 0x0168
#define PE_FILE_MACHINE_WCEMIPSV2 0x0169
#define PE_FILE_MACHINE_ALPHA 0x0184
#define PE_FILE_MACHINE_SH3 0x01a2
#define PE_FILE_MACHINE_SH3DSP 0x01a3
#define PE_FILE_MACHINE_SH3E 0x01a4
#define PE_FILE_MACHINE_SH4 0x01a6
#define PE_FILE_MACHINE_SH5 0x01a8
#define PE_FILE_MACHINE_ARM 0x01c0
#define PE_FILE_MACHINE_ARMV7 0x01c4
#define PE_FILE_MACHINE_ARMNT 0x01c4
#define PE_FILE_MACHINE_ARM64 0xaa64
#define PE_FILE_MACHINE_THUMB 0x01c2
#define PE_FILE_MACHINE_AM33 0x01d3
#define PE_FILE_MACHINE_POWERPC 0x01F0
#define PE_FILE_MACHINE_POWERPCFP 0x01f1
#define PE_FILE_MACHINE_IA64 0x0200
#define PE_FILE_MACHINE_MIPS16 0x0266
#define PE_FILE_MACHINE_ALPHA64 0x0284
#define PE_FILE_MACHINE_MIPSFPU 0x0366
#define PE_FILE_MACHINE_MIPSFPU16 0x0466
#define PE_FILE_MACHINE_AXP64 PE_FILE_MACHINE_ALPHA64
#define PE_FILE_MACHINE_TRICORE 0x0520
#define PE_FILE_MACHINE_CEF 0x0CEF
#define PE_FILE_MACHINE_EBC 0x0EBC
#define PE_FILE_MACHINE_AMD64 0x8664
#define PE_FILE_MACHINE_M32R 0x9041
#define PE_FILE_MACHINE_CEE 0xc0ee

#define PE_FILE_RELOCS_STRIPPED 0x0001
#define PE_FILE_EXECUTABLE_IMAGE 0x0002
#define PE_FILE_LINE_NUMS_STRIPPED 0x0004
#define PE_FILE_LOCAL_SYMS_STRIPPED 0x0008
#define PE_FILE_AGGRESIVE_WS_TRIM 0x0010
#define PE_FILE_RELOCS_STRIPPED 0x0001
#define PE_FILE_EXECUTABLE_IMAGE 0x0002
#define PE_FILE_LINE_NUMS_STRIPPED 0x0004
#define PE_FILE_LOCAL_SYMS_STRIPPED 0x0008
#define PE_FILE_AGGRESIVE_WS_TRIM 0x0010
#define PE_FILE_LARGE_ADDRESS_AWARE 0x0020
#define PE_FILE_BYTES_REVERSED_LO 0x0080
#define PE_FILE_32BIT_MACHINE 0x0100
#define PE_FILE_DEBUG_STRIPPED 0x0200
#define PE_FILE_REMOVABLE_RUN_FROM_SWAP 0x0400
#define PE_FILE_NET_RUN_FROM_SWAP 0x0800
#define PE_FILE_SYSTEM 0x1000
#define PE_FILE_DLL 0x2000
#define PE_FILE_UP_SYSTEM_ONLY 0x4000
#define PE_FILE_BYTES_REVERSED_HI 0x8000

#define PE_NT_SIGNATURE 0x00004550
#define PE_NT_OPTIONAL_HDR32_MAGIC 0x10b
#define PE_NT_OPTIONAL_HDR64_MAGIC 0x20b
//#define PE_NT_OPTIONAL_HDR_MAGIC        PE_NT_OPTIONAL_HDR64_MAGIC
#define PE_NT_OPTIONAL_HDR_MAGIC PE_NT_OPTIONAL_HDR32_MAGIC

#define PE_TYPE(pe) ((pe_type) * (uint16*)pe_header_opt((pe)))

void* pe_header_nt(void*);
void* pe_header_opt(void*);
int pe_rva2section(void* base, uint32 rva);
int64 pe_rva2offset(void*, uint32 rva);
void* pe_rva2ptr(void*, uint32 rva);
uint32 pe_header_sig(void*);
uint32 pe_offset2rva(uint8*, int64 off);
uint64 pe_thunk(void*, void* ptr, int64 index);
pe_coff_header* pe_header_coff(void*);
pe32_nt_headers* pe_header_nt32(void*);
pe64_nt_headers* pe_header_nt64(void*);
pe32_opt_header* pe_header_opt32(void*);
pe64_opt_header* pe_header_opt64(void*);
pe_section_header* pe_get_section(void*, const char* name);
pe_data_directory* pe_header_datadir(void*);
pe_section_header* pe_header_sections(void*, uint16* nsections);
pe_data_directory* pe_get_datadir(void*, uint32* num);
const char* pe_datadir_name(int);
char* pe_dllname(uint8*);

#define PE_FIELD_OFFSET(type, field) ((size_t)(uint8*)&(((type*)0)->field))
#define PE_FIELD_SIZE(type, field) sizeof(((type*)0)->field)
#define PE_STRUCT_OFFSETS(st, field) PE_FIELD_OFFSET(pe32_##st, field), PE_FIELD_SIZE(pe32_##st, field), PE_FIELD_OFFSET(pe64_##st, field), PE_FIELD_SIZE(pe64_##st, field))

#define PE32_FIRST_SECTION(ntheader)                                                               \
  ((pe_section_header*)((uint8*)ntheader + PE_FIELD_OFFSET(pe32_nt_headers, optional_header) +     \
                        ((pe32_nt_headers*)(ntheader))->coff_header.size_of_optional_header))
#define PE64_FIRST_SECTION(ntheader)                                                               \
  ((pe_section_header*)((uint8*)ntheader + PE_FIELD_OFFSET(pe64_nt_headers, optional_header) +     \
                        ((pe64_nt_headers*)(ntheader))->coff_header.size_of_optional_header))

#define PE_OFFSET(pe, st, field)                                                                   \
  (PE_64(pe) ? PE_FIELD_OFFSET(pe64_##st, field) : PE_FIELD_OFFSET(pe32_##st, field))

#define PE_ADDR(pe, ptr, st, field) ((void*)(((char*)ptr) + PE_OFFSET(pe, st, field)))

#define PE_SIZE(pe, st, field)                                                                     \
  (PE_64(pe) ? PE_FIELD_SIZE(pe64_##st, field) : PE_FIELD_SIZE(pe32_##st, field))

#define PE_STRUCT_SIZE(pe, st) (PE_64(pe) ? sizeof(pe64_##st) : sizeof(pe32_##st))

#define PE_GET(pe, ptr, st, field)                                                                 \
  pe_get_value(pe,                                                                                 \
               ptr,                                                                                \
               PE_FIELD_OFFSET(pe32_##st, field),                                                  \
               PE_FIELD_SIZE(pe32_##st, field),                                                    \
               PE_FIELD_OFFSET(pe64_##st, field),                                                  \
               PE_FIELD_SIZE(pe64_##st, field))
#define PE_MAGIC(pe) uint16_read((const char*)pe_header_opt(pe))

#define PE_BITS(pe) (PE_MAGIC(pe) == PE_MAGIC_PE64 ? 64 : 32)
#define PE_64(pe) (PE_MAGIC(pe) == PE_MAGIC_PE64)
#define PE_32(pe) (PE_MAGIC(pe) == PE_MAGIC_PE32)

uint64
pe_get_value(void* pe, void* ptr, unsigned off32, unsigned size32, unsigned off64, unsigned size64);

#ifdef __cplusplus
}
#endif
#endif /* defined PE_H */
