#include "../coff.h"

const char*
coff_storage_class(uint8 sclass) {
  switch(sclass) {
    case COFF_C_NULL: return "null";
    case COFF_C_AUTO: return "automatic variable";
    case COFF_C_EXT: return "external symbol";
    case COFF_C_STATIC: return "static";
    case COFF_C_REG: return "register variable";
    case COFF_C_EXTDEF: return "external definition";
    case COFF_C_LABEL: return "label";
    case COFF_C_ULABEL: return "undefined label";
    case COFF_C_MOS: return "member of structure";
    case COFF_C_ARG: return "function argument";
    case COFF_C_STRTAG: return "structure tag";
    case COFF_C_MOU: return "member of union";
    case COFF_C_UNTAG: return "union tag";
    case COFF_C_TPDEF: return "type definition";
    case COFF_C_USTATIC: return "undefined static";
    case COFF_C_ENTAG: return "enumeration tag";
    case COFF_C_MOE: return "member of enumeration";
    case COFF_C_REGPARM: return "register parameter";
    case COFF_C_FIELD: return "bit field";
    case COFF_C_AUTOARG: return "auto argument";
    case COFF_C_LASTENT: return "dummy entry (end of block)";
    case COFF_C_BLOCK: return "\".bb\" or \".eb\"";
    case COFF_C_FCN: return "\".bf\" or \".ef\"";
    case COFF_C_EOS: return "end of structure";
    case COFF_C_FILE: return "file name";
    case COFF_C_LINE: return "line # reformatted as symbol table entry";
    case COFF_C_ALIAS: return "duplicate tag";
    case COFF_C_HIDDEN: return "ext symbol in dmert public lib";
    case COFF_C_EFCN: return "physical end of function";
  }
  return NULL;
}

