#include "../elf.h"

void*
elf_find_segment_vaddr(range map, uint64 addr) {
  range segments = elf_program_headers(map.start);
  void* segment;

  range_foreach(&segments, segment) {
    uint64 vaddr = ELF_GET(map.start, segment, phdr, p_vaddr);
    uint64 memsz = ELF_GET(map.start, segment, phdr, p_memsz);

    if(addr >= vaddr && addr < vaddr + memsz)
      return segment;
  }
  return 0;
}
