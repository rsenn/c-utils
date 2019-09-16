#include "../pe.h"

const char*
pe_datadir_name(int i) {
  static const char* const names[] = {
      "EXPORT",
      "IMPORT",
      "RESOURCE",
      "EXCEPTION",
      "SECURITY",
      "BASERELOC",
      "DEBUG",
      "ARCHITECTURE",
      "GLOBALPTR",
      "TLS",
      "LOAD_CONFIG",
      "BOUND_IMPORT",
      "IAT",
      "DELAY_IMPORT",
      "COMHEADER",
  };
  if(i >= 0 && i < (int)(sizeof(names) / sizeof(names[0])))
    return names[i];
  return NULL;
}

