
typedef struct {
  uint32 virtual_address;
  uint32 size;
} pe_data_directory;

typedef enum {
  MAGIC_ROM   = 0x107,
  MAGIC_PE32  = 0x10b,
  MAGIC_PE64  = 0x20b // PE32+
} opt_type_e;

/* 64 bit version of the PE Optional Header also known as IMAGE_OPTIONAL_HEADER64 */
typedef struct {
  uint16 signature; //decimal number 267 for 32 bit, 523 for 64 bit, and 263 for a ROM image.
  char major_linker_version;
  char minor_linker_version;
  uint32 size_of_code;
  uint32 size_of_initialized_data;
  uint32 size_of_uninitialized_data;
  uint32 address_of_entry_point;  //The RVA of the code entry point
  uint32 base_of_code;
  /*The next 21 fields are an extension to the COFF optional header format*/
  uint64 image_base;
  uint32 section_alignment;
  uint32 file_alignment;
  uint16 majoros_version;
  uint16 minoros_version;
  uint16 major_image_version;
  uint16 minor_image_version;
  uint16 major_subsystem_version;
  uint16 minor_subsystem_version;
  uint32 win32_version_value;
  uint32 size_of_image;
  uint32 size_of_headers;
  uint32 Checksum;
  uint16 Subsystem;
  uint16 dll_characteristics;
  uint64 size_of_stack_reserve;
  uint64 size_of_stack_commit;
  uint64 size_of_heap_reserve;
  uint64 size_of_heap_commit;
  uint32 loader_flags;
  uint32 number_of_rva_and_sizes;
  pe_data_directory data_directory[0];     //Can have any number of elements, matching the number in number_of_rva_and_sizes.
} pe64_opt_header;                                        //However, it is always 16 in PE files.

/* This is the 32 bit version of the PE Optional Header presented as a C data structure: */

/* 32 bit version of the PE Optional Header also known as IMAGE_OPTIONAL_HEADER */
typedef struct pe32_opt_header {
  uint16 signature; //decimal number 267 for 32 bit, 523 for 64 bit, and 263 for a ROM image.
  char major_linker_version;
  char minor_linker_version;
  uint32 size_of_code;
  uint32 size_of_initialized_data;
  uint32 size_of_uninitialized_data;
  uint32 address_of_entry_point;  //The RVA of the code entry point
  uint32 base_of_code;
  uint32 base_of_data;
  /*The next 21 fields are an extension to the COFF optional header format*/
  uint32 image_base;
  uint32 section_alignment;
  uint32 file_alignment;
  uint16 majoros_version;
  uint16 minoros_version;
  uint16 major_image_version;
  uint16 minor_image_version;
  uint16 major_subsystem_version;
  uint16 minor_subsystem_version;
  uint32 win32_version_value;
  uint32 size_of_image;
  uint32 size_of_headers;
  uint32 Checksum;
  uint16 Subsystem;
  uint16 dll_characteristics;
  uint32 size_of_stack_reserve;
  uint32 size_of_stack_commit;
  uint32 size_of_heap_reserve;
  uint32 size_of_heap_commit;
  uint32 loader_flags;
  uint32 number_of_rva_and_sizes;
  pe_data_directory data_directory[0];     //Can have any number of elements, matching the number in number_of_rva_and_sizes.
} pe32_opt_header;                                       //However, it is always 16 in PE files.

