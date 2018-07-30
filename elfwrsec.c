#include "lib/buffer.h"
#include "lib/mmap.h"
#include <elf.h>

static const char* const s_flags[] = {"SHF_WRITE", "SHF_ALLOC", "SHF_EXECINSTR", "SHF_UNKNOWN_3", "SHF_MERGE", "SHF_STRINGS", "SHF_INFO_LINK", "SHF_LINK_ORDER", "SHF_OS_NONCONFORMING", "SHF_GROUP", "SHF_TLS", "SHF_COMPRESSED", 0 };
static const char* const p_flags[] = { "PF_X", "PF_W", "PF_R", 0 };
static size_t size;
static void* base;

const char*
get_p_type(int type) {
  static const char* const p_types[] = { "PT_NULL   ", "PT_LOAD   ", "PT_DYNAMIC", "PT_INTERP ", "PT_NOTE   ", "PT_SHLIB  ", "PT_PHDR   ", "PT_TLS    ", "PT_NUM    " };

   if(type < (int)(sizeof(p_types) / sizeof(p_types[0])))
     return p_types[type];

   if(type >= 0x6474e550 && type <= 0x6474e552) {
       static const char* const gnu_p_types[] = { "PT_GNU_EH_FRAME", "PT_GNU_STACK", "PT_GNU_RELRO" };
       return gnu_p_types[type % 3];
   }

   return "(unknown)";
}

void
print_flags(const char* const flagset[], int flags) {
  int prev = 0;

  for(size_t shift = 0; flagset[shift];  ++shift) {
    if(flags & (1 << shift)) {
      if(prev) buffer_puts(buffer_1, " | ");
      buffer_puts(buffer_1, flagset[shift]);
      ++prev;
    }
  }
}

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
    
    buffer_putm(buffer_1, ": type=", get_p_type(phdr[i].p_type));
    buffer_puts(buffer_1, ", offset="); buffer_putxlong0(buffer_1, phdr[i].p_offset, 8);
    buffer_puts(buffer_1, ", vaddr="); buffer_putxlong0(buffer_1, phdr[i].p_vaddr, 8);
    buffer_puts(buffer_1, ", paddr="); buffer_putxlong0(buffer_1, phdr[i].p_paddr, 8);
    buffer_puts(buffer_1, ", filesz="); buffer_putulong(buffer_1, phdr[i].p_filesz);
    buffer_puts(buffer_1, ", memsz="); buffer_putulong(buffer_1, phdr[i].p_memsz);
    buffer_puts(buffer_1, ", flags="); print_flags(p_flags, phdr[i].p_flags);

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
    buffer_puts(buffer_1, " '");
    buffer_putspad(buffer_1, sh_strtab_p + shdr[i].sh_name, 16);
    buffer_puts(buffer_1, "', flags="); print_flags(s_flags, shdr[i].sh_flags);
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
