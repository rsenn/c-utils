#include "../coff.h"

coff_opt_header*
coff_header_opt(void* obj) {
  coff_file_header* coff = coff_header_file(obj);
  return (coff_opt_header*)&coff[1];
}
