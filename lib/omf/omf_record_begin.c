#include "../omf.h"

void*
omf_record_begin(omf_record* rec) {
  range data = omf_data(rec);
  char* p = data.start;

  switch(rec->type) {
    /* OMF_PUBDEF */
    case 0x91:
    case 0x90: {
      int16 group_index, segment_index;

      p = (char*)omf_read_index(p, &group_index);
      p = (char*)omf_read_index(p, &segment_index);

      if(segment_index == 0)
        p += 2; // skip base frame field

      /* now got pointer to first public name (length) */
      break;
    }

    /* OMF_LNAMES */ case 0x96:
    /* OMF_EXTDEF */ case 0x8C : { break; }
    default: {
      /* a record kind that cannot be walked: nothing to iterate (it used to return
       * NULL, which omf_record_foreach then treated as a valid element) */
      p = omf_record_end(rec);
      break;
    }
  }
  return p;
}
