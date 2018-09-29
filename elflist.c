/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "lib/buffer.h"
#include "lib/mmap.h"
#include "lib/elf.h"
#include "lib/str.h"
#include <assert.h>
#include <stdlib.h>

void elf_dump_sections(uint8* base);
void elf_dump_segments(uint8* base);

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
  buffer_putnspace(buffer_1, 3);
  buffer_puts(buffer_1, "type");
  buffer_putnlflush(buffer_1);

  range_foreach(&sections, section) {
    uint32 name = ELF_GET(base, section, shdr, sh_name);
    uint64 addr = ELF_GET(base, section, shdr, sh_addr);
    uint64 size = ELF_GET(base, section, shdr, sh_size);
    uint64 offs = ELF_GET(base, section, shdr, sh_offset);
    uint64 align = ELF_GET(base, section, shdr, sh_addralign);
    uint32 type = ELF_GET(base, section, shdr, sh_type) % ELF_SHT_NUM;

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
    buffer_putspace(buffer_1);
    buffer_puts(buffer_1, elf_section_type(type));
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
  buffer_putnspace(buffer_1, 3);
  buffer_puts(buffer_1, "flags");
  buffer_putnlflush(buffer_1);

  range_foreach(&segments, segment) {
    uint64 paddr = ELF_GET(base, segment, phdr, p_paddr);
    uint64 vaddr = ELF_GET(base, segment, phdr, p_vaddr);
    uint64 filesz = ELF_GET(base, segment, phdr, p_filesz);
    uint64 memsz = ELF_GET(base, segment, phdr, p_memsz);
    uint32 flags = ELF_GET(base, segment, phdr, p_flags);

    if(!paddr && !vaddr && !filesz) continue;

    buffer_puts(buffer_1, "0x");
    buffer_putxlonglong0(buffer_1, paddr, ELF_BITS(base) / 4);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, vaddr, ELF_BITS(base) / 4);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, filesz, ELF_BITS(base) / 4);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, memsz, ELF_BITS(base) / 4);
    buffer_putm(buffer_1, " ", (flags & ELF_PF_R) ? "r" : "-", (flags & ELF_PF_W) ? "w" : "-", (flags & ELF_PF_W) ? "x" : "-");
    buffer_putnlflush(buffer_1);
  }
}
