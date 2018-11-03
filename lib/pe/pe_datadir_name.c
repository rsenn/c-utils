#include "../pe.h"

const char*
pe_datadir_name(int i) {
  static const char*const names[] = {
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
    "COMHEADER",
  };
  return names[i];
}
