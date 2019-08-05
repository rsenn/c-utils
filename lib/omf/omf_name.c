#include "../omf.h"

const char*
omf_name(omf_record* rec) {
  switch(rec->type) {
    /* Translator Header Record */
    case 0x80:
      return "THEADR";
    /* Library Module Header Record */
    case 0x82:
      return "LHEADR";
    /* Comment Record (Including all comment class extensions) */
    case 0x88:
      return "COMENT";
    /* Module End Record */
    case 0x8a:
    case 0x8b:
      return "MODEND";
    /* External Names Definition Record */
    case 0x8c:
      return "EXTDEF";
    /* Public Names Definition Record */
    case 0x90:
    case 0x91:
      return "PUBDEF";
    /* Line Numbers Record */
    case 0x94:
    case 0x95:
      return "LINNUM";
    /* List of Names Record */
    case 0x96:
      return "LNAMES";
    /* Segment Definition Record */
    case 0x98:
    case 0x99:
      return "SEGDEF";
    /* Group Definition Record */
    case 0x9a:
      return "GRPDEF";
    /* Fixup Record */
    case 0x9c:
    case 0x9d:
      return "FIXUPP";
    /* Logical Enumerated Data Record */
    case 0xa0:
    case 0xa1:
      return "LEDATA";
    /* Logical Iterated Data Record */
    case 0xa2:
    case 0xa3:
      return "LIDATA";
    /* Communal Names Definition Record */
    case 0xb0:
      return "COMDEF";
    /* Backpatch Record */
    case 0xb2:
    case 0xb3:
      return "BAKPAT";
    /* Local External Names Definition Record */
    case 0xb4:
      return "LEXTDEF";
    /* Local Public Names Definition Record */
    case 0xb6:
    case 0xb7:
      return "LPUBDEF";
    /* Local Communal Names Definition Record */
    case 0xb8:
      return "LCOMDEF";
    /* COMDAT External Names Definition Record */
    case 0xbc:
      return "CEXTDEF";
    /* Initialized Communal Data Record */
    case 0xc2:
    case 0xc3:
      return "COMDAT";
    /* Symbol Line Numbers Record */
    case 0xc4:
    case 0xc5:
      return "LINSYM";
    /* Alias Definition Record */
    case 0xc6:
      return "ALIAS";
    /* Named Backpatch Record */
    case 0xc8:
    case 0xc9:
      return "NBKPAT";
    /* Local Logical Names Definition Record */
    case 0xca:
      return "LLNAMES";
    /* OMF Version Number Record */
    case 0xcc:
      return "VERNUM";
    /* Vendor-specific OMF Extension Record */
    case 0xce:
      return "VENDEXT";
    /* Header Record */
    case 0xf0: return "Library";
  }
  return NULL;
}

