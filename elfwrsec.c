#include <elf.h>
#include "lib/mmap.h"

static size_t size;
static void* base;

int
process32(Elf32_Ehdr* hdr) {
  return 0;
}

int
process64(Elf64_Ehdr* hdr) {
  return 0;
}

int main(int argc, char* argv[]) {

  base = mmap_private(argv[1], &size);


  Elf64_Ehdr* header = base;

  int ret = (header->e_ident[EI_CLASS] == ELFCLASS64 ? process64(base) : process32(base));

  mmap_unmap(base, size);


  return ret;
}
