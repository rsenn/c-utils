#include "../uint32.h"
#include "../pe.h"

void*
pe_opthdr_ptr(const void* ptr) {
  char* p = (char*)pe_filehdr_ptr(ptr);
  p += sizeof(pe_file_header) + sizeof(uint32);
  return p;
}
