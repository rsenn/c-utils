#ifndef PE_H
#define PE_H

typedef struct {
  unsigned char* x;
  size_t n;
} pe_map;


typedef struct {
  uint32 virtual_address;
  uint32 size;
} pe_data_directory;

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
  uint16 machine; /* MachineType */
  uint16 number_of_sections;
  uint32 time_date_stamp;
  uint32 pointer_to_symbol_table;
  uint32 number_of_symbols;
  uint16 size_of_optional_header;
  uint16 characteristics; /* ImageCharacteristics */
} pe_file_header;

/* Quoting pecoff_v8.docx: "Entries in the section table are numbered starting from one (1)". */
typedef struct {
  unsigned char name[SECTION_NAME_SIZE]; /* TODO: Should we use char instead? */
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
  uint32 characteristics; /* SectionCharacteristics */
} pe_section_header;


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
  uint16 subsystem; /* WindowsSubsystem */
  uint16 dll_characteristics;
  uint32 size_of_stack_reserve;
  uint32 size_of_stack_commit;
  uint32 size_of_heap_reserve;
  uint32 size_of_heap_commit;
  uint32 loader_flags;
  uint32 number_of_rva_and_sizes;
  /* image_data_d_irectory DataDirectory[MAX_DIRECTORIES]; */
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
  uint16 subsystem; /* WindowsSubsystem */
  uint16 dll_characteristics;
  uint64 size_of_stack_reserve;
  uint64 size_of_stack_commit;
  uint64 size_of_heap_reserve;
  uint64 size_of_heap_commit;
  uint32 loader_flags; /* must be zero */
  uint32 number_of_rva_and_sizes;
  /* image_data_d_irectory DataDirectory[MAX_DIRECTORIES]; */
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
  PE_OPTHDR_MAGIC                          = 0,
  PE_OPTHDR_SIZE_OF_CODE                   = 1,
  PE_OPTHDR_SIZE_OF_INITIALIZED_DATA       = 2,
  PE_OPTHDR_SIZE_OF_UNINITIALIZED_DATA     = 3,
  PE_OPTHDR_ADDRESS_OF_ENTRY_POINT         = 4,
  PE_OPTHDR_BASE_OF_CODE                   = 5,
  PE_OPTHDR_IMAGE_BASE                     = 6,
  PE_OPTHDR_SECTION_ALIGNMENT              = 7,
  PE_OPTHDR_FILE_ALIGNMENT                 = 8,
  PE_OPTHDR_MAJOR_OPERATING_SYSTEM_VERSION = 9,
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


pe_file_header*
pe_filehdr_ptr(const pe_file*);

void*
pe_opthdr_ptr(const pe_file*);

uint32
pe_opthdr_offset(const pe_file*, pe_opthdr_field);

#endif /* defined PE_H */
