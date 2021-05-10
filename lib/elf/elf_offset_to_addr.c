#include "../elf.h"

int64
elf_offset_to_addr(range map, uint64 offs) {
  void* segment;

  if((segment = elf_find_segment_offset(map, offs))) {
    uint64 offset = ELF_GET(map.start, segment, phdr, p_offset);
    uint64 vaddr = ELF_GET(map.start, segment, phdr, p_vaddr);

    return offs - offset + vaddr;
  }
  return -1;
}
