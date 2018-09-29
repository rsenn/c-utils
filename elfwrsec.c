#include "lib/buffer.h"
#include "lib/elf.h"
#include "lib/mmap.h"

static const char* const s_flags[] = {"SHF_WRITE",
                                      "SHF_ALLOC",
                                      "SHF_EXECINSTR",
                                      "SHF_UNKNOWN_3",
                                      "SHF_MERGE",
                                      "SHF_STRINGS",
                                      "SHF_INFO_LINK",
                                      "SHF_LINK_ORDER",
                                      "SHF_OS_NONCONFORMING",
                                      "SHF_GROUP",
                                      "SHF_TLS",
                                      "SHF_COMPRESSED",
                                      0};
static const char* const p_flags[] = {"PF_X", "PF_W", "PF_R", 0};
static size_t size;
static void* base;
static const char* section = ".rodata";

const char*
get_p_type(int type) {
  static const char* const p_types[] = {"PT_NULL   ",
                                        "PT_LOAD   ",
                                        "PT_DYNAMIC",
                                        "PT_INTERP ",
                                        "PT_NOTE   ",
                                        "PT_SHLIB  ",
                                        "PT_PHDR   ",
                                        "PT_TLS    ",
                                        "PT_NUM    "};

  if(type < (int)(sizeof(p_types) / sizeof(p_types[0]))) return p_types[type];

  if(type >= 0x6474e550 && type <= 0x6474e552) {
    static const char* const gnu_p_types[] = {"PT_GNU_EH_FRAME", "PT_GNU_STACK", "PT_GNU_RELRO"};
    return gnu_p_types[type % 3];
  }

  return "(unknown)";
}

void
print_flags(const char* const flagset[], int flags) {
  size_t shift;
  int prev = 0;

  for(shift = 0; flagset[shift]; ++shift) {
    if(flags & (1 << shift)) {
      if(prev) buffer_puts(buffer_1, " | ");
      buffer_puts(buffer_1, flagset[shift]);
      ++prev;
    }
  }
}

int
print_phdr64(elf64_phdr* phdr) {
  elf64_ehdr* ehdr = (elf64_ehdr*)base;
  elf64_phdr* phdrs = (elf64_phdr*)((char*)base + ehdr->e_phoff);

  buffer_putlong(buffer_1, phdr - phdrs);

  buffer_puts(buffer_1, ": type=");
  buffer_putspad(buffer_1, get_p_type(phdr->p_type), 16);
  buffer_puts(buffer_1, ", offset=");
  buffer_putxlong0(buffer_1, phdr->p_offset, 8);
  buffer_puts(buffer_1, ", vaddr=");
  buffer_putxlong0(buffer_1, phdr->p_vaddr, 8);
  buffer_puts(buffer_1, ", paddr=");
  buffer_putxlong0(buffer_1, phdr->p_paddr, 8);
  buffer_puts(buffer_1, ", filesz=");
  buffer_putulong(buffer_1, phdr->p_filesz);
  buffer_puts(buffer_1, ", memsz=");
  buffer_putulong(buffer_1, phdr->p_memsz);
  buffer_puts(buffer_1, ", flags=");
  print_flags(p_flags, phdr->p_flags);

  buffer_putnlflush(buffer_1);

  return 0;
}


int
print_shdr64(elf64_shdr* shdr) {

  elf64_ehdr* ehdr = (elf64_ehdr*)base;
  elf64_shdr* shdrs = (elf64_shdr*)((char*)base + ehdr->e_shoff);

  int shnum = ehdr->e_shnum;

  buffer_putlong(buffer_1, shdr - shdrs);
  buffer_puts(buffer_1, ": ");
  buffer_putlong(buffer_1, shdr->sh_name);
  buffer_puts(buffer_1, " '");
  buffer_putspad(buffer_1, &elf_strtab(ehdr)[shdr->sh_name], 16);
  buffer_puts(buffer_1, "', flags=");
  print_flags(s_flags, shdr->sh_flags);
  buffer_putnlflush(buffer_1);

  return 0;
}

int
process32(elf32_ehdr* hdr) {
  return 0;
}

int
process64(elf64_ehdr* hdr) {
  elf64_shdr* shdrs = (elf64_shdr*)((char*)base + hdr->e_shoff);
  elf64_phdr* phdrs = (elf64_phdr*)((char*)base + hdr->e_phoff);
  int i;

  for(i = 0; i < hdr->e_shnum; ++i) {
    const char* name = elf_strtab(hdr) + shdrs[i].sh_name;

    if(str_equal(name, section)) {
      print_shdr64(&shdrs[i]);

      shdrs[i].sh_flags |= ELF_SHF_WRITE;
    }
  }

  for(i = 0; i < hdr->e_phnum; ++i) {

    if(!(phdrs[i].p_flags & ELF_PF_W)) {
      print_phdr64(&phdrs[i]);

      phdrs[i].p_flags |= ELF_PF_W;
    }
  }

  return 0;
}

int
elfwrsec(const char* file) {
  int ret;
  elf64_ehdr* header;

  header = base = mmap_shared(file, &size);

  ret = (header->e_ident[ELF_EI_CLASS] == ELF_ELFCLASS64 ? process64(base) : process32(base));

  mmap_unmap(base, size);

  return ret;
}

int
main(int argc, char* argv[]) {

  const char* fn = NULL;

  if(argc > 1) fn = argv[1];
  if(argc > 2) section = argv[2];

  return elfwrsec(fn);
}
