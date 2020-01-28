#ifndef COFF_H
#define COFF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "uint8.h"
#include "uint16.h"
#include "uint32.h"
#include "range.h"

typedef struct __unaligned {
  uint16 machine; /* machine_type */
  uint16 number_of_sections;
  uint32 time_date_stamp;
  uint32 pointer_to_symbol_table;
  uint32 number_of_symbols;
  uint16 size_of_optional_header;
  uint16 characteristics; /* image_characteristics */
}
coff_file_header;

#define COFF_FILE_RELOCS_STRIPPED 0x0001
#define COFF_FILE_EXECUTABLE_IMAGE 0x0002
#define COFF_FILE_LINE_NUMS_STRIPPED 0x0004
#define COFF_FILE_LOCAL_SYMS_STRIPPED 0x0008
#define COFF_FILE_AGGRESIVE_WS_TRIM 0x0010
#define COFF_FILE_LARGE_ADDRESS_AWARE 0x0020
#define COFF_FILE_BYTES_REVERSED_LO 0x0080
#define COFF_FILE_32BIT_MACHINE 0x0100
#define COFF_FILE_MACHINE_I386 0x014c
#define COFF_FILE_MACHINE_R3000 0x0162
#define COFF_FILE_MACHINE_R4000 0x0166
#define COFF_FILE_MACHINE_R10000 0x0168
#define COFF_FILE_MACHINE_WCEMIPSV2 0x0169
#define COFF_FILE_MACHINE_ALPHA 0x0184
#define COFF_FILE_MACHINE_POWERPC 0x01f0
#define COFF_FILE_MACHINE_SH3 0x01a2
#define COFF_FILE_MACHINE_SH3DSP 0x01a3
#define COFF_FILE_MACHINE_SH3E 0x01a4
#define COFF_FILE_MACHINE_SH4 0x01a6
#define COFF_FILE_MACHINE_SH5 0x01a8
#define COFF_FILE_MACHINE_ARM 0x01c0
#define COFF_FILE_MACHINE_THUMB 0x01c2
#define COFF_FILE_MACHINE_ARMNT 0x01c4
#define COFF_FILE_MACHINE_ARMV7 0x01c4
#define COFF_FILE_MACHINE_AM33 0x01d3
#define COFF_FILE_MACHINE_POWERPCFP 0x01f1
#define COFF_FILE_DEBUG_STRIPPED 0x0200
#define COFF_FILE_MACHINE_IA64 0x0200
#define COFF_FILE_MACHINE_MIPS16 0x0266
#define COFF_FILE_MACHINE_ALPHA64 0x0284
#define COFF_FILE_MACHINE_MIPSFPU 0x0366
#define COFF_FILE_REMOVABLE_RUN_FROM_SWAP 0x0400
#define COFF_FILE_MACHINE_MIPSFPU16 0x0466
#define COFF_FILE_MACHINE_TRICORE 0x0520
#define COFF_FILE_NET_RUN_FROM_SWAP 0x0800
#define COFF_FILE_MACHINE_CEF 0x0cef
#define COFF_FILE_MACHINE_EBC 0x0ebc
#define COFF_FILE_SYSTEM 0x1000
#define COFF_FILE_MACHINE_MICROCHIP_V1 0x1234
#define COFF_FILE_MACHINE_MICROCHIP_V2 0x1240
#define COFF_FILE_DLL 0x2000
#define COFF_FILE_UP_SYSTEM_ONLY 0x4000
#define COFF_FILE_BYTES_REVERSED_HI 0x8000
#define COFF_FILE_MACHINE_AMD64 0x8664
#define COFF_FILE_MACHINE_M32R 0x9041
#define COFF_FILE_MACHINE_ARM64 0xaa64
#define COFF_FILE_MACHINE_CEE 0xc0ee

#define COFF_SECTION_NAME_SIZE 8

/* Quoting pecoff_v8.docx: "Entries in the section table are numbered starting from one (1)". */
typedef struct __unaligned {
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
}
coff_section_header;

#define COFF_SECTION_UNDEF ((int16)0)       // external symbol
#define COFF_SECTION_ABSOLUTE ((int16)-1)   // value of symbol is absolute
#define COFF_SECTION_DEBUG ((int16)-2)      // debugging symbol - value is meaningless
#define COFF_SECTION_N_TV ((int16)-3)       // indicates symbol needs preload transfer vector
#define COFF_SECTION_P_TV ((int16)-4)       // indicates symbol needs postload transfer vector
#define COFF_SECTION_REMOVE_ME ((int16)-99) // Specific for objconv program: Debug or exception section being removed

typedef struct __unaligned {
  uint16 magic;
  uint8 major_linker_version;
  uint8 minor_linker_version;
  uint32 size_of_code;
  uint32 size_of_initialized_data;
  uint32 size_of_uninitialized_data;
  uint32 address_of_entry_point;
  uint32 base_of_code;
  uint32 base_of_data;
}
coff_opt_header;

typedef struct __unaligned {
  uint16 magic;
  uint32 vstamp; /* version of the compiler assembler */
  uint32 proc_type;
  uint32 rom_width_bits;
  uint32 ram_width_bits;
}
coff_opt_header_microchip;

#define COFF_OPT_MAGIC_MICROCHIP_V1 0x5678
#define COFF_OPT_MAGIC_MICROCHIP_V2 0x5678

typedef struct __unaligned {
  union {
    uint32 fname; // function name symbol table index, if Line == 0
    uint32 addr;  // section-relative address of code that corresponds to line
  };
  uint16 line; // line number
}
coff_line_number;

/*
 *  All symbols and sections have the following definition
 */
#define COFF_E_SYMNMLEN 8 /* # characters in a short symbol name	*/

typedef union {
  struct __unaligned {
    union {
      char name[COFF_E_SYMNMLEN]; /* Symbol name (first 8 characters) */
      struct {
        uint32 zeroes; /* Leading zeros */
        uint32 offset; /* Offset if this is a header section */
      };
    };

    uint32 value; /* Value (address) of the segment */
    uint16 scnum; /* Section number */
    uint16 type;  /* Type of section */
    uint8 sclass; /* Loader class */
    uint8 numaux; /* Number of auxiliary entries which follow */
  }
  e;

  // Function definition
  struct __unaligned {
    uint32 tag_index;                // Index to .bf entry
    uint32 code_size;                // Size of function code
    uint32 pointer_to_line_number;   // Pointer to line number entry
    uint32 pointer_to_next_function; // Symbol table index of next function
    uint16 x_tvndx;                  // Unused
  }
  func;

  // .bf abd .ef
  struct __unaligned {
    uint32 unused1;
    uint16 source_line_number; // Line number in source file
    uint16 unused2;
    uint32 unused3;                  // Pointer to line number entry
    uint32 pointer_to_next_function; // Symbol table index of next function
    uint16 unused4;                  // Unused
  }
  bfef;

  // Weak external
  struct __unaligned {
    uint32 tag_index;       // Symbol table index of alternative symbol2
    uint32 characteristics; //
    uint32 unused1;
    uint32 unused2;
    uint16 unused3; // Unused
  }
  weak;

  // File name
  struct __unaligned {
    char file_name[18]; //           File name
  }
  filename;

  // String table index
  struct {         // MS COFF uses multiple aux records rather than a string table entry!
    uint32 zeroes; // zeroes if name file name longer than 18
    uint32 offset; // string table entry
  } stringindex;

  // Section definition
  struct __unaligned {
    uint32 length;
    uint16 number_of_relocations; // Line number in source file
    uint16 number_of_line_numbers;
    uint32 check_sum; // Pointer to line number entry
    uint16 number;    // Symbol table index of next function
    uint8 selection;  // Unused
    uint8 unused1[3];
  }
  section;

} coff_symtab_entry;

#define COFF_SSYMBOL_NAME_MAX 8

/* symbol table entry */
typedef struct __unaligned {
  union __unaligned {
    char name[COFF_SSYMBOL_NAME_MAX]; /* symbol name if less than 8 characters */
    struct __unaligned {
      uint32 zeroes; /* first four characters are 0 */
      uint32 offset; /* pointer to the string table */
    };
  };

  uint32 value; /* symbol value */
  int16 scnum;  /* section number */
  uint32 type;  /* type */
  int8 sclass;  /* storage class */
  uint8 numaux; /* number of auxiliary symbols */
}
coff_symtab_entry_microchip;

#define COFF_SYMTAB_MCHP_TYPE_NULL 0    /* null */
#define COFF_SYMTAB_MCHP_TYPE_VOID 1    /* void */
#define COFF_SYMTAB_MCHP_TYPE_CHAR 2    /* character */
#define COFF_SYMTAB_MCHP_TYPE_SHORT 3   /* short integer */
#define COFF_SYMTAB_MCHP_TYPE_INT 4     /* integer */
#define COFF_SYMTAB_MCHP_TYPE_LONG 5    /* long integer */
#define COFF_SYMTAB_MCHP_TYPE_FLOAT 6   /* floating point */
#define COFF_SYMTAB_MCHP_TYPE_DOUBLE 7  /* double length floating point */
#define COFF_SYMTAB_MCHP_TYPE_STRUCT 8  /* structure */
#define COFF_SYMTAB_MCHP_TYPE_UNION 9   /* union */
#define COFF_SYMTAB_MCHP_TYPE_ENUM 10   /* enumeration */
#define COFF_SYMTAB_MCHP_TYPE_MOE 11    /* member of enumeration */
#define COFF_SYMTAB_MCHP_TYPE_UCHAR 12  /* unsigned character */
#define COFF_SYMTAB_MCHP_TYPE_USHORT 13 /* unsigned short */
#define COFF_SYMTAB_MCHP_TYPE_UINT 14   /* unsigned integer */
#define COFF_SYMTAB_MCHP_TYPE_ULONG 15  /* unsigned long */

#define SYMBOL_SIZE_v1 18
#define SYMBOL_SIZE_v2 20

#define COFF_C_NULL 0     /* null */
#define COFF_C_AUTO 1     /* automatic variable */
#define COFF_C_EXT 2      /* external symbol */
#define COFF_C_STATIC 3   /* static */
#define COFF_C_REG 4      /* register variable */
#define COFF_C_EXTDEF 5   /* external definition */
#define COFF_C_LABEL 6    /* label */
#define COFF_C_ULABEL 7   /* undefined label */
#define COFF_C_MOS 8      /* member of structure */
#define COFF_C_ARG 9      /* function argument */
#define COFF_C_STRTAG 10  /* structure tag */
#define COFF_C_MOU 11     /* member of union */
#define COFF_C_UNTAG 12   /* union tag */
#define COFF_C_TPDEF 13   /* type definition */
#define COFF_C_USTATIC 14 /* undefined static */
#define COFF_C_ENTAG 15   /* enumeration tag */
#define COFF_C_MOE 16     /* member of enumeration */
#define COFF_C_REGPARM 17 /* register parameter */
#define COFF_C_FIELD 18   /* bit field */
#define COFF_C_AUTOARG 19 /* auto argument */
#define COFF_C_LASTENT 20 /* dummy entry (end of block) */
#define COFF_C_BLOCK 100  /* ".bb" or ".eb" */
#define COFF_C_FCN 101    /* ".bf" or ".ef" */
#define COFF_C_EOS 102    /* end of structure */
#define COFF_C_FILE 103   /* file name */
#define COFF_C_LINE 104   /* line # reformatted as symbol table entry */
#define COFF_C_ALIAS 105  /* duplicate tag */
#define COFF_C_HIDDEN 106 /* ext symbol in dmert public lib */
#define COFF_C_EFCN 255   /* physical end of function */

coff_section_header* coff_get_section(void*, uint16 index);
const char* coff_get_strtab(void*, uint32* szptr);
coff_symtab_entry* coff_get_symtab(void*, uint32* num);
coff_file_header* coff_header_file(void*);
coff_opt_header* coff_header_opt(void*);
coff_section_header* coff_header_sections(void*, uint16* nsections);
coff_symtab_entry* coff_index_symtab(void*, int index);
range coff_line_numbers(void*, coff_section_header* shdr);
const char* coff_sclass_name(uint8);
const char* coff_storage_class(uint8);
const char* coff_symbol_name(void*, coff_symtab_entry* sym);
range coff_symbol_table(void*);

#define COFF_TYPE(coff) ((coff_type) * (uint16*)coff_header_opt((coff)))
#define COFF_FIELD_OFFSET(type, field) ((size_t)(uint8*)&(((type*)0)->field))
#define COFF_FIELD_SIZE(type, field) sizeof(((type*)0)->field)
#define COFF_ADDR(coff, ptr, st, field) ((void*)(((char*)ptr) + COFF_OFFSET(coff, st, field)))
#define COFF_ADDR(coff, ptr, st, field) ((void*)(((char*)ptr) + COFF_OFFSET(coff, st, field)))
#define COFF_MAGIC(coff) uint16_read(coff_header_opt(coff))

#ifdef __cplusplus
}
#endif
#endif /* defined COFF_H */
