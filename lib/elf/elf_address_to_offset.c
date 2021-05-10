#include "../elf.h"

int64
elf_address_to_offset(range map, uint64 addr) {
  void* segment;

  if((segment = elf_find_segment_vaddr(map, addr))) {
    uint64 offset = ELF_GET(map.start, segment, phdr, p_offset);
    uint64 vaddr = ELF_GET(map.start, segment, phdr, p_vaddr);

    return addr - vaddr + offset;
  }
  return -1;
}
