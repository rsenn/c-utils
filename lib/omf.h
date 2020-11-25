#ifndef OMF_H
#define OMF_H

#include "typedefs.h"
#include "uint8.h"
#include "uint16.h"
#include "range.h"

#ifdef __cplusplus
extern "C" {
#endif

/*#pragma pack(push, 1)*/
typedef struct __unaligned {
  uint8 type;
  uint16 length;
}
omf_record;
/*#pragma pack(pop)*/

#define OMF_LNAME_FLAT 0x01  // Default group name
#define OMF_LNAME_CODE 0x02  // Default class for code
#define OMF_LNAME_DATA 0x03  // Default class for data
#define OMF_LNAME_BSS 0x04   // Default class for uninitialized data
#define OMF_LNAME_CONST 0x05 // Default class for constant data
#define OMF_LNAME_LAST 0x05  // Last default name. Nondefault names start at OMF_LNAME_LAST + 1
#define OMF_THEADR 0x80      // Translator Header Record
#define OMF_LHEADR 0x82      // Library Module Header Record
#define OMF_COMENT 0x88      // Comment Record (Including all comment class extensions)
#define OMF_MODEND 0x8A      // (0x8B) Module End Record
#define OMF_EXTDEF 0x8C      // External Names Definition Record
#define OMF_PUBDEF 0x90      // (0x91) Public Names Definition Record
#define OMF_LINNUM 0x94      // (0x95) Line Numbers Record
#define OMF_LNAMES 0x96      // List of Names Record
#define OMF_SEGDEF 0x98      // (0x99) Segment Definition Record
#define OMF_GRPDEF 0x9A      // Group Definition Record
#define OMF_FIXUPP 0x9C      // (0x9D) Fixup Record
#define OMF_LEDATA 0xA0      // (0xA1) Logical Enumerated Data Record
#define OMF_LIDATA 0xA2      // (0xA3) Logical Iterated Data Record
#define OMF_COMDEF 0xB0      // Communal Names Definition Record
#define OMF_BAKPAT 0xB2      // (0xB3) Backpatch Record
#define OMF_LEXTDEF 0xB4     // Local External Names Definition Record
#define OMF_LPUBDEF 0xB6     // (0xB7) Local Public Names Definition Record
#define OMF_LCOMDEF 0xB8     // Local Communal Names Definition Record
#define OMF_CEXTDEF 0xBC     // COMDAT External Names Definition Record
#define OMF_COMDAT 0xC2      // (0xC3) Initialized Communal Data Record
#define OMF_LINSYM 0xC4      // (0xC5) Symbol Line Numbers Record
#define OMF_ALIAS 0xC6       // Alias Definition Record
#define OMF_NBKPAT 0xC8      // (0xC9) Named Backpatch Record
#define OMF_LLNAMES 0xCA     // Local Logical Names Definition Record
#define OMF_VERNUM 0xCC      // OMF Version Number Record
#define OMF_VENDEXT 0xCE     // Vendor-specific OMF Extension Record
#define OMF_LIBHEAD 0xF0     // Library Header Record
#define OMF_LIBEND 0xF1      // Library End Record
#define OMF_LIBEXT 0xF2      // Library extended dictionary

typedef struct omf_file_s {
  void* map;
  size_t size;
  omf_record* record;
  range data;
} omf_file;

int omf_open(omf_file*, const char* filename);
void omf_close(omf_file*);

omf_record* omf_begin(omf_file*);
omf_record* omf_end(omf_file*);
omf_record* omf_next(omf_record*);

range omf_data(omf_record*);
range omf_get_name(omf_record*, size_t idx);

const char* omf_name(omf_record*);

void* omf_record_begin(omf_record*);
void* omf_record_end(omf_record*);
void* omf_record_next(omf_record*, void* ptr);

#define omf_foreach(omf, r)                                                                        \
  for((r) = omf_begin(omf); (char*)r + 2 + uint16_get(&(r)->length) < (char*)omf_end(omf);         \
      (r) = omf_next(r))

#define omf_record_foreach(rec, ptr)                                                               \
  for((ptr) = omf_record_begin(rec); (char*)(ptr) < (char*)omf_record_end(rec);                    \
      (ptr) = omf_record_next(rec, (ptr)))

static inline void*
omf_read_index(void* readptr, int16* idxptr) {
  unsigned char* p = (unsigned char*)readptr;
  if(p[0] & 0x80) {
    *idxptr = (p[0] & 0x7f) * 0x100 + p[1];
    ++p;
  } else {
    *idxptr = p[0];
  }
  return p + 1;
}

#define omf_str_len(ptr) (((unsigned char*)ptr)[0])
#define omf_str_begin(ptr) (((char*)ptr) + 1)
#define omf_str_end(ptr) (omf_str_begin(ptr) + omf_str_len(ptr))

#ifdef __cplusplus
}
#endif
#endif /* defined OMF_H */
