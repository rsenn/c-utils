#include "../omf.h"

void*
omf_record_next(omf_record* rec, void* ptr) {
  unsigned char* p = ptr;

  switch(rec->type) {
    /* OMF_THEADR */
    case 0x80: {
      break;
    }

    /* OMF_LHEADR */
    case 0x82: {
      break;
    }

    /* OMF_COMENT */
    case 0x88: {
      break;
    }

    /* OMF_MODEND */
    case 0x8B:
    case 0x8A: {
      break;
    }

    /* OMF_EXTDEF */
    case 0x8C: {
      int16 type_index;

      /* skip name */
      p += p[0] + 1;

      p = omf_read_index(p, &type_index);

      /* checksum?  */
      if(p[0] == 0 || p + 1 == omf_record_end(rec))
        p += 1;

      break;
    }

    /* OMF_PUBDEF */
    case 0x91:
    case 0x90: {
      int16 type_index;

      /* skip public name */
      p += p[0] + 1;

      /* skip public offset */
      p += (rec->type & 1) ? 4 : 2;

      p = omf_read_index(p, &type_index);

      /* skip checksum */
      p += 1;
      break;
    }

    /* OMF_LINNUM */
    case 0x95:
    case 0x94: {
      break;
    }

    /* OMF_LNAMES */
    case 0x96: {
      /* skip name */
      p += p[0] + 1;
      break;
    }

    /* OMF_SEGDEF */
    case 0x99:
    case 0x98: {
      break;
    }

    /* OMF_GRPDEF */
    case 0x9A: {
      break;
    }

    /* OMF_FIXUPP */
    case 0x9D:
    case 0x9C: {
      break;
    }

    /* OMF_LEDATA */
    case 0xA1:
    case 0xA0: {
      break;
    }

    /* OMF_LIDATA */
    case 0xA3:
    case 0xA2: {
      break;
    }

    /* OMF_COMDEF */
    case 0xB0: {
      break;
    }

    /* OMF_BAKPAT */
    case 0xB3:
    case 0xB2: {
      break;
    }

    /* OMF_LEXTDEF */
    case 0xB4: {
      break;
    }

    /* OMF_LPUBDEF */
    case 0xB7:
    case 0xB6: {
      break;
    }

    /* OMF_LCOMDEF */
    case 0xB8: {
      break;
    }

    /* OMF_CEXTDEF */
    case 0xBC: {
      break;
    }

    /* OMF_COMDAT */
    case 0xC3:
    case 0xC2: {
      break;
    }

    /* OMF_LINSYM */
    case 0xC5:
    case 0xC4: {
      break;
    }

    /* OMF_ALIAS */
    case 0xC6: {
      break;
    }

    /* OMF_NBKPAT */
    case 0xC9:
    case 0xC8: {
      break;
    }

    /* OMF_LLNAMES */
    case 0xCA: {
      break;
    }

    /* OMF_VERNUM */
    case 0xCC: {
      break;
    }

    /* OMF_VENDEXT */
    case 0xCE: {
      break;
    }

    /* OMF_LIBHEAD */
    case 0xF0: {
      break;
    }

    /* OMF_LIBEND */
    case 0xF1: {
      break;
    }
  }
  return p;
}
