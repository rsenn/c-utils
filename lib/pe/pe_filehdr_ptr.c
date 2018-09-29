#include "../uint32.h"
#include "../pe.h"

pe_file_header*
pe_filehdr_ptr(const void* ptr) {
  const pe_dos_header* dos = ptr;
  uint32 offset = uint32_get(&dos->e_lfanew);
  return (pe_file_header*)((unsigned char*)ptr + offset);
}
