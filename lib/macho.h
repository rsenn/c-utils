#ifndef MACHO_H
#define MACHO_H

#include "uint8.h"
#include "uint16.h"
#include "uint32.h"
#include "uint64.h"

#define MACHO_MH_OBJECT 0x1
#define MACHO_MH_EXECUTE 0x2
#define MACHO_MH_FVMLIB 0x3
#define MACHO_MH_CORE 0x4
#define MACHO_MH_PRELOAD 0x5
#define MACHO_MH_DYLIB 0x6
#define MACHO_MH_DYLINKER 0x7
#define MACHO_MH_BUNDLE 0x8
#define MACHO_MH_DYLIB_STUB 0x9
#define MACHO_MH_NOUNDEFS 0x1
#define MACHO_MH_INCRLINK 0x2
#define MACHO_MH_DYLDLINK 0x4
#define MACHO_MH_BINDATLOAD 0x8
#define MACHO_MH_PREBOUND 0x10
#define MACHO_MH_SPLIT_SEGS 0x20
#define MACHO_MH_LAZY_INIT 0x40
#define MACHO_MH_TWOLEVEL 0x80
#define MACHO_MH_FORCE_FLAT 0x100
#define MACHO_MH_NOMULTIDEFS 0x200
#define MACHO_MH_NOFIXPREBINDING 0x400
#define MACHO_MH_PREBINDABLE 0x800
#define MACHO_MH_ALLMODSBOUND 0x1000
#define MACHO_MH_SUBSECTIONS_VIA_SYMBOLS 0x2000
#define MACHO_MH_CANONICAL 0x4000
#define MACHO_MH_WEAK_DEFINES 0x8000
#define MACHO_MH_BINDS_TO_WEAK 0x10000
#define MACHO_MH_ALLOW_STACK_EXECUTION 0x20000

#define MACHO_LC_REQ_DYLD 0x80000000

#define MACHO_LC_SEGMENT 0x1
#define MACHO_LC_SYMTAB 0x2
#define MACHO_LC_SYMSEG 0x3
#define MACHO_LC_THREAD 0x4
#define MACHO_LC_UNIXTHREAD 0x5
#define MACHO_LC_LOADFVMLIB 0x6
#define MACHO_LC_IDFVMLIB 0x7
#define MACHO_LC_IDENT 0x8
#define MACHO_LC_FVMFILE 0x9
#define MACHO_LC_PREPAGE 0xa
#define MACHO_LC_DYSYMTAB 0xb
#define MACHO_LC_LOAD_DYLIB 0xc
#define MACHO_LC_ID_DYLIB 0xd
#define MACHO_LC_LOAD_DYLINKER 0xe
#define MACHO_LC_ID_DYLINKER 0xf
#define MACHO_LC_PREBOUND_DYLIB 0x10
#define MACHO_LC_ROUTINES 0x11
#define MACHO_LC_SUB_FRAMEWORK 0x12
#define MACHO_LC_SUB_UMBRELLA 0x13
#define MACHO_LC_SUB_CLIENT 0x14
#define MACHO_LC_SUB_LIBRARY 0x15
#define MACHO_LC_TWOLEVEL_HINTS 0x16
#define MACHO_LC_PREBIND_CKSUM 0x17

#define MACHO_LC_LOAD_WEAK_DYLIB (0x18 | MACHO_LC_REQ_DYLD)

#define MACHO_SG_HIGHVM 0x1
#define MACHO_SG_FVMLIB 0x2
#define MACHO_SG_NORELOC 0x4
#define MACHO_SG_PROTECTED_VERSION_1 0x8

#define MACHO_SECTION_TYPE 0x000000ff
#define MACHO_SECTION_ATTRIBUTES 0xffffff00

#define MACHO_S_REGULAR 0x0
#define MACHO_S_ZEROFILL 0x1
#define MACHO_S_CSTRING_LITERALS 0x2
#define MACHO_S_4BYTE_LITERALS 0x3
#define MACHO_S_8BYTE_LITERALS 0x4
#define MACHO_S_LITERAL_POINTERS 0x5

#define MACHO_S_NON_LAZY_SYMBOL_POINTERS 0x6
#define MACHO_S_LAZY_SYMBOL_POINTERS 0x7
#define MACHO_S_SYMBOL_STUBS 0x8
#define MACHO_S_MOD_INIT_FUNC_POINTERS 0x9
#define MACHO_S_MOD_TERM_FUNC_POINTERS 0xa
#define MACHO_S_COALESCED 0xb
#define MACHO_S_GB_ZEROFILL 0xc
#define MACHO_S_INTERPOSING 0xd

#define MACHO_SECTION_ATTRIBUTES_USR 0xff000000
#define MACHO_S_ATTR_PURE_INSTRUCTIONS 0x80000000
#define MACHO_S_ATTR_NO_TOC 0x40000000
#define MACHO_S_ATTR_STRIP_STATIC_SYMS 0x20000000
#define MACHO_S_ATTR_NO_DEAD_STRIP 0x10000000
#define MACHO_S_ATTR_LIVE_SUPPORT 0x08000000
#define MACHO_S_ATTR_SELF_MODIFYING_CODE 0x04000000
#define MACHO_SECTION_ATTRIBUTES_SYS 0x00ffff00
#define MACHO_S_ATTR_SOME_INSTRUCTIONS 0x00000400
#define MACHO_S_ATTR_EXT_RELOC 0x00000200
#define MACHO_S_ATTR_LOC_RELOC 0x00000100

#define MACHO_SEG_PAGEZERO "__PAGEZERO"
#define MACHO_SEG_TEXT "__TEXT"
#define MACHO_SECT_TEXT "__text"
#define MACHO_SECT_FVMLIB_INIT0 "__fvmlib_init0"
#define MACHO_SECT_FVMLIB_INIT1 "__fvmlib_init1"
#define MACHO_SEG_DATA "__DATA"
#define MACHO_SECT_DATA "__data"
#define MACHO_SECT_BSS "__bss"
#define MACHO_SECT_COMMON "__common"
#define MACHO_SEG_OBJC "__OBJC"
#define MACHO_SECT_OBJC_SYMBOLS "__symbol_table"
#define MACHO_SECT_OBJC_MODULES "__module_info"
#define MACHO_SECT_OBJC_STRINGS "__selector_strs"
#define MACHO_SECT_OBJC_REFS "__selector_refs"
#define MACHO_SEG_ICON "__ICON"
#define MACHO_SECT_ICON_HEADER "__header"
#define MACHO_SECT_ICON_TIFF "__tiff"
#define MACHO_SEG_LINKEDIT "__LINKEDIT"
#define MACHO_SEG_UNIXSTACK "__UNIXSTACK"
#define MACHO_SEG_IMPORT "__IMPORT"

/*
 * Capability bits used in the definition of cpu_type.
 */
#define MACHO_CPU_ARCH_MASK 0xff000000  /* mask for architecture bits */
#define MACHO_CPU_ARCH_ABI64 0x01000000 /* 64 bit ABI */

#define MACHO_CPU_TYPE_ANY ((cpu_type_t)-1)

#define MACHO_CPU_TYPE_VAX ((cpu_type_t)1)
/* skip        ((cpu_type_t) 2)  */
/* skip        ((cpu_type_t) 3)  */
/* skip        ((cpu_type_t) 4)  */
/* skip        ((cpu_type_t) 5)  */
#define MACHO_CPU_TYPE_MC680x0 ((cpu_type_t)6)
#define MACHO_CPU_TYPE_X86 ((cpu_type_t)7)
#define MACHO_CPU_TYPE_I386 MACHO_CPU_TYPE_X86 /* compatibility */
#define MACHO_CPU_TYPE_X86_64 (MACHO_CPU_TYPE_X86 | MACHO_CPU_ARCH_ABI64)

/* skip CPU_TYPE_MIPS    ((cpu_type_t) 8)  */
/* skip       ((cpu_type_t) 9)  */
#define MACHO_CPU_TYPE_MC98000 ((cpu_type_t)10)
#define MACHO_CPU_TYPE_HPPA ((cpu_type_t)11)
#define MACHO_CPU_TYPE_ARM ((cpu_type_t)12)
#define MACHO_CPU_TYPE_MC88000 ((cpu_type_t)13)
#define MACHO_CPU_TYPE_SPARC ((cpu_type_t)14)
#define MACHO_CPU_TYPE_I860 ((cpu_type_t)15)
/* skip  CPU_TYPE_ALPHA    ((cpu_type_t) 16)  */
/* skip        ((cpu_type_t) 17)  */
#define MACHO_CPU_TYPE_POWERPC ((cpu_type_t)18)
#define MACHO_CPU_TYPE_POWERPC64 (MACHO_CPU_TYPE_POWERPC | MACHO_CPU_ARCH_ABI64)

/*
 *  Machine subtypes (these are defined here, instead of in a machine
 *  dependent directory, so that any program can get all definitions
 *  regardless of where is it compiled).
 */

/*
 * Capability bits used in the definition of cpu_subtype.
 */
#define MACHO_CPU_SUBTYPE_MASK 0xff000000  /* mask for feature flags */
#define MACHO_CPU_SUBTYPE_LIB64 0x80000000 /* 64 bit libraries */

/*
 *  Object files that are hand-crafted to run on any
 *  implementation of an architecture are tagged with
 *  MACHO_CPU_SUBTYPE_MULTIPLE.  This functions essentially the same as
 *  the "ALL" subtype of an architecture except that it allows us
 *  to easily find object files that may need to be modified
 *  whenever a new implementation of an architecture comes out.
 *
 *  It is the responsibility of the implementor to make sure the
 *  software handles unsupported implementations elegantly.
 */
#define MACHO_CPU_SUBTYPE_MULTIPLE ((cpu_subtype_t)-1)
#define MACHO_CPU_SUBTYPE_LITTLE_ENDIAN ((cpu_subtype_t)0)
#define MACHO_CPU_SUBTYPE_BIG_ENDIAN ((cpu_subtype_t)1)

/*
 *     Machine threadtypes.
 *     This is none - not defined - for most machine types/subtypes.
 */
#define MACHO_CPU_THREADTYPE_NONE ((cpu_threadtype_t)0)

/*
 *  VAX subtypes (these do *not* necessary conform to the actual cpu
 *  ID assigned by DEC available via the SID register).
 */

#define MACHO_CPU_SUBTYPE_VAX_ALL ((cpu_subtype_t)0)
#define MACHO_CPU_SUBTYPE_VAX780 ((cpu_subtype_t)1)
#define MACHO_CPU_SUBTYPE_VAX785 ((cpu_subtype_t)2)
#define MACHO_CPU_SUBTYPE_VAX750 ((cpu_subtype_t)3)
#define MACHO_CPU_SUBTYPE_VAX730 ((cpu_subtype_t)4)
#define MACHO_CPU_SUBTYPE_UVAXI ((cpu_subtype_t)5)
#define MACHO_CPU_SUBTYPE_UVAXII ((cpu_subtype_t)6)
#define MACHO_CPU_SUBTYPE_VAX8200 ((cpu_subtype_t)7)
#define MACHO_CPU_SUBTYPE_VAX8500 ((cpu_subtype_t)8)
#define MACHO_CPU_SUBTYPE_VAX8600 ((cpu_subtype_t)9)
#define MACHO_CPU_SUBTYPE_VAX8650 ((cpu_subtype_t)10)
#define MACHO_CPU_SUBTYPE_VAX8800 ((cpu_subtype_t)11)
#define MACHO_CPU_SUBTYPE_UVAXIII ((cpu_subtype_t)12)

/*
 *   680x0 subtypes
 *
 * The subtype definitions here are unusual for historical reasons.
 * NeXT used to consider 68030 code as generic 68000 code.  For
 * backwards compatability:
 *
 *  MACHO_CPU_SUBTYPE_MC68030 symbol has been preserved for source code
 *  compatability.
 *
 *  MACHO_CPU_SUBTYPE_MC680x0_ALL has been defined to be the same
 *  subtype as MACHO_CPU_SUBTYPE_MC68030 for binary comatability.
 *
 *  MACHO_CPU_SUBTYPE_MC68030_ONLY has been added to allow new object
 *  files to be tagged as containing 68030-specific instructions.
 */

#define MACHO_CPU_SUBTYPE_MC680x0_ALL ((cpu_subtype_t)1)
#define MACHO_CPU_SUBTYPE_MC68030 ((cpu_subtype_t)1) /* compat */
#define MACHO_CPU_SUBTYPE_MC68040 ((cpu_subtype_t)2)
#define MACHO_CPU_SUBTYPE_MC68030_ONLY ((cpu_subtype_t)3)

/*
 *  I386 subtypes
 */

#define MACHO_CPU_SUBTYPE_INTEL(f, m) ((cpu_subtype_t)(f) + ((m) << 4))

#define MACHO_CPU_SUBTYPE_I386_ALL MACHO_CPU_SUBTYPE_INTEL(3, 0)
#define MACHO_CPU_SUBTYPE_386 MACHO_CPU_SUBTYPE_INTEL(3, 0)
#define MACHO_CPU_SUBTYPE_486 MACHO_CPU_SUBTYPE_INTEL(4, 0)
#define MACHO_CPU_SUBTYPE_486SX MACHO_CPU_SUBTYPE_INTEL(4, 8) // 8 << 4 = 128
#define MACHO_CPU_SUBTYPE_586 MACHO_CPU_SUBTYPE_INTEL(5, 0)
#define MACHO_CPU_SUBTYPE_PENT MACHO_CPU_SUBTYPE_INTEL(5, 0)
#define MACHO_CPU_SUBTYPE_PENTPRO MACHO_CPU_SUBTYPE_INTEL(6, 1)
#define MACHO_CPU_SUBTYPE_PENTII_M3 MACHO_CPU_SUBTYPE_INTEL(6, 3)
#define MACHO_CPU_SUBTYPE_PENTII_M5 MACHO_CPU_SUBTYPE_INTEL(6, 5)
#define MACHO_CPU_SUBTYPE_CELERON MACHO_CPU_SUBTYPE_INTEL(7, 6)
#define MACHO_CPU_SUBTYPE_CELERON_MOBILE MACHO_CPU_SUBTYPE_INTEL(7, 7)
#define MACHO_CPU_SUBTYPE_PENTIUM_3 MACHO_CPU_SUBTYPE_INTEL(8, 0)
#define MACHO_CPU_SUBTYPE_PENTIUM_3_M MACHO_CPU_SUBTYPE_INTEL(8, 1)
#define MACHO_CPU_SUBTYPE_PENTIUM_3_XEON MACHO_CPU_SUBTYPE_INTEL(8, 2)
#define MACHO_CPU_SUBTYPE_PENTIUM_M MACHO_CPU_SUBTYPE_INTEL(9, 0)
#define MACHO_CPU_SUBTYPE_PENTIUM_4 MACHO_CPU_SUBTYPE_INTEL(10, 0)
#define MACHO_CPU_SUBTYPE_PENTIUM_4_M MACHO_CPU_SUBTYPE_INTEL(10, 1)
#define MACHO_CPU_SUBTYPE_ITANIUM MACHO_CPU_SUBTYPE_INTEL(11, 0)
#define MACHO_CPU_SUBTYPE_ITANIUM_2 MACHO_CPU_SUBTYPE_INTEL(11, 1)
#define MACHO_CPU_SUBTYPE_XEON MACHO_CPU_SUBTYPE_INTEL(12, 0)
#define MACHO_CPU_SUBTYPE_XEON_MP MACHO_CPU_SUBTYPE_INTEL(12, 1)

#define MACHO_CPU_SUBTYPE_INTEL_FAMILY(x) ((x)&15)
#define MACHO_CPU_SUBTYPE_INTEL_FAMILY_MAX 15

#define MACHO_CPU_SUBTYPE_INTEL_MODEL(x) ((x) >> 4)
#define MACHO_CPU_SUBTYPE_INTEL_MODEL_ALL 0

/*
 *  X86 subtypes.
 */

#define MACHO_CPU_SUBTYPE_X86_ALL ((cpu_subtype_t)3)
#define MACHO_CPU_SUBTYPE_X86_64_ALL ((cpu_subtype_t)3)
#define MACHO_CPU_SUBTYPE_X86_ARCH1 ((cpu_subtype_t)4)

#define MACHO_CPU_THREADTYPE_INTEL_HTT ((cpu_threadtype_t)1)

/*
 *  Mips subtypes.
 */

#define MACHO_CPU_SUBTYPE_MIPS_ALL ((cpu_subtype_t)0)
#define MACHO_CPU_SUBTYPE_MIPS_R2300 ((cpu_subtype_t)1)
#define MACHO_CPU_SUBTYPE_MIPS_R2600 ((cpu_subtype_t)2)
#define MACHO_CPU_SUBTYPE_MIPS_R2800 ((cpu_subtype_t)3)
#define MACHO_CPU_SUBTYPE_MIPS_R2000a ((cpu_subtype_t)4) /* pmax */
#define MACHO_CPU_SUBTYPE_MIPS_R2000 ((cpu_subtype_t)5)
#define MACHO_CPU_SUBTYPE_MIPS_R3000a ((cpu_subtype_t)6) /* 3max */
#define MACHO_CPU_SUBTYPE_MIPS_R3000 ((cpu_subtype_t)7)

/*
 *  MC98000 (PowerPC) subtypes
 */
#define MACHO_CPU_SUBTYPE_MC98000_ALL ((cpu_subtype_t)0)
#define MACHO_CPU_SUBTYPE_MC98601 ((cpu_subtype_t)1)

/*
 *  HPPA subtypes for Hewlett-Packard HP-PA family of
 *  risc processors. Port by NeXT to 700 series.
 */

#define MACHO_CPU_SUBTYPE_HPPA_ALL ((cpu_subtype_t)0)
#define MACHO_CPU_SUBTYPE_HPPA_7100 ((cpu_subtype_t)0) /* compat */
#define MACHO_CPU_SUBTYPE_HPPA_7100LC ((cpu_subtype_t)1)

/*
 *  MC88000 subtypes.
 */
#define MACHO_CPU_SUBTYPE_MC88000_ALL ((cpu_subtype_t)0)
#define MACHO_CPU_SUBTYPE_MC88100 ((cpu_subtype_t)1)
#define MACHO_CPU_SUBTYPE_MC88110 ((cpu_subtype_t)2)

/*
 *  SPARC subtypes
 */
#define MACHO_CPU_SUBTYPE_SPARC_ALL ((cpu_subtype_t)0)

/*
 *  I860 subtypes
 */
#define MACHO_CPU_SUBTYPE_I860_ALL ((cpu_subtype_t)0)
#define MACHO_CPU_SUBTYPE_I860_860 ((cpu_subtype_t)1)

/*
 *  PowerPC subtypes
 */
#define MACHO_CPU_SUBTYPE_POWERPC_ALL ((cpu_subtype_t)0)
#define MACHO_CPU_SUBTYPE_POWERPC_601 ((cpu_subtype_t)1)
#define MACHO_CPU_SUBTYPE_POWERPC_602 ((cpu_subtype_t)2)
#define MACHO_CPU_SUBTYPE_POWERPC_603 ((cpu_subtype_t)3)
#define MACHO_CPU_SUBTYPE_POWERPC_603e ((cpu_subtype_t)4)
#define MACHO_CPU_SUBTYPE_POWERPC_603ev ((cpu_subtype_t)5)
#define MACHO_CPU_SUBTYPE_POWERPC_604 ((cpu_subtype_t)6)
#define MACHO_CPU_SUBTYPE_POWERPC_604e ((cpu_subtype_t)7)
#define MACHO_CPU_SUBTYPE_POWERPC_620 ((cpu_subtype_t)8)
#define MACHO_CPU_SUBTYPE_POWERPC_750 ((cpu_subtype_t)9)
#define MACHO_CPU_SUBTYPE_POWERPC_7400 ((cpu_subtype_t)10)
#define MACHO_CPU_SUBTYPE_POWERPC_7450 ((cpu_subtype_t)11)
#define MACHO_CPU_SUBTYPE_POWERPC_970 ((cpu_subtype_t)100)

/*
 *  ARM subtypes
 */
#define MACHO_CPU_SUBTYPE_ARM_ALL ((cpu_subtype_t)0)
#define MACHO_CPU_SUBTYPE_ARM_V4T ((cpu_subtype_t)5)
#define MACHO_CPU_SUBTYPE_ARM_V6 ((cpu_subtype_t)6)
#define MACHO_CPU_SUBTYPE_ARM_V5TEJ ((cpu_subtype_t)7)
#define MACHO_CPU_SUBTYPE_ARM_XSCALE ((cpu_subtype_t)8)
#define MACHO_CPU_SUBTYPE_ARM_V7 ((cpu_subtype_t)9)

typedef int vm_prot_t;
typedef int cpu_type_t;
typedef int cpu_subtype_t;
typedef int cpu_threadtype_t;

#define MACHO_FAT_MAGIC 0xcafebabe
#define MACHO_FAT_CIGAM 0xbebafeca

#define MACHO_MH_MAGIC 0xfeedface
#define MACHO_MH_CIGAM 0xcefaedfe
#define MACHO_MH_MAGIC_64 0xfeedfacf /* the 64-bit mach magic number */
#define MACHO_MH_CIGAM_64 0xcffaedfe /* NXSwapInt(MACHO_MH_MAGIC_64) */

typedef struct macho_fat_header {
  uint32 magic;
  uint32 nfat_arch;
} macho_fat_header;

typedef struct macho_fat_arch {
  cpu_type_t cputype;
  cpu_subtype_t cpusubtype;
  uint32 offset;
  uint32 size;
  uint32 align;
} macho_fat_arch;

typedef struct macho_mach_header_magic {
  uint32 magic;
} macho_mach_header_magic;

typedef struct macho_mach_header {
  uint32 magic;
  cpu_type_t cputype;
  cpu_subtype_t cpusubtype;
  uint32 filetype;
  uint32 ncmds;
  uint32 sizeofcmds;
  uint32 flags;
} macho_mach_header;

typedef struct macho_mach_header_64 {
  uint32 magic;             /* mach magic number identifier */
  cpu_type_t cputype;       /* cpu specifier */
  cpu_subtype_t cpusubtype; /* machine specifier */
  uint32 filetype;          /* type of file */
  uint32 ncmds;             /* number of load commands */
  uint32 sizeofcmds;        /* the size of all the load commands */
  uint32 flags;             /* flags */
  uint32 reserved;          /* reserved */
} macho_mach_header_64;

typedef struct macho_load_command {
  uint32 cmd;
  uint32 cmdsize;
} macho_load_command;

typedef struct macho_segment_command {
  uint32 cmd;
  uint32 cmdsize;
  char segname[16];
  uint32 vmaddr;
  uint32 vmsize;
  uint32 fileoff;
  uint32 filesize;
  vm_prot_t maxprot;
  vm_prot_t initprot;
  uint32 nsects;
  uint32 flags;
} macho_segment_command;

typedef struct macho_segment_command_64 { /* for 64-bit architectures */
  uint32 cmd;                             /* LC_SEGMENT_64 */
  uint32 cmdsize;                         /* includes sizeof macho_section_64 structs */
  char segname[16];                       /* segment name */
  uint64 vmaddr;                          /* memory address of this segment */
  uint64 vmsize;                          /* memory size of this segment */
  uint64 fileoff;                         /* file offset of this segment */
  uint64 filesize;                        /* amount to map from the file */
  vm_prot_t maxprot;                      /* maximum VM protection */
  vm_prot_t initprot;                     /* initial VM protection */
  uint32 nsects;                          /* number of sections in segment */
  uint32 flags;                           /* flags */
} macho_segment_command_64;

typedef struct macho_section {
  char sectname[16];
  char segname[16];
  uint32 addr;
  uint32 size;
  uint32 offset;
  uint32 align;
  uint32 reloff;
  uint32 nreloc;
  uint32 flags;
  uint32 reserved1;
  uint32 reserved2;
} macho_section;

typedef struct macho_section_64 { /* for 64-bit architectures */
  char sectname[16];              /* name of this macho_section */
  char segname[16];               /* segment this macho_section goes in */
  uint64 addr;                    /* memory address of this macho_section */
  uint64 size;                    /* size in bytes of this macho_section */
  uint32 offset;                  /* file offset of this macho_section */
  uint32 align;                   /* macho_section alignment (power of 2) */
  uint32 reloff;                  /* file offset of relocation entries */
  uint32 nreloc;                  /* number of relocation entries */
  uint32 flags;                   /* flags (macho_section type and attributes)*/
  uint32 reserved1;               /* reserved (for offset or index) */
  uint32 reserved2;               /* reserved (for count or sizeof) */
  uint32 reserved3;               /* reserved */
} macho_section_64;

union lc_str {
  uint32 offset;
#ifndef __LP64__
  char* ptr;
#endif
};

typedef struct macho_symtab_command {
  uint32 cmd;
  uint32 cmdsize;
  uint32 symoff;
  uint32 nsyms;
  uint32 stroff;
  uint32 strsize;
} macho_symtab_command;

typedef struct macho_dylib {
  union lc_str name;
  uint32 timestamp;
  uint32 current_version;
  uint32 compatibility_version;
} macho_dylib;

typedef struct macho_dylib_command {
  uint32 cmd;
  uint32 cmdsize;
  macho_dylib dylib;
} macho_dylib_command;

typedef struct macho_dylinker_command {
  uint32 cmd;
  uint32 cmdsize;
  union lc_str name;
} macho_dylinker_command;

typedef struct macho_dysymtab_command {
  uint32 cmd;
  uint32 cmdsize;
  uint32 ilocalsym;
  uint32 nlocalsym;
  uint32 iextdefsym;
  uint32 nextdefsym;
  uint32 iundefsym;
  uint32 nundefsym;
  uint32 tocoff;
  uint32 ntoc;
  uint32 modtaboff;
  uint32 nmodtab;
  uint32 extrefsymoff;
  uint32 nextrefsyms;
  uint32 indirectsymoff;
  uint32 nindirectsyms;
  uint32 extreloff;
  uint32 nextrel;
  uint32 locreloff;
  uint32 nlocrel;
} macho_dysymtab_command;

typedef struct macho_dylib_table_of_contents {
  uint32 symbol_index;
  uint32 module_index;
} macho_dylib_table_of_contents;

typedef struct macho_dylib_module {
  uint32 module_name;
  uint32 iextdefsym;
  uint32 nextdefsym;
  uint32 irefsym;
  uint32 nrefsym;
  uint32 ilocalsym;
  uint32 nlocalsym;
  uint32 iextrel;
  uint32 nextrel;
  uint32 iinit_iterm;
  uint32 ninit_nterm;
  uint32 objc_module_info_addr;
  uint32 objc_module_info_size;
} macho_dylib_module;

typedef struct macho_twolevel_hints_command {
  uint32 cmd;
  uint32 cmdsize;
  uint32 offset;
  uint32 nhints;
} macho_twolevel_hints_command;

typedef struct macho_nlist {
  uint32 n_strx;
  uint8 n_type;
  uint8 n_sect;
  uint16 n_desc;
  uint64 n_value;
} macho_nlist;

/*
** The n_type field really contains four fields:
** unsigned char [N_STAB:3 | N_PEXT:1 | N_TYPE:3 | N_EXT:1]
** which are used via the following masks
*/
#define MACHO_N_STAB 0xe0 /* if any of these bits set, a symbolic debugging entry */
#define MACHO_N_PEXT 0x10 /* private external symbol bit */
#define MACHO_N_TYPE 0x0e /* mask for the type bits */
#define MACHO_N_EXT 0x01  /* external symbol bit, set for external symbols */

/* Values for N_TYPE bits of the n_type field */
#define MACHO_N_UNDF 0x0 /* undefined, n_sect == NO_SECT */
#define MACHO_N_ABS 0x2  /* absolute, n_sect == NO_SECT */
#define MACHO_N_SECT 0xe /* defined in section number n_sect */
#define MACHO_N_PBUD 0xc /* prebound undefined (defined in a dylib) */
#define MACHO_N_INDR 0xa /* indirect */

#endif
