#ifndef COFF_H
#define COFF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "uint8.h"
#include "uint16.h"
#include "uint32.h"

typedef struct {
  uint16 machine; /* machine_type */
  uint16 number_of_sections;
  uint32 time_date_stamp;
  uint32 pointer_to_symbol_table;
  uint32 number_of_symbols;
  uint16 size_of_optional_header;
  uint16 characteristics; /* image_characteristics */
} coff_file_header;

#define COFF_FILE_RELOCS_STRIPPED         0x0001
#define COFF_FILE_EXECUTABLE_IMAGE        0x0002
#define COFF_FILE_LINE_NUMS_STRIPPED      0x0004
#define COFF_FILE_LOCAL_SYMS_STRIPPED     0x0008
#define COFF_FILE_AGGRESIVE_WS_TRIM       0x0010
#define COFF_FILE_LARGE_ADDRESS_AWARE     0x0020
#define COFF_FILE_BYTES_REVERSED_LO       0x0080
#define COFF_FILE_32BIT_MACHINE           0x0100
#define COFF_FILE_MACHINE_I386            0x014c
#define COFF_FILE_MACHINE_R3000           0x0162
#define COFF_FILE_MACHINE_R4000           0x0166
#define COFF_FILE_MACHINE_R10000          0x0168
#define COFF_FILE_MACHINE_WCEMIPSV2       0x0169
#define COFF_FILE_MACHINE_ALPHA           0x0184
#define COFF_FILE_MACHINE_POWERPC         0x01f0
#define COFF_FILE_MACHINE_SH3             0x01a2
#define COFF_FILE_MACHINE_SH3DSP          0x01a3
#define COFF_FILE_MACHINE_SH3E            0x01a4
#define COFF_FILE_MACHINE_SH4             0x01a6
#define COFF_FILE_MACHINE_SH5             0x01a8
#define COFF_FILE_MACHINE_ARM             0x01c0
#define COFF_FILE_MACHINE_THUMB           0x01c2
#define COFF_FILE_MACHINE_ARMNT           0x01c4
#define COFF_FILE_MACHINE_ARMV7           0x01c4
#define COFF_FILE_MACHINE_AM33            0x01d3
#define COFF_FILE_MACHINE_POWERPCFP       0x01f1
#define COFF_FILE_DEBUG_STRIPPED          0x0200
#define COFF_FILE_MACHINE_IA64            0x0200
#define COFF_FILE_MACHINE_MIPS16          0x0266
#define COFF_FILE_MACHINE_ALPHA64         0x0284
#define COFF_FILE_MACHINE_MIPSFPU         0x0366
#define COFF_FILE_REMOVABLE_RUN_FROM_SWAP 0x0400
#define COFF_FILE_MACHINE_MIPSFPU16       0x0466
#define COFF_FILE_MACHINE_TRICORE         0x0520
#define COFF_FILE_NET_RUN_FROM_SWAP       0x0800
#define COFF_FILE_MACHINE_CEF             0x0cef
#define COFF_FILE_MACHINE_EBC             0x0ebc
#define COFF_FILE_SYSTEM                  0x1000
#define COFF_FILE_DLL                     0x2000
#define COFF_FILE_UP_SYSTEM_ONLY          0x4000
#define COFF_FILE_BYTES_REVERSED_HI       0x8000
#define COFF_FILE_MACHINE_AMD64           0x8664
#define COFF_FILE_MACHINE_M32R            0x9041
#define COFF_FILE_MACHINE_ARM64           0xaa64
#define COFF_FILE_MACHINE_CEE             0xc0ee

#define COFF_SECTION_NAME_SIZE 8

/* Quoting pecoff_v8.docx: "Entries in the section table are numbered starting from one (1)". */
typedef struct {
  char name[COFF_SECTION_NAME_SIZE]; /* TODO: Should we use char instead? */
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
} coff_section_header;

typedef struct {
  uint16 magic;
  uint8 major_linker_version;
  uint8 minor_linker_version;
  uint32 size_of_code;
  uint32 size_of_initialized_data;
  uint32 size_of_uninitialized_data;
  uint32 address_of_entry_point;
  uint32 base_of_code;
  uint32 base_of_data;
} coff_opt_header;

coff_file_header*    coff_header_file(void*);
coff_opt_header*     coff_header_opt(void*);
coff_section_header* coff_header_sections(void*, uint16* nsections);

#define COFF_TYPE(coff) ((coff_type) * (uint16*)coff_header_opt((coff)))
#define COFF_FIELD_OFFSET(type, field) ((size_t)(uint8*)&(((type*)0)->field))
#define COFF_FIELD_SIZE(type, field) sizeof(((type*)0)->field)
#define COFF_ADDR(coff, ptr, st, field) ((void*)(((char*)ptr)+COFF_OFFSET(coff, st, field)))
#define COFF_ADDR(coff, ptr, st, field) ((void*)(((char*)ptr)+COFF_OFFSET(coff, st, field)))
#define COFF_MAGIC(coff) uint16_read(coff_header_opt(coff))

#ifdef __cplusplus
}
#endif
#endif /* defined COFF_H */
