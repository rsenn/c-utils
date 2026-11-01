#include "../elf.h"
#include <string.h>

/* offset and length lie inside size bytes */
static int
within(uint64 off, uint64 len, size_t size) {
  return off <= size && len <= size - off;
}

/* a string table: it ends in a NUL byte */
static int
strtab_ok(const uint8* x, size_t size, uint64 off, uint64 len) {
  return len > 0 && within(off, len, size) && x[off + len - 1] == 0;
}

/*
 * Checks that an ELF image of the given size is consistent enough for the
 * functions of this module (and the programs that print its contents) to
 * walk it: the header, the program and section header tables, the sections,
 * the string tables, the names in symbol tables and the dynamic section all
 * lie inside the image. Returns NULL if so, otherwise a text that says what
 * is wrong. The file is read in the byte order of the host, a file of the
 * other byte order is refused.
 */
const char*
elf_check(const void* base, size_t size) {
  const uint8* x = (const uint8*)base;
  const uint16 one = 1;
  int is64, i;
  uint64 phoff, shoff;
  unsigned phentsize, phnum, shentsize, shnum, shstrndx;

  if(base == NULL || size < ELF_EI_NIDENT || memcmp(x, ELF_ELFMAG, ELF_SELFMAG) != 0)
    return "not an ELF file";

  if(x[4] != 1 && x[4] != 2)
    return "unknown ELF class";

  is64 = x[4] == 2;

  if(x[5] != ((*(const uint8*)&one) ? 1 : 2))
    return "the byte order of the file is not the one of this machine";

  if(size < (is64 ? sizeof(elf64_ehdr) : sizeof(elf32_ehdr)))
    return "truncated ELF header";

  if(is64) {
    const elf64_ehdr* h = (const elf64_ehdr*)x;

    phoff = h->e_phoff;
    shoff = h->e_shoff;
    phentsize = h->e_phentsize;
    phnum = h->e_phnum;
    shentsize = h->e_shentsize;
    shnum = h->e_shnum;
    shstrndx = h->e_shstrndx;
  } else {
    const elf32_ehdr* h = (const elf32_ehdr*)x;

    phoff = h->e_phoff;
    shoff = h->e_shoff;
    phentsize = h->e_phentsize;
    phnum = h->e_phnum;
    shentsize = h->e_shentsize;
    shnum = h->e_shnum;
    shstrndx = h->e_shstrndx;
  }

  /* program headers */
  if(phnum) {
    if(phentsize < (is64 ? sizeof(elf64_phdr) : sizeof(elf32_phdr)))
      return "program header entries are too small";

    if(!within(phoff, (uint64)phnum * phentsize, size))
      return "the program header table is outside the file";

    for(i = 0; i < (int)phnum; i++) {
      const uint8* p = x + phoff + (uint64)i * phentsize;
      uint64 off = is64 ? ((const elf64_phdr*)p)->p_offset : ((const elf32_phdr*)p)->p_offset;
      uint64 fsz = is64 ? ((const elf64_phdr*)p)->p_filesz : ((const elf32_phdr*)p)->p_filesz;

      if(!within(off, fsz, size))
        return "a segment is outside the file";
    }
  }

  /* section headers */
  if(shnum == 0 && shoff == 0)
    return NULL;

  if(shnum == 0)
    return "section headers without a count";

  if(shentsize < (is64 ? sizeof(elf64_shdr) : sizeof(elf32_shdr)))
    return "section header entries are too small";

  if(!within(shoff, (uint64)shnum * shentsize, size))
    return "the section header table is outside the file";

  if(shstrndx >= shnum)
    return "the section name table does not exist";

#define SH(n, field) (is64 ? ((const elf64_shdr*)(x + shoff + (uint64)(n)*shentsize))->field : ((const elf32_shdr*)(x + shoff + (uint64)(n)*shentsize))->field)

  if(SH(shstrndx, sh_type) != ELF_SHT_STRTAB || !strtab_ok(x, size, SH(shstrndx, sh_offset), SH(shstrndx, sh_size)))
    return "the section name table is damaged";

  for(i = 0; i < (int)shnum; i++) {
    uint32 type = SH(i, sh_type);
    uint64 off = SH(i, sh_offset), sz = SH(i, sh_size);
    uint32 link = SH(i, sh_link);

    if(SH(i, sh_name) >= SH(shstrndx, sh_size))
      return "a section name is outside the name table";

    if(type == 0 || type == ELF_SHT_NOBITS)
      continue;

    if(!within(off, sz, size))
      return "a section is outside the file";

    if(type == ELF_SHT_STRTAB && sz > 0 && x[off + sz - 1] != 0)
      return "a string table does not end";

    if(type == ELF_SHT_SYMTAB || type == ELF_SHT_DYNSYM || type == ELF_SHT_DYNAMIC) {
      uint64 stroff, strsz, n, j, entsz = type == ELF_SHT_DYNAMIC ? (is64 ? sizeof(elf64_dyn) : sizeof(elf32_dyn)) : (is64 ? sizeof(elf64_sym) : sizeof(elf32_sym));

      if(link >= shnum || SH(link, sh_type) != ELF_SHT_STRTAB)
        return "a symbol table has no string table";

      stroff = SH(link, sh_offset);
      strsz = SH(link, sh_size);

      if(!strtab_ok(x, size, stroff, strsz))
        return "the string table of a symbol table is damaged";

      n = sz / entsz;

      for(j = 0; j < n; j++) {
        const uint8* e = x + off + j * entsz;

        if(type == ELF_SHT_DYNAMIC) {
          int64 tag = is64 ? (int64)((const elf64_dyn*)e)->d_tag : (int64)(int32)((const elf32_dyn*)e)->d_tag;
          uint64 val = is64 ? ((const elf64_dyn*)e)->d_un.d_val : ((const elf32_dyn*)e)->d_un.d_val;

          if(tag == 0)
            break;

          /* DT_NEEDED, DT_SONAME, DT_RPATH, DT_RUNPATH name something in the table */
          if((tag == 1 || tag == 14 || tag == 15 || tag == 29) && val >= strsz)
            return "a name in the dynamic section is outside the string table";
        } else {
          uint32 name = is64 ? ((const elf64_sym*)e)->st_name : ((const elf32_sym*)e)->st_name;

          if(name >= strsz)
            return "a symbol name is outside the string table";
        }
      }
    }
  }

  return NULL;
}
