#include "../pe.h"
#include <string.h>

static int
within(uint64 off, uint64 len, size_t size) {
  return off <= size && len <= size - off;
}

/* file offset of an RVA, or -1 if no section holds it or the offset is outside the file */
static int64
rva_off(const uint8* x, size_t size, const pe_section_header* sec, unsigned n, uint32 rva) {
  unsigned i;

  for(i = 0; i < n; i++) {
    uint32 span = sec[i].virtual_size > sec[i].size_of_raw_data ? sec[i].virtual_size : sec[i].size_of_raw_data;

    if(rva >= sec[i].virtual_address && rva - sec[i].virtual_address < span) {
      uint64 off = (uint64)sec[i].pointer_to_raw_data + (rva - sec[i].virtual_address);

      return off < size ? (int64)off : -1;
    }
  }

  return -1;
}

/* a string at an RVA that ends inside the file */
static int
string_ok(const uint8* x, size_t size, const pe_section_header* sec, unsigned n, uint32 rva) {
  int64 off = rva_off(x, size, sec, n, rva);

  return off >= 0 && memchr(x + off, 0, size - (size_t)off) != NULL;
}

/*
 * Checks that a PE image of the given size is consistent enough for the
 * functions of this module (and the programs that list it) to walk it: the DOS
 * and NT headers, the optional header with its data directories, the section
 * table and the sections, the export directory and the import table with the
 * names in them. Returns NULL if so, otherwise a text that says what is wrong.
 */
const char*
pe_check(const void* base, size_t size) {
  const uint8* x = (const uint8*)base;
  const pe_dos_header* dos;
  const pe_coff_header* coff;
  const pe_section_header* sec;
  uint64 lfanew, opt_off, sec_off;
  unsigned optsize, nsec, ndir = 0, i, fixed, is64;
  const pe_data_directory* dir = NULL;

  if(base == NULL || size < sizeof(pe_dos_header))
    return "not a PE file";

  dos = (const pe_dos_header*)x;

  if(dos->e_magic != 0x5a4d)
    return "not a PE file";

  lfanew = dos->e_lfanew;

  if(!within(lfanew, 4 + sizeof(pe_coff_header), size))
    return "the PE header is outside the file";

  if(*(const uint32*)(x + lfanew) != PE_NT_SIGNATURE)
    return "no PE signature";

  coff = (const pe_coff_header*)(x + lfanew + 4);
  optsize = coff->size_of_optional_header;
  opt_off = lfanew + 4 + sizeof(pe_coff_header);

  if(!within(opt_off, optsize, size) || optsize < 2)
    return "the optional header is outside the file";

  switch(*(const uint16*)(x + opt_off)) {
    case PE_MAGIC_PE32: is64 = 0; fixed = 96; break;
    case PE_MAGIC_PE64: is64 = 1; fixed = 112; break;
    default: return "unknown optional header";
  }

  if(optsize < fixed)
    return "the optional header is too small";

  {
    uint32 num = *(const uint32*)(x + opt_off + fixed - 4);

    ndir = (optsize - fixed) / sizeof(pe_data_directory);

    if(num < ndir)
      ndir = num;
    if(ndir > PE_NUMBEROF_DIRECTORY_ENTRIES)
      ndir = PE_NUMBEROF_DIRECTORY_ENTRIES;

    dir = (const pe_data_directory*)(x + opt_off + fixed);
  }

  nsec = coff->number_of_sections;
  sec_off = opt_off + optsize;

  if(!within(sec_off, (uint64)nsec * sizeof(pe_section_header), size))
    return "the section table is outside the file";

  sec = (const pe_section_header*)(x + sec_off);

  for(i = 0; i < nsec; i++)
    if(sec[i].size_of_raw_data && !within(sec[i].pointer_to_raw_data, sec[i].size_of_raw_data, size))
      return "a section is outside the file";

  /* the export directory */
  if(ndir > PE_DIRECTORY_ENTRY_EXPORT && dir[PE_DIRECTORY_ENTRY_EXPORT].virtual_address) {
    int64 off = rva_off(x, size, sec, nsec, dir[PE_DIRECTORY_ENTRY_EXPORT].virtual_address);
    const pe_export_directory* ex;
    int64 names, funcs, ords;

    if(off < 0 || !within(off, sizeof(pe_export_directory), size))
      return "the export directory is outside the file";

    ex = (const pe_export_directory*)(x + off);

    if(!string_ok(x, size, sec, nsec, ex->name))
      return "the name of the library is outside the file";

    if(ex->number_of_names > size / 4 || ex->number_of_functions > size / 4)
      return "the export directory is damaged";

    if(ex->number_of_names) {
      names = rva_off(x, size, sec, nsec, ex->address_of_names);
      ords = rva_off(x, size, sec, nsec, ex->address_of_name_ordinals);

      if(names < 0 || !within(names, (uint64)ex->number_of_names * 4, size) || ords < 0 ||
         !within(ords, (uint64)ex->number_of_names * 2, size))
        return "the export name table is outside the file";

      for(i = 0; i < ex->number_of_names; i++)
        if(!string_ok(x, size, sec, nsec, *(const uint32*)(x + names + (uint64)i * 4)))
          return "an export name is outside the file";
    }

    if(ex->number_of_functions) {
      funcs = rva_off(x, size, sec, nsec, ex->address_of_functions);

      if(funcs < 0 || !within(funcs, (uint64)ex->number_of_functions * 4, size))
        return "the export address table is outside the file";
    }
  }

  /* the import table */
  if(ndir > PE_DIRECTORY_ENTRY_IMPORT && dir[PE_DIRECTORY_ENTRY_IMPORT].virtual_address) {
    int64 off = rva_off(x, size, sec, nsec, dir[PE_DIRECTORY_ENTRY_IMPORT].virtual_address);
    uint64 n;

    if(off < 0)
      return "the import table is outside the file";

    for(n = 0;; n++) {
      const pe_import_descriptor* d;
      int64 t;
      uint32 thunk_rva;
      unsigned tsize = is64 ? 8 : 4;
      uint64 j;

      if(!within((uint64)off + n * sizeof(pe_import_descriptor), sizeof(pe_import_descriptor), size))
        return "the import table is outside the file";

      d = (const pe_import_descriptor*)(x + off + n * sizeof(pe_import_descriptor));

      if(d->name == 0 && d->first_thunk == 0 && d->original_first_thunk == 0)
        break;

      if(!string_ok(x, size, sec, nsec, d->name))
        return "the name of an imported library is outside the file";

      thunk_rva = d->original_first_thunk ? d->original_first_thunk : d->first_thunk;

      if(thunk_rva && (t = rva_off(x, size, sec, nsec, thunk_rva)) >= 0) {
        for(j = 0;; j++) {
          uint64 entry;

          if(!within((uint64)t + j * tsize, tsize, size))
            return "an import thunk table is outside the file";

          entry = is64 ? *(const uint64*)(x + t + j * tsize) : *(const uint32*)(x + t + j * tsize);

          if(entry == 0)
            break;

          /* the high bit is set for an import by ordinal */
          if(!(entry >> (is64 ? 63 : 31)) && !string_ok(x, size, sec, nsec, (uint32)(entry + 2)))
            return "the name of an import is outside the file";
        }
      }
    }
  }

  return NULL;
}
