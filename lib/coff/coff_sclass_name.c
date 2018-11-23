#include "../coff.h"

const char*
coff_sclass_name(uint8 sclass) {
  switch(sclass) {
    case COFF_C_NULL: return "NULL";
    case COFF_C_AUTO: return "AUTO";
    case COFF_C_EXT: return "EXT";
    case COFF_C_STATIC: return "STATIC";
    case COFF_C_REG: return "REG";
    case COFF_C_EXTDEF: return "EXTDEF";
    case COFF_C_LABEL: return "LABEL";
    case COFF_C_ULABEL: return "ULABEL";
    case COFF_C_MOS: return "MOS";
    case COFF_C_ARG: return "ARG";
    case COFF_C_STRTAG: return "STRTAG";
    case COFF_C_MOU: return "MOU";
    case COFF_C_UNTAG: return "UNTAG";
    case COFF_C_TPDEF: return "TPDEF";
    case COFF_C_USTATIC: return "USTATIC";
    case COFF_C_ENTAG: return "ENTAG";
    case COFF_C_MOE: return "MOE";
    case COFF_C_REGPARM: return "REGPARM";
    case COFF_C_FIELD: return "FIELD";
    case COFF_C_AUTOARG: return "AUTOARG";
    case COFF_C_LASTENT: return "LASTENT";
    case COFF_C_BLOCK: return "BLOCK";
    case COFF_C_FCN: return "FCN";
    case COFF_C_EOS: return "EOS";
    case COFF_C_FILE: return "FILE";
    case COFF_C_LINE: return "LINE";
    case COFF_C_ALIAS: return "ALIAS";
    case COFF_C_HIDDEN: return "HIDDEN";
    case COFF_C_EFCN: return "EFCN";
  }
  return NULL;
}