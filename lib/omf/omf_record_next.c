#include "../omf.h"

void*
omf_record_next(omf_record* rec, void* ptr) {
  unsigned char* p = (unsigned char*)ptr;

  switch(rec->type) {
    /* OMF_THEADR */
    case OMF_THEADR: break;

    /* OMF_LHEADR */
    case OMF_LHEADR: break;

    /* OMF_COMENT */
    case OMF_COMENT: break;

    /* OMF_MODEND */
    case OMF_MODEND + 1:
    case OMF_MODEND: break;

    /* OMF_EXTDEF */
    case OMF_EXTDEF: {
      int16 type_index;
      /* skip name */
      p += p[0] + 1;
      p = (unsigned char*)omf_read_index(p, &type_index);
      /* checksum?  */
      if(p[0] == 0 || p + 1 == omf_record_end(rec))
        p += 1;
      break;
    }

    /* OMF_PUBDEF */
    case OMF_PUBDEF + 1:
    case OMF_PUBDEF: {
      int16 type_index;
      /* skip public name */
      p += p[0] + 1;
      /* skip public offset */
      p += (rec->type & 1) ? 4 : 2;
      p = (unsigned char*)omf_read_index(p, &type_index);
      /* skip checksum */
      p += 1;
      break;
    }

    /* OMF_LINNUM */
    case OMF_LINNUM + 1:
    case OMF_LINNUM: break;

    /* OMF_LNAMES */
    case OMF_LNAMES: {
      /* skip name */
      p += p[0] + 1;
      break;
    }

    /* OMF_SEGDEF */
    case OMF_SEGDEF +:
    case OMF_SEGDEF: break;

    /* OMF_GRPDEF */
    case OMF_GRPDEF: break;

    /* OMF_FIXUPP */
    case OMF_FIXUPP + 1:
    case OMF_FIXUPP: break;

    /* OMF_LEDATA */
    case OMF_LEDATA + 1:
    case OMF_LEDATA: break;

    /* OMF_LIDATA */
    case OMF_LIDATA + 1:
    case OMF_LIDATA: break;

    /* OMF_COMDEF */
    case OMF_COMDEF: break;

    /* OMF_BAKPAT */
    case OMF_BAKPAT + 1:
    case OMF_BAKPAT: break;

    /* OMF_LEXTDEF */
    case OMF_LEXTDEF: break;

    /* OMF_LPUBDEF */
    case OMF_LPUBDEF + 1:
    case OMF_LPUBDEF: break;

    /* OMF_LCOMDEF */
    case OMF_LCOMDEF: break;

    /* OMF_CEXTDEF */
    case OMF_CEXTDEF: break;

    /* OMF_COMDAT */
    case OMF_COMDAT + 1:
    case OMF_COMDAT: break;

    /* OMF_LINSYM */
    case OMF_LINSYM + 1:
    case OMF_LINSYM: break;

    /* OMF_ALIAS */
    case OMF_ALIAS: break;

    /* OMF_NBKPAT */
    case OMF_NBKPAT + 1:
    case OMF_NBKPAT: break;

    /* OMF_LLNAMES */
    case OMF_LLNAMES: break;

    /* OMF_VERNUM */
    case OMF_VERNUM: break;

    /* OMF_VENDEXT */
    case OMF_VENDEXT: break;

    /* OMF_LIBHEAD */
    case OMF_LIBHEAD: break;

    /* OMF_LIBEND */
    case OMF_LIBEND: break;
  }
  return p;
}
