#ifndef PE_H
#define PE_H

#include <sys/types.h>
#include "uint16.h"
#include "uint32.h"
#include "uint64.h"

typedef struct {
  unsigned char* x;
  size_t n;
} pe_file;

typedef struct {
  uint32 virtual_address;
  uint32 size;
} pe_data_directory,pe32_data_directory;

typedef struct {
  uint64 virtual_address;
  uint64 size;
} pe64_data_directory;

typedef struct {
  uint32 characteristics;
  uint32 time_date_stamp;
  uint16 major_version;
  uint16 minor_version;
  uint32 name;
  uint32 base;
  uint32 number_of_functions;
  uint32 number_of_names;
  uint32 address_of_functions;
  uint32 address_of_names;
  uint32 address_of_name_ordinals;
} pe_export_directory;

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
  uint16  hint;
    unsigned char   name[1];
} pe_import_by_name;

typedef struct {
  union {
    uint32 all_attributes;
    struct {
      uint32 rva_based : 1;
      uint32 reserved_attributes : 31;
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

typedef struct {
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
} pe_dos_header;

typedef struct {
  uint16 machine; /* machine_type */
  uint16 number_of_sections;
  uint32 time_date_stamp;
  uint32 pointer_to_symbol_table;
  uint32 number_of_symbols;
  uint16 size_of_optional_header;
  uint16 characteristics; /* image_characteristics */
} pe_file_header;

#define PE_SECTION_NAME_SIZE 8

/* Quoting pecoff_v8.docx: "Entries in the section table are numbered starting from one (1)". */
typedef struct {
  unsigned char name[PE_SECTION_NAME_SIZE]; /* TODO: Should we use char instead? */
  union {
    uint32 physical_address; /* same value as next field */
    uint32 virtual_size;
  } misc;
  uint32 virtual_address;
  uint32 size_of_raw_data;
  uint32 pointer_to_raw_data;
  uint32 pointer_to_relocations; /* always zero in executables */
  uint32 pointer_to_linenumbers; /* deprecated */
  uint16 number_of_relocations;
  uint16 number_of_linenumbers; /* deprecated */
  uint32 characteristics; /* section_characteristics */
} pe_section_header;


#define PE_NUMBEROF_DIRECTORY_ENTRIES 16

/* r_eference: http://msdn.microsoft.com/en-us/library/windows/desktop/ms680339(v=vs.85).aspx */
typedef struct {
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
  pe32_data_directory data_directory[PE_NUMBEROF_DIRECTORY_ENTRIES];
} pe32_opt_header;

typedef struct {
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
  pe64_data_directory data_directory[PE_NUMBEROF_DIRECTORY_ENTRIES];
} pe64_opt_header;
/*
typedef struct {
  pe_dos_header      dos_hdr;
  uint32             signature;
  pe_file_header*    coff_hdr;
  void*              optional_hdr_ptr;
  pe_optional_header optional_hdr;
  uint32             num_directories;
  void*              directories_ptr;
  uint16             num_sections;
  void*              sections_ptr;
  uint64             entrypoint;
  uint64             imagebase;
} pe_file_t;
*/
typedef enum {
  MAGIC_ROM   = 0x107,
  MAGIC_PE32  = 0x10b,
  MAGIC_PE64  = 0x20b /* PE32+ */
} pe_type;

typedef enum {
  PE_OPTHDR_MAGIC                          =  0,
  PE_OPTHDR_SIZE_OF_CODE                   =  1,
  PE_OPTHDR_SIZE_OF_INITIALIZED_DATA       =  2,
  PE_OPTHDR_SIZE_OF_UNINITIALIZED_DATA     =  3,
  PE_OPTHDR_ADDRESS_OF_ENTRY_POINT         =  4,
  PE_OPTHDR_BASE_OF_CODE                   =  5,
  PE_OPTHDR_IMAGE_BASE                     =  6,
  PE_OPTHDR_SECTION_ALIGNMENT              =  7,
  PE_OPTHDR_FILE_ALIGNMENT                 =  8,
  PE_OPTHDR_MAJOR_OPERATING_SYSTEM_VERSION =  9,
  PE_OPTHDR_MINOR_OPERATING_SYSTEM_VERSION = 10,
  PE_OPTHDR_MAJOR_IMAGE_VERSION            = 11,
  PE_OPTHDR_MINOR_IMAGE_VERSION            = 12,
  PE_OPTHDR_MAJOR_SUBSYSTEM_VERSION        = 13,
  PE_OPTHDR_MINOR_SUBSYSTEM_VERSION        = 14,
  PE_OPTHDR_RESERVED1                      = 15,
  PE_OPTHDR_SIZE_OF_IMAGE                  = 16,
  PE_OPTHDR_SIZE_OF_HEADERS                = 17,
  PE_OPTHDR_CHECKSUM                       = 18,
  PE_OPTHDR_DLL_CHARACTERISTICS            = 19,
  PE_OPTHDR_SIZE_OF_STACK_RESERVE          = 20,
  PE_OPTHDR_SIZE_OF_STACK_COMMIT           = 21,
  PE_OPTHDR_SIZE_OF_HEAP_RESERVE           = 22,
  PE_OPTHDR_SIZE_OF_HEAP_COMMIT            = 23,
  PE_OPTHDR_LOADER_FLAGS                   = 24,
  PE_OPTHDR_NUMBER_OF_RVA_AND_SIZES        = 25,
  PE_OPTHDR_BASE_OF_DATA                   = 26,
} pe_opthdr_field;

typedef struct {
  uint64 flink, blink;
} list_entry;

#define  PE_SIZEOF_SHORT_NAME 8 
typedef struct {
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
} section_header;
  
typedef struct {
  union {
    uint64 forwarder_string;
    uint64 function;
    uint64 ordinal;
    uint64 address_of_data;
  } u1;
} pe_thunk_data64;

typedef struct {
  union {
    uint32 forwarder_string;
    uint32 function;
    uint32 ordinal;
    uint32 address_of_data;
  } u1;
} pe_thunk_data32;

//typedef struct {
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


typedef struct {
  uint32 Signature;
  pe_file_header file_header;
  pe64_opt_header optional_header;
} pe_nt_headers64;

/*
typedef struct {
  uint32 Signature;
  pe_file_header file_header;
  pe32_opt_header optional_header;
} pe_nt_headers32;

*/


typedef struct {
  uint64             module_name;         /* 0x00 (PSTR) */
  uint64             h_file;              /* 0x08 (HANDLE) */
  uint64             mapped_address;      /* 0x10 (PUCHAR) */
  pe_nt_headers64*    file_header;         /* 0x18 (PIMAGE_NT_HEADERS64) */
  uint64             last_rva_section;    /* 0x20 (pe_section_header) */
  uint64             number_of_sections;  /* 0x28 (ULONG) */
  section_header*    sections;            /* 0x30 (pe_section_header) */
  uint32             characteristics;     /* 0x38 (ULONG) */
  char               f_system_image;      /* 0x3c (BOOLEAN) */
  char               fdos_image;          /* 0x3d (BOOLEAN) */
  char               f_read_only;         /* 0x3e (BOOLEAN) */
  char               version;             /* 0x3f (UCHAR) */
  list_entry         links;                       /* 0x40 (LIST_ENTRY) */
  uint64             size_of_image;       /* 0x50 (ULONG) */
} pe_loaded_image;

//
// Image architectures
//
#define PE_FILE_MACHINE_AM33       0x1d3
#define PE_FILE_MACHINE_AMD64      0x8664
#define PE_FILE_MACHINE_ARM        0x1c0
#define PE_FILE_MACHINE_ARMV7      0x1c4
#define PE_FILE_MACHINE_EBC        0xebc
#define PE_FILE_MACHINE_I386       0x14c
#define PE_FILE_MACHINE_IA64       0x200
#define PE_FILE_MACHINE_M32R       0x9041
#define PE_FILE_MACHINE_MIPS16     0x266
#define PE_FILE_MACHINE_MIPSFPU    0x366
#define PE_FILE_MACHINE_MIPSFPU16  0x466
#define PE_FILE_MACHINE_POWERPC    0x1f0
#define PE_FILE_MACHINE_POWERPCFP  0x1f1
#define PE_FILE_MACHINE_R4000      0x166
#define PE_FILE_MACHINE_SH3        0x1a2
#define PE_FILE_MACHINE_SH3E       0x01a4


#define PE_DIRECTORY_ENTRY_DELAY_IMPORT 13
#define PE_DIRECTORY_ENTRY_EXPORT 0
#define PE_DIRECTORY_ENTRY_IMPORT 1

pe_file_header*
pe_filehdr_ptr(const void*);

void*
pe_opthdr_ptr(const void*);

int32
pe_opthdr_offset(const void*, pe_opthdr_field);

#endif /* defined PE_H */
