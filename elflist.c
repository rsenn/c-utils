/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "lib/buffer.h"
#include "lib/mmap.h"
#include "lib/elf.h"
#include "lib/str.h"
#include <assert.h>
#include <stdlib.h>

/*void elf_dump_sections(uint8* base);

uint32
elf_offset2rva(uint8* base, int64 off) {
  int i, n;
  elf_section_header* sections = elf_header_sections(base, &n);

  if((uint32)off < sections[0].pointer_to_raw_data) return off;

  for(i = 0; i < n; i++) {
    uint32 start, size;
    uint32_unpack(&sections[i].pointer_to_raw_data, &start);
    uint32_unpack(&sections[i].size_of_raw_data, &size);

    if((uint32)off >= start && (uint32)off < start + size) return (uint32)off - start + sections[i].virtual_address;
  }
  return 0;
}

void
elf_print_data_directory(buffer* b, elf_data_directory* data_dir) {
  buffer_puts(b, "virtual_address: 0x");
  buffer_putxlonglong0(b, data_dir->virtual_address, ELF_BITS(base) / 4);
  buffer_puts(b, " size: 0x");
  buffer_putxlonglong0(b, data_dir->size, ELF_BITS(base) / 4);
  buffer_putnlflush(b);
}

void
elf_print_export_directory(buffer* b, elf_export_directory* export_dir) {
  buffer_puts(b, "characteristics: ");
  buffer_putxlonglong0(b, export_dir->characteristics, ELF_BITS(base) / 4);
  buffer_puts(b, "\ntime_date_stamp: ");
  buffer_putxlonglong0(b, export_dir->time_date_stamp, ELF_BITS(base) / 4);
  buffer_puts(b, "\nmajor_version: ");
  buffer_putxlonglong0(b, export_dir->major_version, ELF_BITS(base) / 4);
  buffer_puts(b, "\nminor_version: ");
  buffer_putxlonglong0(b, export_dir->minor_version, ELF_BITS(base) / 4);
  buffer_puts(b, "\nname: ");
  buffer_putxlonglong0(b, export_dir->name, ELF_BITS(base) / 4);
  buffer_puts(b, "\nbase: ");
  buffer_putxlonglong0(b, export_dir->base, ELF_BITS(base) / 4);
  buffer_puts(b, "\nnumber_of_functions: ");
  buffer_putxlonglong0(b, export_dir->number_of_functions, ELF_BITS(base) / 4);
  buffer_puts(b, "\nnumber_of_names: ");
  buffer_putxlonglong0(b, export_dir->number_of_names, ELF_BITS(base) / 4);
  buffer_puts(b, "\naddress_of_functions: ");
  buffer_putxlonglong0(b, export_dir->address_of_functions, ELF_BITS(base) / 4);
  buffer_puts(b, "\naddress_of_names: ");
  buffer_putxlonglong0(b, export_dir->address_of_names, ELF_BITS(base) / 4);
  buffer_puts(b, "\naddress_of_name_ordinals: ");
  buffer_putxlonglong0(b, export_dir->address_of_name_ordinals, ELF_BITS(base) / 4);
  buffer_putnlflush(b);
}

void
elf_dump_exports(uint8* base) {
  int i;
  elf_data_directory* export_dir = &elf_header_datadir(base)[PE_DIRECTORY_ENTRY_EXPORT];
  elf_section_header* text;
  uint32 fnaddr, *nameptr, *fnptr, mintextptr, maxtextptr;

  elf_export_directory* exports = elf_rva2ptr(base, export_dir->virtual_address);
  // elf_print_export_directory(buffer_1, exports);

  if(!(text = elf_get_section(base, ".text"))) {
    buffer_putsflush(buffer_2, "no .text section\n");
    return;
  }

  nameptr = (uint32*)elf_rva2ptr(base, exports->address_of_names);
  fnptr = (uint32*)elf_rva2ptr(base, exports->address_of_functions);
  mintextptr = text->virtual_address;
  maxtextptr = mintextptr + text->size_of_raw_data;

  for(i = 0; i < exports->number_of_names; i++) {
    fnaddr = fnptr[i];
    if(mintextptr < fnaddr && fnaddr < maxtextptr) {
      buffer_puts(buffer_1, elf_rva2ptr(base, nameptr[i]));
      buffer_putnlflush(buffer_1);
    }
  }
}

void
elf_dump_imports(uint8* base) {
  int i, j, n;
  elf_data_directory* import_dir = &elf_header_datadir(base)[PE_DIRECTORY_ENTRY_IMPORT];
  elf_import_descriptor* imports = elf_rva2ptr(base, import_dir->virtual_address);

  //n = import_dir->size / sizeof(elf_import_descriptor) - 1;
  
  buffer_putspad(buffer_1, "symbol", 64);
  buffer_putspace(buffer_1);
  buffer_putspad(buffer_1, "ordinal", 5);
  buffer_putnspace(buffer_1, 3);
  buffer_puts(buffer_1, "dll");
  buffer_putnlflush(buffer_1);

  for(i = 0; imports[i].original_first_thunk; ++i) {
    const char* name = elf_rva2ptr(base, uint32_get(&imports[i].name));
    
    if(name[0] == '\0') break;

    uint32 thunk = uint32_get(&imports[i].original_first_thunk);

    for(j = 0;; ++j) {
      const char* sym = NULL;
      uint16 ordinal;
      int64 rva;

      if(!(rva = elf_thunk(base, thunk, j))) break;

      if(rva < 0) {
        rva <<= 1;
        rva >>= 1;
        ordinal = rva;
      } else {
        elf_import_by_name* name_import;

        if((name_import = elf_rva2ptr(base, rva))) {
          sym = name_import->name;
          ordinal = uint16_get(&name_import->hint);
        }
      }

      buffer_putspad(buffer_1, sym ? sym : "", 64);
      buffer_putnspace(buffer_1, 3);
      buffer_putulong0(buffer_1, ordinal, 5);
      buffer_putnspace(buffer_1, 3);
      buffer_puts(buffer_1, name);
      buffer_putnlflush(buffer_1);
    }
  }
}
i
*/
int
main(int argc, char** argv) {
  uint8* base = NULL;
  size_t filesize;

  if(argc < 2) {
    buffer_putm(buffer_1, "Usage: ", str_basename(argv[0]), " XXX.dll\n");
    buffer_flush(buffer_1);
    return 0;
  }

  base = (uint8*)mmap_private(argv[1], &filesize);

  {
    char elf64 = elf_header_ident(base)[ELF_EI_CLASS] == ELF_ELFCLASS64;


    uint64 x = ELF_GET(base, base, ehdr, e_shentsize);
  elf_dump_sections(base);
  elf_dump_segments(base);
/*    elf_dump_imports(base);*/
  }

  mmap_unmap(base, filesize);

  return 0;
}

void
elf_dump_sections(uint8* base) {
  int i, n;
  range sections = elf_section_headers(base);
  void* section;

  buffer_putspad(buffer_1, "section name", 32);
  buffer_putspace(buffer_1);
  buffer_putspad(buffer_1, "addr", ELF_BITS(base) / 4);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "size", ELF_BITS(base) / 4);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "offset", ELF_BITS(base) / 4);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "align", ELF_BITS(base) / 4);
  buffer_putnlflush(buffer_1);

  range_foreach(&sections, section) {
    uint32 name = ELF_GET(base, section, shdr, sh_name);
    uint64 addr = ELF_GET(base, section, shdr, sh_addr);
    uint64 size = ELF_GET(base, section, shdr, sh_size);
    uint64 offs = ELF_GET(base, section, shdr, sh_offset);
    uint64 align = ELF_GET(base, section, shdr, sh_addralign);

    if(!name && !addr && !size) continue;

    buffer_putspad(buffer_1, 
    &(elf_strtab(base)[name]), 32);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, addr, ELF_BITS(base) / 4);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, size, ELF_BITS(base) / 4);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, offs, ELF_BITS(base) / 4);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, align, ELF_BITS(base) / 4);
    /*
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, sections[i].size_of_raw_data, ELF_BITS(base) / 4);
    jbuffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, sections[i].pointer_to_raw_data, ELF_BITS(base) / 4);*/
    buffer_putnlflush(buffer_1);
  }
}
void
elf_dump_segments(uint8* base) {
  int i, n;
  range segments = elf_program_headers(base);
  void* segment;

  buffer_putspad(buffer_1, "paddr", ELF_BITS(base) / 4);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "vaddr", ELF_BITS(base) / 4);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "filesz", ELF_BITS(base) / 4);
  buffer_putnspace(buffer_1, 3);
  buffer_putspad(buffer_1, "memsz", ELF_BITS(base) / 4);
  buffer_putnlflush(buffer_1);

  range_foreach(&segments, segment) {
    uint64 paddr = ELF_GET(base, segment, phdr, p_paddr);
    uint64 vaddr = ELF_GET(base, segment, phdr, p_vaddr);
    uint64 filesz = ELF_GET(base, segment, phdr, p_filesz);
    uint64 memsz = ELF_GET(base, segment, phdr, p_memsz);

    if(!paddr && !vaddr && !filesz) continue;

    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, paddr, ELF_BITS(base) / 4);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, vaddr, ELF_BITS(base) / 4);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, filesz, ELF_BITS(base) / 4);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, memsz, ELF_BITS(base) / 4);
    buffer_putnlflush(buffer_1);
  }
}
