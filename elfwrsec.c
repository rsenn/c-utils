#include "lib/buffer.h"
#include "lib/mmap.h"
#include <elf.h>

static size_t size;
static void* base;

int
print_phdr64() {

  static const char* const p_types[] = { "PT_NULL   ", "PT_LOAD   ", "PT_DYNAMIC", "PT_INTERP ", "PT_NOTE   ", "PT_SHLIB  ", "PT_PHDR   ", "PT_TLS    ", "PT_NUM    " };
  Elf64_Ehdr* ehdr = (Elf64_Ehdr*)base;
  Elf64_Phdr* phdr = (Elf64_Phdr*)((char*)base + ehdr->e_phoff);
  int phnum = ehdr->e_phnum;

//  Elf64_Phdr* ph_strtab = &phdr[ehdr->e_phstrndx];
//  const char* const ph_strtab_p = (char*)base + ph_strtab->ph_offset;

  for(int i = 0; i < phnum; ++i) {
    buffer_putlong(buffer_1, i);
    
    buffer_putm(buffer_1, ": type=", p_types[phdr[i].p_type]);
    buffer_puts(buffer_1, ", offset="); buffer_putxlong0(buffer_1, phdr[i].p_offset, 10);
    buffer_puts(buffer_1, ", vaddr="); buffer_putxlong0(buffer_1, phdr[i].p_vaddr, 10);
    buffer_puts(buffer_1, ", paddr="); buffer_putxlong0(buffer_1, phdr[i].p_paddr, 10);
    buffer_puts(buffer_1, ", filesz="); buffer_putxlong0(buffer_1, phdr[i].p_filesz, 10);
    buffer_puts(buffer_1, ", memsz="); buffer_putxlong0(buffer_1, phdr[i].p_memsz, 10);

    buffer_putnlflush(buffer_1);
  }

  return 0;
}

int
print_shdr64() {

  Elf64_Ehdr* ehdr = (Elf64_Ehdr*)base;
  Elf64_Shdr* shdr = (Elf64_Shdr*)((char*)base + ehdr->e_shoff);
  int shnum = ehdr->e_shnum;

  Elf64_Shdr* sh_strtab = &shdr[ehdr->e_shstrndx];
  const char* const sh_strtab_p = (char*)base + sh_strtab->sh_offset;

  for(int i = 0; i < shnum; ++i) {
    buffer_putlong(buffer_1, i);
    buffer_puts(buffer_1, ": ");
    buffer_putlong(buffer_1, shdr[i].sh_name);
    buffer_putm(buffer_1, " '", sh_strtab_p + shdr[i].sh_name, "'");
    buffer_putnlflush(buffer_1);
  }

  return 0;
}

int
process32(Elf32_Ehdr* hdr) {
  return 0;
}

int
process64(Elf64_Ehdr* hdr) {

  char* ptr = (char*)hdr + hdr->e_shoff;

  for(int i = 0; i < hdr->e_shnum; ++i) { Elf64_Shdr* shdr = (Elf64_Shdr*)(ptr + (i * hdr->e_shentsize)); }
  print_phdr64();
  print_shdr64();

  return 0;
}

int
main(int argc, char* argv[]) {

  base = mmap_private(argv[1], &size);

  Elf64_Ehdr* header = base;

  int ret = (header->e_ident[EI_CLASS] == ELFCLASS64 ? process64(base) : process32(base));

  mmap_unmap(base, size);

  return ret;
}
