/*   Copyright (C) 1999-2000 Free
   Software Foundation, Inc.

     This file is part of GNU libtool.

     This program is free software; you
   can redistribute it and/or modify it
   under the terms of the GNU General
   Public License as published by the
   Free Software Foundation; either
   version 2 of the License, or (at your
   option) any later version.

     This program is distributed in the
   hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even
   the implied warranty of
     MERCHANTABILITY or FITNESS FOR A
   PARTICULAR PURPOSE.  See the GNU
   General Public License for more
   details.

     You should have received a copy of
   the GNU General Public License
     auint32 with this program; if not,
   write to the Free Software
     Foundation, Inc., 59 Temple Place -
   Suite 330, Boston, MA 02111-1307,
   USA.
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
  int unix_optarg;

  for(unix_optarg = 1; unix_optarg < argc; ++unix_optarg) {
    char *base, *filename, *dll_name;
    size_t dllsz;
    uint32 i, *name_rvas, nexp, num_entries;
    uint16* ord_rvas;
    pe_data_directory* datadir;
    pe_export_directory* exports;
    pe32_opt_header* opt_hdr_32;
    pe_type type;

    filename = argv[unix_optarg];

    base = (char*)mmap_read(filename, &dllsz);
    if(base == NULL)
      return 1;

    dll_name = pe_dllname((uint8*)base);
    if(dll_name == NULL)
      dll_name = str_basename(filename);

    opt_hdr_32 = pe_header_opt(base);

    type = uint16_get(&opt_hdr_32->magic);

    datadir = pe_get_datadir(base, &num_entries);

    if(num_entries < 1) /* no exports */
      return 1;

    exports = pe_rva2ptr(base, uint32_get(&datadir->virtual_address));

    nexp = uint32_get(&exports->number_of_names);
    name_rvas = pe_rva2ptr(base, uint32_get(&exports->address_of_names));
    ord_rvas = pe_rva2ptr(base, uint32_get(&exports->address_of_name_ordinals));

    buffer_putm_internal(buffer_1, "LIBRARY ", dll_name, "\n", 0);
    buffer_puts(buffer_1, "EXPORTS\n");
    (void)dll_name;
    /* buffer_putm_internal(buffer_1,
     * "LIBRARY ", dll_name, "\n", 0);
     */

    for(i = 0; i < nexp; i++) {
      buffer_putm_internal(buffer_1, "  ", pe_rva2ptr(base, uint32_get(&name_rvas[i])), " @ ", 0);
      buffer_putulong(buffer_1, uint16_get(&ord_rvas[i]));
      buffer_putnlflush(buffer_1);
    }
    mmap_unmap(base, dllsz);
  }

  return 0;
}
