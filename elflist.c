/* -*- Mode: C; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "lib/buffer.h"
#include "lib/elf.h"
#include "lib/mmap.h"
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
    elf_dump_dynamic(base);
    /*    elf_dump_imports(base);*/
  }

  mmap_unmap(base, filesize);

  return 0;
}
void
elf_dump_dynamic(uint8* base) {

  int di = elf_section_index(base, ".dynamic");
  range dyn;
  void* entry;
  const char* dynstrtab = NULL;
  static const char* const dynamic_types[] = {"NULL",       "NEEDED",     "PLTRELSZ",      "PLTGOT",         "HASH",
                                              "STRTAB",     "SYMTAB",     "RELA",          "RELASZ",         "RELAENT",
                                              "STRSZ",      "SYMENT",     "INIT",          "FINI",           "SONAME",
                                              "RPATH",      "SYMBOLIC",   "REL",           "RELSZ",          "RELENT",
                                              "PLTREL",     "DEBUG",      "TEXTREL",       "JMPREL",         "BIND_NOW",
                                              "INIT_ARRAY", "FINI_ARRAY", "INIT_ARRAYSZ",  "FINI_ARRAYSZ",   "RUNPATH",
                                              "FLAGS",      "ENCODING",   "PREINIT_ARRAY", "PREINIT_ARRAYSZ"};

  if(di == -1) return;

  dyn = elf_dynamic_section(base);

  range_foreach(&dyn, entry) {
    int64 tag = ELF_GET(base, entry, dyn, d_tag);

    if(tag == ELF_DT_STRTAB) {
      dynstrtab = base + ELF_GET(base, entry, dyn, d_un.d_val);
      break;
    }
  }

  range_foreach(&dyn, entry) {
    int64 tag = ELF_GET(base, entry, dyn, d_tag);
    uint64 val = ELF_GET(base, entry, dyn, d_un.d_val);

    if(tag >= ELF_DT_NUM) {
      buffer_puts(buffer_1, "0x");
      buffer_putxlonglong0(buffer_1, tag, ELF_BITS(base) / 4);
    } else {
      buffer_putspad(buffer_1, dynamic_types[tag % ELF_DT_NUM], 18);
    }

    if(tag == ELF_DT_NEEDED) {
      buffer_putspace(buffer_1);
      buffer_puts(buffer_1, &dynstrtab[val]);
    } else {
      buffer_puts(buffer_1, " 0x");
      buffer_putxlonglong0(buffer_1, val, ELF_BITS(base) / 4);
    }
    buffer_putnlflush(buffer_1);

    if(tag == ELF_DT_NULL) break;
  }
}

void
elf_dump_symbols(uint8* base, uint8* tab, size_t size, const char* stname) {
  range symtab;
  symtab.start = tab;
  symtab.end = tab + size;
  symtab.elem_size = ELF_BITS(base) == 64 ? sizeof(elf64_sym) : sizeof(elf32_sym);
  void* symbol;
  int si = elf_section_index(base, stname);
  const char* strtab = elf_section_offset(base, si);
  static const char* const binding_types[] = {"LOCAL", "GLOBAL", "WEAK"};
  static const char* const symbol_types[] = {"NOTYPE", "OBJECT", "FUNC", "SECTION", "FILE", "COMMON", "TLS"};

  range_foreach(&symtab, symbol) {
    uint32 name = ELF_GET(base, symbol, sym, st_name);
    uint64 value = ELF_GET(base, symbol, sym, st_value);
    uint64 size = ELF_GET(base, symbol, sym, st_size);
    uint8 info = ELF_GET(base, symbol, sym, st_info);

    if(!name) continue;

    buffer_putspad(buffer_1, &(strtab[name]), 32);
    /*buffer_puts(buffer_1, "0x");
    buffer_putxlong0(buffer_1, name, 8);
   */ buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, value, ELF_BITS(base) / 4);
    buffer_puts(buffer_1, " 0x");
    buffer_putxlonglong0(buffer_1, size, ELF_BITS(base) / 4);
    buffer_putspace(buffer_1);
    buffer_puts(buffer_1, binding_types[ELF_ELF32_ST_BIND(info)]);
    buffer_putspace(buffer_1);
    buffer_puts(buffer_1, symbol_types[ELF_ELF32_ST_TYPE(info)]);

    buffer_putnlflush(buffer_1);
  }
}

void
elf_dump_sections(uint8* base) {
  int i, n;
  range sections = elf_section_headers(base);
  void* section;

  buffer_putspad(buffer_1, "section name", 16);
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

    buffer_putspad(buffer_1, &(elf_shstrtab(base)[name]), 16);
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

    if(type == ELF_SHT_SYMTAB || type == ELF_SHT_DYNSYM) {
      elf_dump_symbols(base, base + offs, size, type == ELF_SHT_SYMTAB ? ".strtab" : ".dynstr");
    }
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
    buffer_putm(
        buffer_1, " ", (flags & ELF_PF_R) ? "r" : "-", (flags & ELF_PF_W) ? "w" : "-", (flags & ELF_PF_W) ? "x" : "-");
    buffer_putnlflush(buffer_1);
  }
}
