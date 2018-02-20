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
#include "pe.h"

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
    pe32_opt_header* opt_hdr_32;
    pe64_opt_header* opt_hdr_64;
    pe_data_directory* datadir;
    pe_type* type;

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

    opt_hdr_32 = (pe32_opt_header*)&dll[pe_header_offset + 4 + 20];
    opt_hdr_64 = (pe64_opt_header*)&dll[pe_header_offset + 4 + 20];

    type = opt_hdr_32->signature;

/*    buffer_puts(buffer_2, "opt_hdr directory: ");
    buffer_putulong(buffer_2, (unsigned char*)&opt_hdr_32->number_of_rva_and_sizes - (unsigned char*)opt_hdr_32);
    buffer_putnlflush(buffer_2);
    buffer_puts(buffer_2, "opt_hdr_64 directory: ");
    buffer_putulong(buffer_2, (unsigned char*)&opt_hdr_64->number_of_rva_and_sizes - (unsigned char*)opt_hdr_64);
    buffer_putnlflush(buffer_2);
*/

    uint32_unpack(&dll[opthdr_ofs + (type == MAGIC_PE64 ? 108 : 92)], &num_entries);

    if(num_entries < 1)  /* no exports */
      return 1;

    datadir = (pe_data_directory*)((unsigned char*)opt_hdr_32 + (type == MAGIC_PE64 ? 112 : 96));

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

