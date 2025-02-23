/**
 * @defgroup   elf
 * @brief      ELF module.
 * @{
 */
#ifndef _ELF_H
#define _ELF_H 1

#include "uint8.h"
#include "uint16.h"
#include "uint32.h"
#include "uint64.h"
#include "range.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Type for version symbol information.  */
typedef uint16 elf32_versym;
typedef uint16 elf64_versym;

/* The ELF file header.  This appears at the start of every ELF file.  */

#define ELF_EI_NIDENT (16)

typedef struct __unaligned {
  uint8 e_ident[ELF_EI_NIDENT]; /* Magic number and other info */
  uint16 e_type;                /* Object file type */
  uint16 e_machine;             /* Architecture */
  uint32 e_version;             /* Object file version */
  uint32 e_entry;               /* Entry point virtual address */
  uint32 e_phoff;               /* Program header table file offset */
  uint32 e_shoff;               /* Section header table file offset */
  uint32 e_flags;               /* Processor-specific flags */
  uint16 e_ehsize;              /* ELF header size in bytes */
  uint16 e_phentsize;           /* Program header table entry size */
  uint16 e_phnum;               /* Program header table entry count */
  uint16 e_shentsize;           /* Section header table entry size */
  uint16 e_shnum;               /* Section header table entry count */
  uint16 e_shstrndx;            /* Section header string table index */
}
elf32_ehdr;

typedef struct __unaligned {
  uint8 e_ident[ELF_EI_NIDENT]; /* Magic number and other info */
  uint16 e_type;                /* Object file type */
  uint16 e_machine;             /* Architecture */
  uint32 e_version;             /* Object file version */
  uint64 e_entry;               /* Entry point virtual address */
  uint64 e_phoff;               /* Program header table file offset */
  uint64 e_shoff;               /* Section header table file offset */
  uint32 e_flags;               /* Processor-specific flags */
  uint16 e_ehsize;              /* ELF header size in bytes */
  uint16 e_phentsize;           /* Program header table entry size */
  uint16 e_phnum;               /* Program header table entry count */
  uint16 e_shentsize;           /* Section header table entry size */
  uint16 e_shnum;               /* Section header table entry count */
  uint16 e_shstrndx;            /* Section header string table index */
}
elf64_ehdr;

/* Fields in the e_ident array.  The EI_* macros are indices into the
   array.  The macros under each EI_* macro are the values the byte
   may have.  */

#define ELF_EI_MAG0 0    /* File identification byte 0 index */
#define ELF_ELFMAG0 0x7f /* Magic number byte 0 */

#define ELF_EI_MAG1 1   /* File identification byte 1 index */
#define ELF_ELFMAG1 'E' /* Magic number byte 1 */

#define ELF_EI_MAG2 2   /* File identification byte 2 index */
#define ELF_ELFMAG2 'L' /* Magic number byte 2 */

#define ELF_EI_MAG3 3   /* File identification byte 3 index */
#define ELF_ELFMAG3 'F' /* Magic number byte 3 */

/* Conglomeration of the identification bytes, for easy testing as a word.
 */
#define ELF_ELFMAG "\177ELF"
#define ELF_SELFMAG 4

#define ELF_EI_CLASS 4     /* File class byte index */
#define ELF_ELFCLASSNONE 0 /* Invalid class */
#define ELF_ELFCLASS32 1   /* 32-bit objects */
#define ELF_ELFCLASS64 2   /* 64-bit objects */
#define ELF_ELFCLASSNUM 3

#define ELF_EI_DATA 5     /* Data encoding byte index */
#define ELF_ELFDATANONE 0 /* Invalid data encoding */
#define ELF_ELFDATA2LSB 1 /* 2's complement, little endian */
#define ELF_ELFDATA2MSB 2 /* 2's complement, big endian */
#define ELF_ELFDATANUM 3

#define ELF_EI_VERSION 6 /* File version byte index */
                         /* Value must be ELF_EV_CURRENT */

#define ELF_EI_OSABI 7          /* OS ABI identification */
#define ELF_ELFOSABI_NONE 0     /* UNIX System V ABI */
#define ELF_ELFOSABI_SYSV 0     /* Alias. */
#define ELF_ELFOSABI_HPUX 1     /* HP-UX */
#define ELF_ELFOSABI_NETBSD 2   /* NetBSD. */
#define ELF_ELFOSABI_LINUX 3    /* Linux. */
#define ELF_ELFOSABI_SOLARIS 6  /* Sun Solaris. */
#define ELF_ELFOSABI_AIX 7      /* IBM AIX. */
#define ELF_ELFOSABI_IRIX 8     /* SGI Irix. */
#define ELF_ELFOSABI_FREEBSD 9  /* FreeBSD. */
#define ELF_ELFOSABI_TRU64 10   /* Compaq TRU64 UNIX. */
#define ELF_ELFOSABI_MODESTO 11 /* Novell Modesto. */
#define ELF_ELFOSABI_OPENBSD 12 /* OpenBSD. */
#define ELF_ELFOSABI_ARM 97     /* ARM */
#define ELF_ELFOSABI_STANDALONE \
  255 /* Standalone (embedded) application \
       */

#define ELF_EI_ABIVERSION 8 /* ABI version */

#define ELF_EI_PAD 9 /* Byte index of padding bytes */

/* Legal values for e_type (object file type).  */

#define ELF_ET_NONE 0        /* No file type */
#define ELF_ET_REL 1         /* Relocatable file */
#define ELF_ET_EXEC 2        /* Executable file */
#define ELF_ET_DYN 3         /* Shared object file */
#define ELF_ET_CORE 4        /* Core file */
#define ELF_ET_NUM 5         /* Number of defined types */
#define ELF_ET_LOOS 0xfe00   /* OS-specific range start */
#define ELF_ET_HIOS 0xfeff   /* OS-specific range end */
#define ELF_ET_LOPROC 0xff00 /* Processor-specific range start */
#define ELF_ET_HIPROC 0xffff /* Processor-specific range end */

/* Legal values for e_machine (architecture).  */

#define ELF_EM_NONE 0  /* No machine */
#define ELF_EM_M32 1   /* AT&T WE 32100 T WE 32100 */
#define ELF_EM_SPARC 2 /* SUN SPARC */
#define ELF_EM_386 3   /* Intel 80386 */
#define ELF_EM_68K 4   /* Motorola m68k family */
#define ELF_EM_88K 5   /* Motorola m88k family */
#define ELF_EM_860 7   /* Intel 80860 */
#define ELF_EM_MIPS 8  /* MIPS R3000 big-endian */
#define ELF_EM_S370 9  /* IBM System/370 */

#define ELF_EM_PARISC 15      /* HPPA */
#define ELF_EM_VPP500 17      /* Fujitsu VPP500 */
#define ELF_EM_SPARC32PLUS 18 /* Sun's "v8plus" */
#define ELF_EM_960 19         /* Intel 80960 */
#define ELF_EM_PPC 20         /* PowerPC */
#define ELF_EM_PPC64 21       /* PowerPC 64-bit */
#define ELF_EM_S390 22        /* IBM S390 */

#define ELF_EM_V800 36       /* NEC V800 series */
#define ELF_EM_FR20 37       /* Fujitsu FR20 */
#define ELF_EM_RH32 38       /* TRW RH-32 */
#define ELF_EM_RCE 39        /* Motorola RCE */
#define ELF_EM_ARM 40        /* ARM */
#define ELF_EM_FAKE_ALPHA 41 /* Digital Alpha */
#define ELF_EM_SH 42         /* Hitachi SH */
#define ELF_EM_SPARCV9 43    /* SPARC v9 64-bit */
#define ELF_EM_TRICORE 44    /* Siemens Tricore */
#define ELF_EM_ARC 45        /* Argonaut RISC Core */
#define ELF_EM_H8_300 46     /* Hitachi H8/300 */
#define ELF_EM_H8_300H 47    /* Hitachi H8/300H */
#define ELF_EM_H8S 48        /* Hitachi H8S */
#define ELF_EM_H8_500 49     /* Hitachi H8/500 */
#define ELF_EM_IA_64 50      /* Intel Merced */
#define ELF_EM_COLDFIRE 52   /* Motorola Coldfire */
#define ELF_EM_68HC12 53     /* Motorola M68HC12 */
#define ELF_EM_MMA 54        /* Fujitsu MMA Multimedia Accelerator*/
#define ELF_EM_PCP 55        /* Siemens PCP */
#define ELF_EM_NCPU 56       /* Sony nCPU embeeded RISC */
#define ELF_EM_NDR1 57       /* Denso NDR1 microprocessor */
#define ELF_EM_STARCORE 58   /* Motorola Start*Core processor */
#define ELF_EM_ME16 59       /* Toyota ME16 processor */
#define ELF_EM_ST100 60      /* STMicroelectronic ST100 processor */
#define ELF_EM_TINYJ 61      /* Advanced Logic Corp. Tinyj emb.fam*/
#define ELF_EM_X86_64 62     /* AMD x86-64 architecture */
#define ELF_EM_PDSP 63       /* Sony DSP Processor */

#define ELF_EM_FX66 66    /* Siemens FX66 microcontroller */
#define ELF_EM_ST9PLUS 67 /* STMicroelectronics ST9+ 8/16 mc */
#define ELF_EM_ST7 68     /* STmicroelectronics ST7 8 bit mc */
#define ELF_EM_68HC16 69  /* Motorola MC68HC16 microcontroller */
#define ELF_EM_68HC11 70  /* Motorola MC68HC11 microcontroller */
#define ELF_EM_68HC08 71  /* Motorola MC68HC08 microcontroller */
#define ELF_EM_68HC05 72  /* Motorola MC68HC05 microcontroller */
#define ELF_EM_SVX 73     /* Silicon Graphics SVx */
#define ELF_EM_ST19 74    /* STMicroelectronics ST19 8 bit mc */
#define ELF_EM_VAX 75     /* Digital VAX */
#define ELF_EM_CRIS \
  76 /* Axis Communications 32-bit embedded processor \
      */
#define ELF_EM_JAVELIN \
  77                       /* Infineon Technologies 32-bit embedded processor \
                            */
#define ELF_EM_FIREPATH 78 /* Element 14 64-bit DSP Processor */
#define ELF_EM_ZSP 79      /* LSI Logic 16-bit DSP Processor */
#define ELF_EM_MMIX 80     /* Donald Knuth's educational 64-bit processor */
#define ELF_EM_HUANY 81    /* Harvard University machine-independent object files */
#define ELF_EM_PRISM 82    /* SiTera Prism */
#define ELF_EM_AVR 83      /* Atmel AVR 8-bit microcontroller */
#define ELF_EM_FR30 84     /* Fujitsu FR30 */
#define ELF_EM_D10V 85     /* Mitsubishi D10V */
#define ELF_EM_D30V 86     /* Mitsubishi D30V */
#define ELF_EM_V850 87     /* NEC v850 */
#define ELF_EM_M32R 88     /* Mitsubishi M32R */
#define ELF_EM_MN10300 89  /* Matsushita MN10300 */
#define ELF_EM_MN10200 90  /* Matsushita MN10200 */
#define ELF_EM_PJ 91       /* picoJava */
#define ELF_EM_OPENRISC 92 /* OpenRISC 32-bit embedded processor */
#define ELF_EM_ARC_A5 93   /* ARC Cores Tangent-A5 */
#define ELF_EM_XTENSA 94   /* Tensilica Xtensa Architecture */
#define ELF_EM_NUM 95

/* If it is necessary to assign new unofficial EM_* values, please
   pick large random numbers (0x8523, 0xa7f2, etc.) to minimize the
   chances of collision with official or non-GNU unofficial values.  */

#define ELF_EM_ALPHA 0x9026

/* Legal values for e_version (version).  */

#define ELF_EV_NONE 0    /* Invalid ELF version */
#define ELF_EV_CURRENT 1 /* Current version */
#define ELF_EV_NUM 2

/* Section header.  */

typedef struct {
  uint32 sh_name;      /* Section name (string tbl index) */
  uint32 sh_type;      /* Section type */
  uint32 sh_flags;     /* Section flags */
  uint32 sh_addr;      /* Section virtual addr at execution */
  uint32 sh_offset;    /* Section file offset */
  uint32 sh_size;      /* Section size in bytes */
  uint32 sh_link;      /* Link to another section */
  uint32 sh_info;      /* Additional section information */
  uint32 sh_addralign; /* Section alignment */
  uint32 sh_entsize;   /* Entry size if section holds table */
} elf32_shdr;

typedef struct {
  uint32 sh_name;      /* Section name (string tbl index) */
  uint32 sh_type;      /* Section type */
  uint64 sh_flags;     /* Section flags */
  uint64 sh_addr;      /* Section virtual addr at execution */
  uint64 sh_offset;    /* Section file offset */
  uint64 sh_size;      /* Section size in bytes */
  uint32 sh_link;      /* Link to another section */
  uint32 sh_info;      /* Additional section information */
  uint64 sh_addralign; /* Section alignment */
  uint64 sh_entsize;   /* Entry size if section holds table */
} elf64_shdr;

/* Special section indices.  */

#define ELF_SHN_UNDEF 0          /* Undefined section */
#define ELF_SHN_LORESERVE 0xff00 /* Start of reserved indices */
#define ELF_SHN_LOPROC 0xff00    /* Start of processor-specific */
#define ELF_SHN_HIPROC 0xff1f    /* End of processor-specific */
#define ELF_SHN_LOOS 0xff20      /* Start of OS-specific */
#define ELF_SHN_HIOS 0xff3f      /* End of OS-specific */
#define ELF_SHN_ABS 0xfff1       /* Associated symbol is absolute */
#define ELF_SHN_COMMON 0xfff2    /* Associated symbol is common */
#define ELF_SHN_XINDEX 0xffff    /* Index is in extra table. */
#define ELF_SHN_HIRESERVE 0xffff /* End of reserved indices */

/* Legal values for sh_type (section type).  */

#define ELF_SHT_NULL 0                 /* Section header table entry unused */
#define ELF_SHT_PROGBITS 1             /* Program data */
#define ELF_SHT_SYMTAB 2               /* Symbol table */
#define ELF_SHT_STRTAB 3               /* String table */
#define ELF_SHT_RELA 4                 /* Relocation entries with addends */
#define ELF_SHT_HASH 5                 /* Symbol hash table */
#define ELF_SHT_DYNAMIC 6              /* Dynamic linking information */
#define ELF_SHT_NOTE 7                 /* Notes */
#define ELF_SHT_NOBITS 8               /* Program space with no data (bss) */
#define ELF_SHT_REL 9                  /* Relocation entries, no addends */
#define ELF_SHT_SHLIB 10               /* Reserved */
#define ELF_SHT_DYNSYM 11              /* Dynamic linker symbol table */
#define ELF_SHT_INIT_ARRAY 14          /* Array of constructors */
#define ELF_SHT_FINI_ARRAY 15          /* Array of destructors */
#define ELF_SHT_PREINIT_ARRAY 16       /* Array of pre-constructors */
#define ELF_SHT_GROUP 17               /* Section group */
#define ELF_SHT_SYMTAB_SHNDX 18        /* Extended section indeces */
#define ELF_SHT_NUM 19                 /* Number of defined types. */
#define ELF_SHT_LOOS 0x60000000        /* Start OS-specific */
#define ELF_SHT_GNU_LIBLIST 0x6ffffff7 /* Prelink library list */
#define ELF_SHT_CHECKSUM 0x6ffffff8    /* Checksum for DSO content. */
#define ELF_SHT_LOSUNW 0x6ffffffa      /* Sun-specific low bound. */
#define ELF_SHT_SUNW_move 0x6ffffffa
#define ELF_SHT_SUNW_COMDAT 0x6ffffffb
#define ELF_SHT_SUNW_syminfo 0x6ffffffc
#define ELF_SHT_GNU_verdef 0x6ffffffd  /* Version definition section.  */
#define ELF_SHT_GNU_verneed 0x6ffffffe /* Version needs section.  */
#define ELF_SHT_GNU_versym 0x6fffffff  /* Version symbol table.  */
#define ELF_SHT_HISUNW 0x6fffffff      /* Sun-specific high bound. */
#define ELF_SHT_HIOS 0x6fffffff        /* End OS-specific type */
#define ELF_SHT_LOPROC 0x70000000      /* Start of processor-specific */
#define ELF_SHT_HIPROC 0x7fffffff      /* End of processor-specific */
#define ELF_SHT_LOUSER 0x80000000      /* Start of application-specific */
#define ELF_SHT_HIUSER 0x8fffffff      /* End of application-specific */

/* Legal values for sh_flags (section flags).  */

#define ELF_SHF_WRITE (1 << 0)            /* Writable */
#define ELF_SHF_ALLOC (1 << 1)            /* Occupies memory during execution */
#define ELF_SHF_EXECINSTR (1 << 2)        /* Executable */
#define ELF_SHF_MERGE (1 << 4)            /* Might be merged */
#define ELF_SHF_STRINGS (1 << 5)          /* Contains nul-terminated strings */
#define ELF_SHF_INFO_LINK (1 << 6)        /* `sh_info' contains SHT index */
#define ELF_SHF_LINK_ORDER (1 << 7)       /* Preserve order after combining */
#define ELF_SHF_OS_NONCONFORMING (1 << 8) /* Non-standard OS specific handling required */
#define ELF_SHF_GROUP (1 << 9)            /* Section is member of a group. */
#define ELF_SHF_TLS (1 << 10)             /* Section hold thread-local data. */
#define ELF_SHF_MASKOS 0x0ff00000         /* OS-specific. */
#define ELF_SHF_MASKPROC 0xf0000000       /* Processor-specific */

/* Section group handling.  */
#define ELF_GRP_COMDAT 0x1 /* Mark group as COMDAT. */

/* Symbol table entry.  */

typedef struct __unaligned {
  uint32 st_name;  /* Symbol name (string tbl index) */
  uint32 st_value; /* Symbol value */
  uint32 st_size;  /* Symbol size */
  uint8 st_info;   /* Symbol type and binding */
  uint8 st_other;  /* Symbol visibility */
  uint16 st_shndx; /* Section index */
}
elf32_sym;

typedef struct __unaligned {
  uint32 st_name;  /* Symbol name (string tbl index) */
  uint8 st_info;   /* Symbol type and binding */
  uint8 st_other;  /* Symbol visibility */
  uint16 st_shndx; /* Section index */
  uint64 st_value; /* Symbol value */
  uint64 st_size;  /* Symbol size */
}
elf64_sym;

/* The syminfo section if available contains additional information about
   every dynamic symbol.  */

typedef struct __unaligned {
  uint16 si_boundto; /* Direct bindings, symbol bound to */
  uint16 si_flags;   /* Per symbol flags */
}
elf32_syminfo;

typedef struct __unaligned {
  uint16 si_boundto; /* Direct bindings, symbol bound to */
  uint16 si_flags;   /* Per symbol flags */
}
elf64_syminfo;

/* Possible values for si_boundto.  */
#define ELF_SYMINFO_BT_SELF 0xffff       /* Symbol bound to self */
#define ELF_SYMINFO_BT_PARENT 0xfffe     /* Symbol bound to parent */
#define ELF_SYMINFO_BT_LOWRESERVE 0xff00 /* Beginning of reserved entries */

/* Possible bitmasks for si_flags.  */
#define ELF_SYMINFO_FLG_DIRECT 0x0001   /* Direct bound symbol */
#define ELF_SYMINFO_FLG_PASSTHRU 0x0002 /* Pass-thru symbol for translator */
#define ELF_SYMINFO_FLG_COPY 0x0004     /* Symbol is a copy-reloc */
#define ELF_SYMINFO_FLG_LAZYLOAD 0x0008 /* Symbol bound to object to be lazy loaded */
/* Syminfo version values.  */
#define ELF_SYMINFO_NONE 0
#define ELF_SYMINFO_CURRENT 1
#define ELF_SYMINFO_NUM 2

/* How to extract and insert information held in the st_info field.  */

#define ELF_ELF32_ST_BIND(val) (((uint8)(val)) >> 4)
#define ELF_ELF32_ST_TYPE(val) ((val)&0xf)
#define ELF_ELF32_ST_INFO(bind, type) (((bind) << 4) + ((type)&0xf))

/* Both elf32_sym and elf64_sym use the same one-byte st_info field.  */
#define ELF_ELF64_ST_BIND(val) ELF_ELF32_ST_BIND(val)
#define ELF_ELF64_ST_TYPE(val) ELF_ELF32_ST_TYPE(val)
#define ELF_ELF64_ST_INFO(bind, type) ELF_ELF32_ST_INFO((bind), (type))

/* Legal values for ST_BIND subfield of st_info (symbol binding).  */

#define ELF_STB_LOCAL 0   /* Local symbol */
#define ELF_STB_GLOBAL 1  /* Global symbol */
#define ELF_STB_WEAK 2    /* Weak symbol */
#define ELF_STB_NUM 3     /* Number of defined types. */
#define ELF_STB_LOOS 10   /* Start of OS-specific */
#define ELF_STB_HIOS 12   /* End of OS-specific */
#define ELF_STB_LOPROC 13 /* Start of processor-specific */
#define ELF_STB_HIPROC 15 /* End of processor-specific */

/* Legal values for ST_TYPE subfield of st_info (symbol type).  */

#define ELF_STT_NOTYPE 0  /* Symbol type is unspecified */
#define ELF_STT_OBJECT 1  /* Symbol is a data object */
#define ELF_STT_FUNC 2    /* Symbol is a code object */
#define ELF_STT_SECTION 3 /* Symbol associated with a section */
#define ELF_STT_FILE 4    /* Symbol's name is file name */
#define ELF_STT_COMMON 5  /* Symbol is a common data object */
#define ELF_STT_TLS 6     /* Symbol is thread-local data object*/
#define ELF_STT_NUM 7     /* Number of defined types. */
#define ELF_STT_LOOS 10   /* Start of OS-specific */
#define ELF_STT_HIOS 12   /* End of OS-specific */
#define ELF_STT_LOPROC 13 /* Start of processor-specific */
#define ELF_STT_HIPROC 15 /* End of processor-specific */

/* Symbol table indices are found in the hash buckets and chain table
   of a symbol hash table section.  This special index value indicates
   the end of a chain, meaning no further symbols are found in that bucket.
 */

#define ELF_STN_UNDEF 0 /* End of a chain. */

/* How to extract and insert information held in the st_other field.  */

#define ELF_ELF32_ST_VISIBILITY(o) ((o)&0x03)

/* For ELF64 the definitions are the same.  */
#define ELF_ELF64_ST_VISIBILITY(o) ELF_ELF32_ST_VISIBILITY(o)

/* Symbol visibility specification encoded in the st_other field.  */
#define ELF_STV_DEFAULT 0   /* Default symbol visibility rules */
#define ELF_STV_INTERNAL 1  /* Processor specific hidden class */
#define ELF_STV_HIDDEN 2    /* Sym unavailable in other modules */
#define ELF_STV_PROTECTED 3 /* Not preemptible, not exported */

/* Relocation table entry without addend (in section of type ELF_SHT_REL).
 */

typedef struct {
  uint32 r_offset; /* Address */
  uint32 r_info;   /* Relocation type and symbol index */
} elf32_rel;

/* I have seen two different definitions of the elf64_rel and
   elf64_rela structures, so we'll leave them out until Novell (or
   whoever) gets their act together.  */
/* The following, at least, is used on Sparc v9, MIPS, and Alpha.  */

typedef struct {
  uint64 r_offset; /* Address */
  uint64 r_info;   /* Relocation type and symbol index */
} elf64_rel;

/* Relocation table entry with addend (in section of type ELF_SHT_RELA). */

typedef struct {
  uint32 r_offset; /* Address */
  uint32 r_info;   /* Relocation type and symbol index */
  int32 r_addend;  /* Addend */
} elf32_rela;

typedef struct {
  uint64 r_offset; /* Address */
  uint64 r_info;   /* Relocation type and symbol index */
  int64 r_addend;  /* Addend */
} elf64_rela;

/* How to extract and insert information held in the r_info field.  */

#define ELF_ELF32_R_SYM(val) ((val) >> 8)
#define ELF_ELF32_R_TYPE(val) ((val)&0xff)
#define ELF_ELF32_R_INFO(sym, type) (((sym) << 8) + ((type)&0xff))

#define ELF_ELF64_R_SYM(i) ((i) >> 32)
#define ELF_ELF64_R_TYPE(i) ((i)&0xffffffff)
#define ELF_ELF64_R_INFO(sym, type) ((((uint64)(sym)) << 32) + (type))

/* Program segment header.  */

typedef struct {
  uint32 p_type;   /* Segment type */
  uint32 p_offset; /* Segment file offset */
  uint32 p_vaddr;  /* Segment virtual address */
  uint32 p_paddr;  /* Segment physical address */
  uint32 p_filesz; /* Segment size in file */
  uint32 p_memsz;  /* Segment size in memory */
  uint32 p_flags;  /* Segment flags */
  uint32 p_align;  /* Segment alignment */
} elf32_phdr;

typedef struct {
  uint32 p_type;   /* Segment type */
  uint32 p_flags;  /* Segment flags */
  uint64 p_offset; /* Segment file offset */
  uint64 p_vaddr;  /* Segment virtual address */
  uint64 p_paddr;  /* Segment physical address */
  uint64 p_filesz; /* Segment size in file */
  uint64 p_memsz;  /* Segment size in memory */
  uint64 p_align;  /* Segment alignment */
} elf64_phdr;

/* Legal values for p_type (segment type).  */

enum {
  ELF_PT_NULL = 0,                  /* Program header table entry unused */
  ELF_PT_LOAD = 1,                  /* Loadable program segment */
  ELF_PT_DYNAMIC = 2,               /* Dynamic linking information */
  ELF_PT_INTERP = 3,                /* Program interpreter */
  ELF_PT_NOTE = 4,                  /* Auxiliary information */
  ELF_PT_SHLIB = 5,                 /* Reserved */
  ELF_PT_PHDR = 6,                  /* Entry for header table itself */
  ELF_PT_TLS = 7,                   /* Thread-local storage segment */
  ELF_PT_NUM = 8,                   /* Number of defined types */
  ELF_PT_LOOS = 0x60000000,         /* Start of OS-specific */
  ELF_PT_GNU_EH_FRAME = 0x6474e550, /* GCC .eh_frame_hdr segment */
  ELF_PT_GNU_STACK = 0x6474e551,    /* Indicates stack executability */
  ELF_PT_LOSUNW = 0x6ffffffa,
  ELF_PT_SUNWBSS = 0x6ffffffa,   /* Sun Specific segment */
  ELF_PT_SUNWSTACK = 0x6ffffffb, /* Stack segment */
  ELF_PT_HISUNW = 0x6fffffff,
  ELF_PT_HIOS = 0x6fffffff,   /* End of OS-specific */
  ELF_PT_LOPROC = 0x70000000, /* Start of processor-specific */
  ELF_PT_HIPROC = 0x7fffffff  /* End of processor-specific */
};

#define ELF_PT_NULL ELF_PT_NULL
#define ELF_PT_LOAD ELF_PT_LOAD
#define ELF_PT_DYNAMIC ELF_PT_DYNAMIC
#define ELF_PT_INTERP ELF_PT_INTERP
#define ELF_PT_NOTE ELF_PT_NOTE
#define ELF_PT_SHLIB ELF_PT_SHLIB
#define ELF_PT_PHDR ELF_PT_PHDR
#define ELF_PT_TLS ELF_PT_TLS
#define ELF_PT_NUM ELF_PT_NUM
#define ELF_PT_LOOS ELF_PT_LOOS
#define ELF_PT_GNU_EH_FRAME ELF_PT_GNU_EH_FRAME
#define ELF_PT_GNU_STACK ELF_PT_GNU_STACK
#define ELF_PT_LOSUNW ELF_PT_LOSUNW
#define ELF_PT_SUNWBSS ELF_PT_SUNWBSS
#define ELF_PT_SUNWSTACK ELF_PT_SUNWSTACK
#define ELF_PT_HISUNW ELF_PT_HISUNW
#define ELF_PT_HIOS ELF_PT_HIOS
#define ELF_PT_LOPROC ELF_PT_LOPROC
#define ELF_PT_HIPROC ELF_PT_HIPROC

/* Legal values for p_flags (segment flags).  */

#define ELF_PF_X (1 << 0)          /* Segment is executable */
#define ELF_PF_W (1 << 1)          /* Segment is writable */
#define ELF_PF_R (1 << 2)          /* Segment is readable */
#define ELF_PF_MASKOS 0x0ff00000   /* OS-specific */
#define ELF_PF_MASKPROC 0xf0000000 /* Processor-specific */

/* Legal values for note segment descriptor types for core files. */

#define ELF_NT_PRSTATUS 1   /* Contains copy of prstatus struct */
#define ELF_NT_FPREGSET 2   /* Contains copy of fpregset struct */
#define ELF_NT_PRPSINFO 3   /* Contains copy of prpsinfo struct */
#define ELF_NT_PRXREG 4     /* Contains copy of prxregset struct */
#define ELF_NT_TASKSTRUCT 4 /* Contains copy of task structure */
#define ELF_NT_PLATFORM 5   /* String from sysinfo(SI_PLATFORM) */
#define ELF_NT_AUXV 6       /* Contains copy of auxv array */
#define ELF_NT_GWINDOWS 7   /* Contains copy of gwindows struct */
#define ELF_NT_ASRS 8       /* Contains copy of asrset struct */
#define ELF_NT_PSTATUS 10   /* Contains copy of pstatus struct */
#define ELF_NT_PSINFO 13    /* Contains copy of psinfo struct */
#define ELF_NT_PRCRED 14    /* Contains copy of prcred struct */
#define ELF_NT_UTSNAME 15   /* Contains copy of utsname struct */
#define ELF_NT_LWPSTATUS 16 /* Contains copy of lwpstatus struct */
#define ELF_NT_LWPSINFO 17  /* Contains copy of lwpinfo struct */
#define ELF_NT_PRFPXREG 20  /* Contains copy of fprxregset struct*/

/* Legal values for the note segment descriptor types for object files.  */

#define ELF_NT_VERSION 1 /* Contains a version string. */

/* Dynamic section entry.  */

typedef struct {
  int32 d_tag; /* Dynamic entry type */
  union {
    uint32 d_val; /* Integer value */
    uint32 d_ptr; /* Address value */
  } d_un;
} elf32_dyn;

typedef struct {
  int64 d_tag; /* Dynamic entry type */
  union {
    uint64 d_val; /* Integer value */
    uint64 d_ptr; /* Address value */
  } d_un;
} elf64_dyn;

/* Legal values for d_tag (dynamic entry type).  */

#define ELF_DT_NULL 0             /* Marks end of dynamic section */
#define ELF_DT_NEEDED 1           /* Name of needed library */
#define ELF_DT_PLTRELSZ 2         /* Size in bytes of PLT relocs */
#define ELF_DT_PLTGOT 3           /* Processor defined value */
#define ELF_DT_HASH 4             /* Address of symbol hash table */
#define ELF_DT_STRTAB 5           /* Address of string table */
#define ELF_DT_SYMTAB 6           /* Address of symbol table */
#define ELF_DT_RELA 7             /* Address of Rela relocs */
#define ELF_DT_RELASZ 8           /* Total size of Rela relocs */
#define ELF_DT_RELAENT 9          /* Size of one Rela reloc */
#define ELF_DT_STRSZ 10           /* Size of string table */
#define ELF_DT_SYMENT 11          /* Size of one symbol table entry */
#define ELF_DT_INIT 12            /* Address of init function */
#define ELF_DT_FINI 13            /* Address of termination function */
#define ELF_DT_SONAME 14          /* Name of shared object */
#define ELF_DT_RPATH 15           /* Library search path (deprecated) */
#define ELF_DT_SYMBOLIC 16        /* Start symbol search here */
#define ELF_DT_REL 17             /* Address of Rel relocs */
#define ELF_DT_RELSZ 18           /* Total size of Rel relocs */
#define ELF_DT_RELENT 19          /* Size of one Rel reloc */
#define ELF_DT_PLTREL 20          /* Type of reloc in PLT */
#define ELF_DT_DEBUG 21           /* For debugging; unspecified */
#define ELF_DT_TEXTREL 22         /* Reloc might modify .text */
#define ELF_DT_JMPREL 23          /* Address of PLT relocs */
#define ELF_DT_BIND_NOW 24        /* Process relocations of object */
#define ELF_DT_INIT_ARRAY 25      /* Array with addresses of init fct */
#define ELF_DT_FINI_ARRAY 26      /* Array with addresses of fini fct */
#define ELF_DT_INIT_ARRAYSZ 27    /* Size in bytes of ELF_DT_INIT_ARRAY */
#define ELF_DT_FINI_ARRAYSZ 28    /* Size in bytes of ELF_DT_FINI_ARRAY */
#define ELF_DT_RUNPATH 29         /* Library search path */
#define ELF_DT_FLAGS 30           /* Flags for the object being loaded */
#define ELF_DT_ENCODING 32        /* Start of encoded range */
#define ELF_DT_PREINIT_ARRAY 32   /* Array with addresses of preinit fct*/
#define ELF_DT_PREINIT_ARRAYSZ 33 /* size in bytes of ELF_DT_PREINIT_ARRAY */
#define ELF_DT_NUM 34             /* Number used */
#define ELF_DT_LOOS 0x6000000d    /* Start of OS-specific */
#define ELF_DT_HIOS 0x6ffff000    /* End of OS-specific */
#define ELF_DT_LOPROC 0x70000000  /* Start of processor-specific */
#define ELF_DT_HIPROC 0x7fffffff  /* End of processor-specific */

/* DT_* entries which fall between ELF_DT_VALRNGHI & ELF_DT_VALRNGLO use
   the Dyn.d_un.d_val field of the Elf*_dyn structure.  This follows Sun's
   approach.  */
#define ELF_DT_VALRNGLO 0x6ffffd00
#define ELF_DT_GNU_PRELINKED 0x6ffffdf5  /* Prelinking timestamp */
#define ELF_DT_GNU_CONFLICTSZ 0x6ffffdf6 /* Size of conflict section */
#define ELF_DT_GNU_LIBLISTSZ 0x6ffffdf7  /* Size of library list */
#define ELF_DT_CHECKSUM 0x6ffffdf8
#define ELF_DT_PLTPADSZ 0x6ffffdf9
#define ELF_DT_MOVEENT 0x6ffffdfa
#define ELF_DT_MOVESZ 0x6ffffdfb
#define ELF_DT_FEATURE_1 0x6ffffdfc /* Feature selection (DTF_*). */
#define ELF_DT_POSFLAG_1 \
  0x6ffffdfd                       /* Flags for DT_* entries, effecting the following DT_* \
                                      entry. */
#define ELF_DT_SYMINSZ 0x6ffffdfe  /* Size of syminfo table (in bytes) */
#define ELF_DT_SYMINENT 0x6ffffdff /* Entry size of syminfo */
#define ELF_DT_VALRNGHI 0x6ffffdff
#define ELF_DT_VALTAGIDX(tag) (ELF_DT_VALRNGHI - (tag)) /* Reverse order! */
#define ELF_DT_VALNUM 12

/* DT_* entries which fall between ELF_DT_ADDRRNGHI & ELF_DT_ADDRRNGLO use
   the Dyn.d_un.d_ptr field of the Elf*_dyn structure.

   If any adjustment is made to the ELF object after it has been
   built these entries will need to be adjusted.  */
#define ELF_DT_ADDRRNGLO 0x6ffffe00
#define ELF_DT_GNU_HASH 0x6ffffe05
#define ELF_DT_TLSDESC_PLT 0x6ffffe06
#define ELF_DT_TLSDESC_GOT 0x6ffffe07
#define ELF_DT_GNU_CONFLICT 0x6ffffef8 /* Start of conflict section */
#define ELF_DT_GNU_LIBLIST 0x6ffffef9  /* Library list */
#define ELF_DT_CONFIG 0x6ffffefa       /* Configuration information. */
#define ELF_DT_DEPAUDIT 0x6ffffefb     /* Dependency auditing. */
#define ELF_DT_AUDIT 0x6ffffefc        /* Object auditing. */
#define ELF_DT_PLTPAD 0x6ffffefd       /* PLT padding. */
#define ELF_DT_MOVETAB 0x6ffffefe      /* Move table. */
#define ELF_DT_SYMINFO 0x6ffffeff      /* Syminfo table. */
#define ELF_DT_ADDRRNGHI 0x6ffffeff
#define ELF_DT_ADDRTAGIDX(tag) (ELF_DT_ADDRRNGHI - (tag)) /* Reverse order! */
#define ELF_DT_ADDRNUM 10

/* The versioning entry types.  The next are defined as part of the
   GNU extension.  */
#define ELF_DT_VERSYM 0x6ffffff0

#define ELF_DT_RELACOUNT 0x6ffffff9
#define ELF_DT_RELCOUNT 0x6ffffffa

/* These were chosen by Sun.  */
#define ELF_DT_FLAGS_1 0x6ffffffb /* State flags, see DF_1_* below. */
#define ELF_DT_VERDEF \
  0x6ffffffc                                                  /* Address of version definition table \
                                                               */
#define ELF_DT_VERDEFNUM 0x6ffffffd                           /* Number of version definitions */
#define ELF_DT_VERNEED 0x6ffffffe                             /* Address of table with needed versions */
#define ELF_DT_VERNEEDNUM 0x6fffffff                          /* Number of needed versions */
#define ELF_DT_VERSIONTAGIDX(tag) (ELF_DT_VERNEEDNUM - (tag)) /* Reverse order! */
#define ELF_DT_VERSIONTAGNUM 16

/* Sun added these machine-independent extensions in the
   "processor-specific" range.  Be compatible.  */
#define ELF_DT_AUXILIARY \
  0x7ffffffd                     /* Shared object to load before self \
                                  */
#define ELF_DT_FILTER 0x7fffffff /* Shared object to get values from */
#define ELF_DT_EXTRATAGIDX(tag) ((uint32) - ((int32)(tag) << 1 >> 1) - 1)
#define ELF_DT_EXTRANUM 3

/* Values of `d_un.d_val' in the ELF_DT_FLAGS entry.  */
#define ELF_DF_ORIGIN 0x00000001   /* Object may use ELF_DF_ORIGIN */
#define ELF_DF_SYMBOLIC 0x00000002 /* Symbol resolutions starts here */
#define ELF_DF_TEXTREL 0x00000004  /* Object contains text relocations */
#define ELF_DF_BIND_NOW 0x00000008 /* No lazy binding for this object */
#define ELF_DF_STATIC_TLS \
  0x00000010 /* Module uses the static TLS model \
              */

/* State flags selectable in the `d_un.d_val' element of the ELF_DT_FLAGS_1
   entry in the dynamic section.  */
#define ELF_DF_1_NOW 0x00000001       /* Set RTLD_NOW for this object. */
#define ELF_DF_1_GLOBAL 0x00000002    /* Set RTLD_GLOBAL for this object. */
#define ELF_DF_1_GROUP 0x00000004     /* Set RTLD_GROUP for this object. */
#define ELF_DF_1_NODELETE 0x00000008  /* Set RTLD_NODELETE for this object.*/
#define ELF_DF_1_LOADFLTR 0x00000010  /* Trigger filtee loading at runtime.*/
#define ELF_DF_1_INITFIRST 0x00000020 /* Set RTLD_INITFIRST for this object*/
#define ELF_DF_1_NOOPEN 0x00000040    /* Set RTLD_NOOPEN for this object. */
#define ELF_DF_1_ORIGIN 0x00000080    /* $ORIGIN must be handled. */
#define ELF_DF_1_DIRECT 0x00000100    /* Direct binding enabled. */
#define ELF_DF_1_TRANS 0x00000200
#define ELF_DF_1_INTERPOSE 0x00000400 /* Object is used to interpose. */
#define ELF_DF_1_NODEFLIB \
  0x00000800                       /* Ignore default lib search path. \
                                    */
#define ELF_DF_1_NODUMP 0x00001000 /* Object can't be dldump'ed. */
#define ELF_DF_1_CONFALT \
  0x00002000                           /* Configuration alternative \
                                          created.*/
#define ELF_DF_1_ENDFILTEE 0x00004000  /* Filtee terminates filters search. */
#define ELF_DF_1_DISPRELDNE 0x00008000 /* Disp reloc applied at build time. */
#define ELF_DF_1_DISPRELPND 0x00010000 /* Disp reloc applied at run-time. */

/* Flags for the feature selection in ELF_DT_FEATURE_1.  */
#define ELF_DTF_1_PARINIT 0x00000001
#define ELF_DTF_1_CONFEXP 0x00000002

/* Flags in the ELF_DT_POSFLAG_1 entry effecting only the next DT_* entry.
 */
#define ELF_DF_P1_LAZYLOAD 0x00000001  /* Lazyload following object. */
#define ELF_DF_P1_GROUPPERM 0x00000002 /* Symbols from next object are not generally available. */

/* Version definition sections.  */

typedef struct __unaligned {
  uint16 vd_version; /* Version revision */
  uint16 vd_flags;   /* Version information */
  uint16 vd_ndx;     /* Version Index */
  uint16 vd_cnt;     /* Number of associated aux entries */
  uint32 vd_hash;    /* Version name hash value */
  uint32 vd_aux;     /* Offset in bytes to verdaux array */
  uint32 vd_next;    /* Offset in bytes to next verdef
                entry */
}
elf32_verdef;

typedef struct __unaligned {
  uint16 vd_version; /* Version revision */
  uint16 vd_flags;   /* Version information */
  uint16 vd_ndx;     /* Version Index */
  uint16 vd_cnt;     /* Number of associated aux entries */
  uint32 vd_hash;    /* Version name hash value */
  uint32 vd_aux;     /* Offset in bytes to verdaux array */
  uint32 vd_next;    /* Offset in bytes to next verdef
                entry */
}
elf64_verdef;

/* Legal values for vd_version (version revision).  */
#define ELF_VER_DEF_NONE 0    /* No version */
#define ELF_VER_DEF_CURRENT 1 /* Current version */
#define ELF_VER_DEF_NUM 2     /* Given version number */

/* Legal values for vd_flags (version information flags).  */
#define ELF_VER_FLG_BASE 0x1 /* Version definition of file itself */
#define ELF_VER_FLG_WEAK 0x2 /* Weak version identifier */

/* Versym symbol index values.  */
#define ELF_VER_NDX_LOCAL 0          /* Symbol is local. */
#define ELF_VER_NDX_GLOBAL 1         /* Symbol is global. */
#define ELF_VER_NDX_LORESERVE 0xff00 /* Beginning of reserved entries. */
#define ELF_VER_NDX_ELIMINATE 0xff01 /* Symbol is to be eliminated. */

/* Auxialiary version information.  */

typedef struct {
  uint32 vda_name; /* Version or dependency names */
  uint32 vda_next; /* Offset in bytes to next verdaux
              entry */
} elf32_verdaux;

typedef struct {
  uint32 vda_name; /* Version or dependency names */
  uint32 vda_next; /* Offset in bytes to next verdaux
              entry */
} elf64_verdaux;

/* Version dependency section.  */

typedef struct __unaligned {
  uint16 vn_version; /* Version of structure */
  uint16 vn_cnt;     /* Number of associated aux entries */
  uint32 vn_file;    /* Offset of filename for this
                dependency */
  uint32 vn_aux;     /* Offset in bytes to vernaux array */
  uint32 vn_next;    /* Offset in bytes to next verneed
                entry */
}
elf32_verneed;

typedef struct __unaligned {
  uint16 vn_version; /* Version of structure */
  uint16 vn_cnt;     /* Number of associated aux entries */
  uint32 vn_file;    /* Offset of filename for this
                dependency */
  uint32 vn_aux;     /* Offset in bytes to vernaux array */
  uint32 vn_next;    /* Offset in bytes to next verneed
                entry */
}
elf64_verneed;

/* Legal values for vn_version (version revision).  */
#define ELF_VER_NEED_NONE 0    /* No version */
#define ELF_VER_NEED_CURRENT 1 /* Current version */
#define ELF_VER_NEED_NUM 2     /* Given version number */

/* Auxiliary needed version information.  */

typedef struct __unaligned {
  uint32 vna_hash;  /* Hash value of dependency name */
  uint16 vna_flags; /* Dependency specific information */
  uint16 vna_other; /* Unused */
  uint32 vna_name;  /* Dependency name string offset */
  uint32 vna_next;  /* Offset in bytes to next vernaux
               entry */
}
elf32_vernaux;

typedef struct __unaligned {
  uint32 vna_hash;  /* Hash value of dependency name */
  uint16 vna_flags; /* Dependency specific information */
  uint16 vna_other; /* Unused */
  uint32 vna_name;  /* Dependency name string offset */
  uint32 vna_next;  /* Offset in bytes to next vernaux
               entry */
}
elf64_vernaux;

/* Legal values for vna_flags.  */
#define ELF_VER_FLG_WEAK 0x2 /* Weak version identifier */

/* Auxiliary vector.  */

/* This vector is normally only used by the program interpreter.  The
   usual definition in an ABI supplement uses the name auxv_t.  The
   vector is not usually defined in a standard <elf.h> file, but it
   can't hurt.  We rename it to avoid conflicts.  The sizes of these
   types are an arrangement between the exec server and the program
   interpreter, so we don't fully specify them here.  */

typedef struct {
  int32 a_type; /* Entry type */
  union {
    long int a_val;      /* Integer value */
    void* a_ptr;         /* Pointer value */
    void (*a_fcn)(void); /* Function pointer value */
  } a_un;
} elf32_auxv_t;

typedef struct {
  long int a_type; /* Entry type */
  union {
    long int a_val;      /* Integer value */
    void* a_ptr;         /* Pointer value */
    void (*a_fcn)(void); /* Function pointer value */
  } a_un;
} elf64_auxv_t;

/* Legal values for a_type (entry type).  */

#define ELF_AT_NULL 0    /* End of vector */
#define ELF_AT_IGNORE 1  /* Entry should be ignored */
#define ELF_AT_EXECFD 2  /* File descriptor of program */
#define ELF_AT_PHDR 3    /* Program headers for program */
#define ELF_AT_PHENT 4   /* Size of program header entry */
#define ELF_AT_PHNUM 5   /* Number of program headers */
#define ELF_AT_PAGESZ 6  /* System page size */
#define ELF_AT_BASE 7    /* Base address of interpreter */
#define ELF_AT_FLAGS 8   /* Flags */
#define ELF_AT_ENTRY 9   /* Entry point of program */
#define ELF_AT_NOTELF 10 /* Program is not ELF */
#define ELF_AT_UID 11    /* Real uid */
#define ELF_AT_EUID 12   /* Effective uid */
#define ELF_AT_GID 13    /* Real gid */
#define ELF_AT_EGID 14   /* Effective gid */
#define ELF_AT_CLKTCK 17 /* Frequency of times() */

/* Some more special a_type values describing the hardware.  */
#define ELF_AT_PLATFORM 15 /* String identifying platform. */
#define ELF_AT_HWCAP 16    /* Machine dependent hints about processor capabilities. */

/* This entry gives some information about the FPU initialization
   performed by the kernel.  */
#define ELF_AT_FPUCW 18 /* Used FPU control word. */

/* Cache block sizes.  */
#define ELF_AT_DCACHEBSIZE 19 /* Data cache block size. */
#define ELF_AT_ICACHEBSIZE 20 /* Instruction cache block size. */
#define ELF_AT_UCACHEBSIZE 21 /* Unified cache block size. */

/* A special ignored value for PPC, used by the kernel to control the
   interpretation of the AUXV. Must be > 16.  */
#define ELF_AT_IGNOREPPC 22 /* Entry should be ignored. */

#define ELF_AT_SECURE 23 /* Boolean, was exec setuid-like? */

#define ELF_AT_BASE_PLATFORM 24 /* String identifying real platforms.*/

#define ELF_AT_RANDOM 25 /* Address of 16 random bytes. */

#define ELF_AT_HWCAP2 26 /* More machine-dependent hints about processor capabilities. */

#define ELF_AT_EXECFN 31 /* Filename of executable. */

/* Pointer to the global system page used for system calls and other
   nice things.  */
#define ELF_AT_SYSINFO 32
#define ELF_AT_SYSINFO_EHDR 33

/* Note section contents.  Each entry in the note section begins with
   a header of a fixed form.  */

typedef struct {
  uint32 n_namesz; /* Length of the note's name.  */
  uint32 n_descsz; /* Length of the note's descriptor.  */
  uint32 n_type;   /* Type of the note.  */
} elf32_nhdr;

typedef struct {
  uint32 n_namesz; /* Length of the note's name.  */
  uint32 n_descsz; /* Length of the note's descriptor.  */
  uint32 n_type;   /* Type of the note.  */
} elf64_nhdr;

/* Known names of notes.  */

/* Solaris entries in the note section have this name.  */
#define ELF_NOTE_SOLARIS "SUNW Solaris"

/* Note entries for GNU systems have this name.  */
#define ELF_NOTE_GNU "GNU"

/* Defined types of notes for Solaris.  */

/* Value of descriptor (one word) is desired pagesize for the binary.  */
#define ELF_NOTE_PAGESIZE_HINT 1

/* Defined note types for GNU systems.  */

/* ABI information.  The descriptor consists of words:
   word 0: OS descriptor
   word 1: major version of the ABI
   word 2: minor version of the ABI
   word 3: subminor version of the ABI
*/
#define ELF_NOTE_ABI 1

/* Known OSes.  These value can appear in word 0 of an ELF_NOTE_ABI
   note section entry.  */
#define ELF_NOTE_OS_LINUX 0
#define ELF_NOTE_OS_GNU 1
#define ELF_NOTE_OS_SOLARIS2 2
#define ELF_NOTE_OS_FREEBSD 3

/* Move records.  */
typedef struct __unaligned {
  uint64 m_value;   /* Symbol value.  */
  uint32 m_info;    /* Size and index.  */
  uint32 m_poffset; /* Symbol offset.  */
  uint16 m_repeat;  /* Repeat count.  */
  uint16 m_stride;  /* Stride info.  */
}
elf32_move;

typedef struct __unaligned {
  uint64 m_value;   /* Symbol value.  */
  uint64 m_info;    /* Size and index.  */
  uint64 m_poffset; /* Symbol offset.  */
  uint16 m_repeat;  /* Repeat count.  */
  uint16 m_stride;  /* Stride info.  */
}
elf64_move;

/* Macro to construct move records.  */
#define ELF_ELF32_M_SYM(info) ((info) >> 8)
#define ELF_ELF32_M_SIZE(info) ((uint8)(info))
#define ELF_ELF32_M_INFO(sym, size) (((sym) << 8) + (uint8)(size))

#define ELF_ELF64_M_SYM(info) ELF_ELF32_M_SYM(info)
#define ELF_ELF64_M_SIZE(info) ELF_ELF32_M_SIZE(info)
#define ELF_ELF64_M_INFO(sym, size) ELF_ELF32_M_INFO(sym, size)

/* Motorola 68k specific definitions.  */

/* Values for elf32_ehdr.e_flags.  */
#define ELF_EF_CPU32 0x00810000

/* m68k relocs.  */

/* Keep this the last entry.  */

/* Intel 80386 specific definitions.  */

/* i386 relocs.  */

#define ELF_R_386_NONE 0     /* No reloc */
#define ELF_R_386_32 1       /* Direct 32 bit */
#define ELF_R_386_PC32 2     /* PC relative 32 bit */
#define ELF_R_386_GOT32 3    /* 32 bit GOT entry */
#define ELF_R_386_PLT32 4    /* 32 bit PLT address */
#define ELF_R_386_COPY 5     /* Copy symbol at runtime */
#define ELF_R_386_GLOB_DAT 6 /* Create GOT entry */
#define ELF_R_386_JMP_SLOT 7 /* Create PLT entry */
#define ELF_R_386_RELATIVE 8 /* Adjust by program base */
#define ELF_R_386_GOTOFF 9   /* 32 bit offset to GOT */
#define ELF_R_386_GOTPC 10   /* 32 bit PC relative offset to GOT */
#define ELF_R_386_32PLT 11
#define ELF_R_386_TLS_TPOFF 14 /* Offset in static TLS block */
#define ELF_R_386_TLS_IE 15    /* Address of GOT entry for static TLS block offset */
#define ELF_R_386_TLS_GOTIE \
  16                        /* GOT entry for static TLS block offset \
                             */
#define ELF_R_386_TLS_LE 17 /* Offset relative to static TLS block */
#define ELF_R_386_TLS_GD \
  18 /* Direct 32 bit for GNU version of general dynamic thread local \
      * data \
      */
#define ELF_R_386_TLS_LDM \
  19 /* Direct 32 bit for GNU version of local dynamic thread local data \
        in LE code */
#define ELF_R_386_16 20
#define ELF_R_386_PC16 21
#define ELF_R_386_8 22
#define ELF_R_386_PC8 23
#define ELF_R_386_TLS_GD_32 24    /* Direct 32 bit for general dynamic thread local data */
#define ELF_R_386_TLS_GD_PUSH 25  /* Tag for pushl in GD TLS code */
#define ELF_R_386_TLS_GD_CALL 26  /* Relocation for call to __tls_get_addr() */
#define ELF_R_386_TLS_GD_POP 27   /* Tag for popl in GD TLS code */
#define ELF_R_386_TLS_LDM_32 28   /* Direct 32 bit for local dynamic thread local data in LE code */
#define ELF_R_386_TLS_LDM_PUSH 29 /* Tag for pushl in LDM TLS code */
#define ELF_R_386_TLS_LDM_CALL 30 /* Relocation for call to __tls_get_addr() in LDM code */
#define ELF_R_386_TLS_LDM_POP 31  /* Tag for popl in LDM TLS code */
#define ELF_R_386_TLS_LDO_32 32   /* Offset relative to TLS block */
#define ELF_R_386_TLS_IE_32 33    /* GOT entry for negated static TLS block offset */
#define ELF_R_386_TLS_LE_32 \
  34                              /* Negated offset relative to static TLS block \
                                   */
#define ELF_R_386_TLS_DTPMOD32 35 /* ID of module containing symbol */
#define ELF_R_386_TLS_DTPOFF32 36 /* Offset in TLS block */
#define ELF_R_386_TLS_TPOFF32 37  /* Negated offset in static TLS block */
/* Keep this the last entry.  */
#define ELF_R_386_NUM 38

/* Entries found in sections of type ELF_SHT_MIPS_GPTAB.  */

typedef union {
  struct {
    uint32 gt_current_g_value; /* -G value used for compilation */
    uint32 gt_unused;          /* Not used */
  } gt_header;                 /* First entry in section */
  struct {
    uint32 gt_g_value; /* If this value were used for -G */
    uint32 gt_bytes;   /* This many bytes would be used */
  } gt_entry;          /* Subsequent entries in section */
} elf32_gptab;

/* Entry found in sections of type ELF_SHT_MIPS_REGINFO.  */

typedef struct {
  uint32 ri_gprmask;    /* General registers used */
  uint32 ri_cprmask[4]; /* Coprocessor registers used */
  int32 ri_gp_value;    /* $gp register value */
} elf32_reg_info;

/* Entries found in sections of type ELF_SHT_MIPS_OPTIONS.  */

typedef struct __unaligned {
  uint8 kind;     /* Determines interpretation of the
                 variable part of descriptor.  */
  uint8 size;     /* Size of descriptor, including header.  */
  uint16 section; /* Section header index of section affected,
              0 for global options.  */
  uint32 info;    /* Kind-specific information.  */
}
elf_options;

/* Values for `kind' field in elf_options.  */

#define ELF_ODK_NULL 0       /* Undefined. */
#define ELF_ODK_REGINFO 1    /* Register usage information. */
#define ELF_ODK_EXCEPTIONS 2 /* Exception processing options. */
#define ELF_ODK_PAD 3        /* Section padding options. */
#define ELF_ODK_HWPATCH 4    /* Hardware workarounds performed */
#define ELF_ODK_FILL 5       /* record the fill value used by the linker. */
#define ELF_ODK_TAGS 6       /* reserve space for desktop tools to write. */
#define ELF_ODK_HWAND 7      /* HW workarounds. 'AND' bits when merging. */
#define ELF_ODK_HWOR 8       /* HW workarounds. 'OR' bits when merging. */

/* Values for `info' in elf_options for ELF_ODK_EXCEPTIONS entries.  */

#define ELF_OEX_FPU_MIN 0x1f   /* FPE's which MUST be enabled. */
#define ELF_OEX_FPU_MAX 0x1f00 /* FPE's which MAY be enabled. */
#define ELF_OEX_PAGE0 0x10000  /* page zero must be mapped. */
#define ELF_OEX_SMM 0x20000    /* Force sequential memory mode? */
#define ELF_OEX_FPDBUG 0x40000 /* Force floating point debug mode? */
#define ELF_OEX_PRECISEFP ELF_OEX_FPDBUG
#define ELF_OEX_DISMISS 0x80000 /* Dismiss invalid address faults? */

#define ELF_OEX_FPU_INVAL 0x10
#define ELF_OEX_FPU_DIV0 0x08
#define ELF_OEX_FPU_OFLO 0x04
#define ELF_OEX_FPU_UFLO 0x02
#define ELF_OEX_FPU_INEX 0x01

/* Masks for `info' in elf_options for an ELF_ODK_HWPATCH entry.  */

#define ELF_OHW_R4KEOP 0x1    /* R4000 end-of-page patch. */
#define ELF_OHW_R8KPFETCH 0x2 /* may need R8000 prefetch patch. */
#define ELF_OHW_R5KEOP 0x4    /* R5000 end-of-page patch. */
#define ELF_OHW_R5KCVTL 0x8   /* R5000 cvt.[ds].l bug.  clean=1.  */

#define ELF_OPAD_PREFIX 0x1
#define ELF_OPAD_POSTFIX 0x2
#define ELF_OPAD_SYMBOL 0x4

/* Entry found in `.options' section.  */

typedef struct {
  uint32 hwp_flags1; /* Extra flags.  */
  uint32 hwp_flags2; /* Extra flags.  */
} elf_options_hw;

/* Masks for `info' in ElfOptions for ELF_ODK_HWAND and ELF_ODK_HWOR
 * entries. */

#define ELF_OHWA0_R4KEOP_CHECKED 0x00000001
#define ELF_OHWA1_R4KEOP_CLEAN 0x00000002

/* Entries found in sections of type ELF_SHT_MIPS_LIBLIST.  */

typedef struct {
  uint32 l_name;       /* Name (string table index) */
  uint32 l_time_stamp; /* Timestamp */
  uint32 l_checksum;   /* Checksum */
  uint32 l_version;    /* Interface version */
  uint32 l_flags;      /* Flags */
} elf32_lib;

typedef struct {
  uint32 l_name;       /* Name (string table index) */
  uint32 l_time_stamp; /* Timestamp */
  uint32 l_checksum;   /* Checksum */
  uint32 l_version;    /* Interface version */
  uint32 l_flags;      /* Flags */
} elf64_lib;

/* Legal values for l_flags.  */

#define ELF_LL_NONE 0
#define ELF_LL_EXACT_MATCH (1 << 0)    /* Require exact match */
#define ELF_LL_IGNORE_INT_VER (1 << 1) /* Ignore interface version */
#define ELF_LL_REQUIRE_MINOR (1 << 2)
#define ELF_LL_EXPORTS (1 << 3)
#define ELF_LL_DELAY_LOAD (1 << 4)
#define ELF_LL_DELTA (1 << 5)

/* Entries found in sections of type ELF_SHT_MIPS_CONFLICT.  */

typedef uint32 elf32_conflict;

/* ARM specific declarations */

/* Processor specific flags for the ELF header e_flags field.  */
#define ELF_EF_ARM_RELEXEC 0x01
#define ELF_EF_ARM_HASENTRY 0x02
#define ELF_EF_ARM_INTERWORK 0x04
#define ELF_EF_ARM_APCS_26 0x08
#define ELF_EF_ARM_APCS_FLOAT 0x10
#define ELF_EF_ARM_PIC 0x20
#define ELF_EF_ARM_ALIGN8 0x40 /* 8-bit structure alignment is in use */
#define ELF_EF_ARM_NEW_ABI 0x80
#define ELF_EF_ARM_OLD_ABI 0x100

/* Other constants defined in the ARM ELF spec. version B-01.  */
/* NB. These conflict with values defined above.  */
#define ELF_EF_ARM_SYMSARESORTED 0x04
#define ELF_EF_ARM_DYNSYMSUSESEGIDX 0x08
#define ELF_EF_ARM_MAPSYMSFIRST 0x10
#define ELF_EF_ARM_EABIMASK 0XFF000000

#define ELF_EF_ARM_EABI_VERSION(flags) ((flags)&ELF_EF_ARM_EABIMASK)
#define ELF_EF_ARM_EABI_UNKNOWN 0x00000000
#define ELF_EF_ARM_EABI_VER1 0x01000000
#define ELF_EF_ARM_EABI_VER2 0x02000000
#define ELF_EF_ARM_EABI_VER4 0x04000000

/* Additional symbol types for Thumb */
#define ELF_STT_ARM_TFUNC 0xd

/* ARM-specific values for sh_flags */
#define ELF_SHF_ARM_ENTRYSECT 0x10000000 /* Section contains an entry point */
#define ELF_SHF_ARM_COMDEF \
  0x80000000 /* Section may be multiply defined in the input to a link \
                step */

/* ARM-specific program header flags */
#define ELF_PF_ARM_SB \
  0x10000000 /* Segment contains the location addressed by the static \
                base */

/* ARM relocs.  */
#define ELF_R_ARM_NONE 0  /* No reloc */
#define ELF_R_ARM_PC24 1  /* PC relative 26 bit branch */
#define ELF_R_ARM_ABS32 2 /* Direct 32 bit */
#define ELF_R_ARM_REL32 3 /* PC relative 32 bit */
#define ELF_R_ARM_PC13 4
#define ELF_R_ARM_ABS16 5 /* Direct 16 bit */
#define ELF_R_ARM_ABS12 6 /* Direct 12 bit */
#define ELF_R_ARM_THM_ABS5 7
#define ELF_R_ARM_ABS8 8 /* Direct 8 bit */
#define ELF_R_ARM_SBREL32 9
#define ELF_R_ARM_THM_PC22 10
#define ELF_R_ARM_THM_PC8 11
#define ELF_R_ARM_AMP_VCALL9 12
#define ELF_R_ARM_SWI24 13
#define ELF_R_ARM_THM_SWI8 14
#define ELF_R_ARM_XPC25 15
#define ELF_R_ARM_THM_XPC22 16
#define ELF_R_ARM_COPY 20      /* Copy symbol at runtime */
#define ELF_R_ARM_GLOB_DAT 21  /* Create GOT entry */
#define ELF_R_ARM_JUMP_SLOT 22 /* Create PLT entry */
#define ELF_R_ARM_RELATIVE 23  /* Adjust by program base */
#define ELF_R_ARM_GOTOFF 24    /* 32 bit offset to GOT */
#define ELF_R_ARM_GOTPC 25     /* 32 bit PC relative offset to GOT */
#define ELF_R_ARM_GOT32 26     /* 32 bit GOT entry */
#define ELF_R_ARM_PLT32 27     /* 32 bit PLT address */
#define ELF_R_ARM_ALU_PCREL_7_0 32
#define ELF_R_ARM_ALU_PCREL_15_8 33
#define ELF_R_ARM_ALU_PCREL_23_15 34
#define ELF_R_ARM_LDR_SBREL_11_0 35
#define ELF_R_ARM_ALU_SBREL_19_12 36
#define ELF_R_ARM_ALU_SBREL_27_20 37
#define ELF_R_ARM_GNU_VTENTRY 100
#define ELF_R_ARM_GNU_VTINHERIT 101
#define ELF_R_ARM_THM_PC11 102 /* thumb unconditional branch */
#define ELF_R_ARM_THM_PC9 103  /* thumb conditional branch */
#define ELF_R_ARM_RXPC25 249
#define ELF_R_ARM_RSBREL32 250
#define ELF_R_ARM_THM_RPC22 251
#define ELF_R_ARM_RREL32 252
#define ELF_R_ARM_RABS22 253
#define ELF_R_ARM_RPC24 254
#define ELF_R_ARM_RBASE 255
/* Keep this the last entry.  */
#define ELF_R_ARM_NUM 256

/* AMD x86-64 relocations.  */
#define ELF_R_X86_64_NONE 0      /* No reloc */
#define ELF_R_X86_64_64 1        /* Direct 64 bit */
#define ELF_R_X86_64_PC32 2      /* PC relative 32 bit signed */
#define ELF_R_X86_64_GOT32 3     /* 32 bit GOT entry */
#define ELF_R_X86_64_PLT32 4     /* 32 bit PLT address */
#define ELF_R_X86_64_COPY 5      /* Copy symbol at runtime */
#define ELF_R_X86_64_GLOB_DAT 6  /* Create GOT entry */
#define ELF_R_X86_64_JUMP_SLOT 7 /* Create PLT entry */
#define ELF_R_X86_64_RELATIVE 8  /* Adjust by program base */
#define ELF_R_X86_64_GOTPCREL 9  /* 32 bit signed PC relative offset to GOT */
#define ELF_R_X86_64_32 10       /* Direct 32 bit zero extended */
#define ELF_R_X86_64_32S 11      /* Direct 32 bit sign extended */
#define ELF_R_X86_64_16 12       /* Direct 16 bit zero extended */
#define ELF_R_X86_64_PC16 13     /* 16 bit sign extended pc relative */
#define ELF_R_X86_64_8 14        /* Direct 8 bit sign extended */
#define ELF_R_X86_64_PC8 15      /* 8 bit sign extended pc relative */
#define ELF_R_X86_64_DTPMOD64 16 /* ID of module containing symbol */
#define ELF_R_X86_64_DTPOFF64 17 /* Offset in module's TLS block */
#define ELF_R_X86_64_TPOFF64 18  /* Offset in initial TLS block */
#define ELF_R_X86_64_TLSGD \
  19 /* 32 bit signed PC relative offset to two GOT entries for GD symbol \
      */
#define ELF_R_X86_64_TLSLD \
  20                             /* 32 bit signed PC relative offset to two GOT entries for LD symbol \
                                  */
#define ELF_R_X86_64_DTPOFF32 21 /* Offset in TLS block */
#define ELF_R_X86_64_GOTTPOFF 22 /* 32 bit signed PC relative offset to GOT entry for IE symbol */
#define ELF_R_X86_64_TPOFF32 23  /* Offset in initial TLS block */

#define ELF_R_X86_64_NUM 24

#define __ELF_NATIVE_CLASS __WORDSIZE

range elf_dynamic_section(void*);
range elf_get_section_r(void*, const char* name);
void* elf_get_section(void*, const char* name, size_t* szp);
range elf_get_symtab_r(void*);
void* elf_get_symtab(void*, size_t* szp);
uint64 elf_get_value(void*, void* ptr, unsigned off32, unsigned size32, unsigned off64, unsigned size64);
uint8* elf_header_ident(void*);
void* elf_header_sections(void*);
range elf_program_headers(void*);
int elf_section_find(void*, const char* sname);
range elf_section_headers(void*);
int elf_section_index(void*, uint32 sh_type);
const char* elf_section_name(void*, int sn);
void* elf_section_offset(void*, int sn);
size_t elf_section_size(void*, int sn);
const char* elf_section_typename(uint32);
range elf_section(void*, void* shdr);
const char* elf_shstrtab(void*);
range elf_symbol_r(void*, void* sym);
void* elf_find_segment_offset(range map, uint64 offs);
void* elf_find_segment_vaddr(range map, uint64 addr);
int64 elf_address_to_offset(range map, uint64 addr);
int64 elf_offset_to_addr(range map, uint64 offs);

#define ELF_BITS(elf) (elf_header_ident((elf))[ELF_EI_CLASS] == ELF_ELFCLASS64 ? 64 : 32)
#define ELF_32(elf) (elf_header_ident((elf))[ELF_EI_CLASS] == ELF_ELFCLASS32)
#define ELF_64(elf) (elf_header_ident((elf))[ELF_EI_CLASS] == ELF_ELFCLASS64)

#define ELF_FIELD_OFFS(type, field) ((size_t)(uint8*)&(((type*)0)->field))
#define ELF_FIELD_SIZE(type, field) sizeof(((type*)0)->field)

#define ELF_STRUCT_OFFSETS(st, field) ELF_FIELD_OFFS(elf32_##st, field), ELF_FIELD_SIZE(elf32_##st, field), ELF_FIELD_OFFS(elf64_##st, field), ELF_FIELD_SIZE(elf64_##st, field))
#define ELF_STRUCT_SIZE(elf, st) (ELF_64(elf) ? sizeof(elf64_##st) : sizeof(elf32_##st))

#define ELF_GET(elf, ptr, st, field) \
  elf_get_value(elf, ptr, ELF_FIELD_OFFS(elf32_##st, field), ELF_FIELD_SIZE(elf32_##st, field), ELF_FIELD_OFFS(elf64_##st, field), ELF_FIELD_SIZE(elf64_##st, field))

#define ELF_FIELD_OFFSET(type, field) ((size_t)(uint8*)&(((type*)0)->field))
#define ELF_FIELD_SIZE(type, field) sizeof(((type*)0)->field)

#define ELF_ADDR(elf, ptr, st, field) ((void*)(((char*)ptr) + ELF_OFFSET(elf, st, field)))
#define ELF_OFFSET(elf, st, field) (ELF_64(elf) ? ELF_FIELD_OFFSET(elf64_##st, field) : ELF_FIELD_OFFSET(elf32_##st, field))
#define ELF_SIZE(elf, st, field) (ELF_64(elf) ? ELF_FIELD_SIZE(elf64_##st, field) : ELF_FIELD_SIZE(elf32_##st, field))

#ifdef __cplusplus
};
#endif

#endif /* elf.h */
/** @} */
