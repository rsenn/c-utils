/*   Copyright (C) 1999-2000 Free Software Foundation, Inc.

     This file is part of GNU libtool.

     This program is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     auint32 with this program; if not, write to the Free Software
     Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "byte.h"
#include "buffer.h"
#include "mmap.h"
#include "open.h"
#include "uint16.h"
#include "uint32.h"

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
  uint32 checksum;
  uint16 subsystem;
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
  uint32 checksum;
  uint16 subsystem;
  uint16 dll_characteristics;
  uint32 size_of_stack_reserve;
  uint32 size_of_stack_commit;
  uint32 size_of_heap_reserve;
  uint32 size_of_heap_commit;
  uint32 loader_flags;
  uint32 number_of_rva_and_sizes;
  pe_data_directory data_directory[0];     //Can have any number of elements, matching the number in number_of_rva_and_sizes.
} pe32_opt_header;                                       //However, it is always 16 in PE files.

int
main(int argc, char* argv[]) {
  int optarg;

  for(optarg = 1; optarg < argc; ++optarg) {
    char *dll, *filename, *dll_name;
    size_t dllsz;
    uint32 pe_header_offset, opthdr_ofs, num_entries, i;
    uint32 export_rva, export_size, expptr;
    uint16 nsections, secptr;
    uint32 name_rvas, nexp;
    char *expdata, *erva;
    pe32_opt_header* opthdr;
    pe64_opt_header* opthdr64;
    pe_data_directory* datadir;
    opt_type_e* type;

    filename = argv[optarg];

    dll = mmap_read(filename, &dllsz);
    if(dll == NULL)
      return 1;

    dll_name = filename;

    for(i = 0; filename[i]; i++)
      if(filename[i] == '/' || filename[i] == '\\'  || filename[i] == ':')
        dll_name = filename + i + 1;

    uint32_unpack(&dll[0x3c], &pe_header_offset);

    opthdr_ofs = pe_header_offset + 4 + 20;

    opthdr = (pe32_opt_header*)&dll[pe_header_offset + 4 + 20];
    opthdr64 = (pe64_opt_header*)&dll[pe_header_offset + 4 + 20];

    type = opthdr->signature;

/*    buffer_puts(buffer_2, "opt_hdr directory: ");
    buffer_putulong(buffer_2, (unsigned char*)&opthdr->number_of_rva_and_sizes - (unsigned char*)opthdr);
    buffer_putnlflush(buffer_2);
    buffer_puts(buffer_2, "opt_hdr_64 directory: ");
    buffer_putulong(buffer_2, (unsigned char*)&opthdr64->number_of_rva_and_sizes - (unsigned char*)opthdr64);
    buffer_putnlflush(buffer_2);
*/

    uint32_unpack(&dll[opthdr_ofs + (type == MAGIC_PE64 ? 108 : 92)], &num_entries);

    if(num_entries < 1)  /* no exports */
      return 1;

    datadir = (pe_data_directory*)((unsigned char*)opthdr + (type == MAGIC_PE64 ? 112 : 96));

    uint32_unpack((const void*)&datadir->virtual_address, &export_rva);
    uint32_unpack((const void*)&datadir->size, &export_size);
    uint16_unpack(&dll[pe_header_offset + 4 + 2], &nsections);
    uint16_unpack(&dll[pe_header_offset + 4 + 16], &secptr);
    secptr += pe_header_offset + 4 + 20;

    expptr = 0;
    for(i = 0; i < nsections; i++) {
      char sname[8];
      uint32 vaddr, vsize, fptr, secptr1 = secptr + 40 * i;
      uint32_unpack(&dll[secptr1 + 12], &vaddr);
      uint32_unpack(&dll[secptr1 + 16], &vsize);
      uint32_unpack(&dll[secptr1 + 20], &fptr);
      byte_copy(sname, 8, &dll[secptr1]);
      if(vaddr <= export_rva && vaddr + vsize > export_rva) {
        expptr = fptr + (export_rva - vaddr);
        if(export_rva + export_size > vaddr + vsize)
          export_size = vsize - (export_rva - vaddr);
        break;
      }
    }

    expdata = &dll[expptr];
    erva = expdata - export_rva;

    uint32_unpack(&expdata[24], &nexp);
    uint32_unpack(&expdata[32], &name_rvas);

    buffer_puts(buffer_1, "EXPORTS\n");
    //  buffer_putm(buffer_1, "LIBRARY ", dll_name, "\n", NULL);

    for(i = 0; i < nexp; i++) {
      uint32 name_rva;
      uint32_unpack(erva + name_rvas + i * 4, &name_rva);
      buffer_putm(buffer_1, "  ", erva + name_rva, " @ ", NULL);
      buffer_putulong(buffer_1, 1 + i);
      buffer_putnlflush(buffer_1);
    }
    mmap_unmap(dll, dllsz);
  }

  return 0;
}

