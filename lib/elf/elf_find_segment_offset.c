#include "../elf.h"

void*
elf_find_segment_offset(range map, uint64 offs) {
  range segments = elf_program_headers(map.start);
  void* segment;

  range_foreach(&segments, segment) {
    uint64 offset = ELF_GET(map.start, segment, phdr, p_offset);
    uint64 filesz = ELF_GET(map.start, segment, phdr, p_filesz);

    if(offs >= offset && offs < offset + filesz)
      return segment;
  }
  return 0;
}
