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

#include "lib/byte.h"
#include "lib/buffer.h"
#include "lib/mmap.h"
#include "lib/open.h"
#include "lib/pe.h"
#include "lib/uint32.h"
#include "lib/str.h"
#include "lib/fmt.h"

int
main(int argc, char* argv[]) {
  int optarg;

  for(optarg = 1; optarg < argc; ++optarg) {
    char *dll, *filename, *dll_name;
    size_t dllsz;
    uint32 i, *name_rvas, nexp, num_entries;
    pe_data_directory* datadir;
    pe_export_directory* expdata;
    pe32_opt_header* opt_hdr_32;
    pe_type type;

    filename = argv[optarg];

    dll = (char*)mmap_read(filename, &dllsz);
    if(dll == NULL)
      return 1;

    dll_name = str_basename(filename);

    opt_hdr_32 = pe_header_opt(dll);

    type = uint16_get(&opt_hdr_32->magic);

    datadir = pe_get_datadir(dll, &num_entries);

    if(num_entries < 1) /* no exports */
      return 1;

    expdata = pe_rva2ptr(dll, uint32_get(&datadir->virtual_address));

    nexp = uint32_get(&expdata->number_of_names);
    name_rvas = pe_rva2ptr(dll, uint32_get(&expdata->address_of_names));

    buffer_puts(buffer_1, "EXPORTS\n");
    (void)dll_name;
    /* buffer_putm_internal(buffer_1, "LIBRARY ", dll_name, "\n", 0); */

    for(i = 0; i < nexp; i++) {
      buffer_putm_internal(buffer_1, "  ", pe_rva2ptr(dll, uint32_get(&name_rvas[i])), " @ ", 0);
      buffer_putulong(buffer_1, 1 + i);
      buffer_putnlflush(buffer_1);
    }
    mmap_unmap(dll, dllsz);
  }

  return 0;
}
