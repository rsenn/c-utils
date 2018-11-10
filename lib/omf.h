#ifndef OMF_H
#define OMF_H

#include "typedefs.h"
#include "uint8.h"
#include "uint16.h"
#include "range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct omf_record_s {
  uint8 type;
  uint16 length;
} omf_record;

typedef struct omf_file_s {
  void* map;
  size_t size;
  omf_record* record;
  range data;
} omf_file;

int         omf_open(omf_file*, const char* filename);
void        omf_close(omf_file*);
omf_record* omf_begin(omf_file*);
range       omf_data(omf_record*);
omf_record* omf_end(omf_file*);
const char* omf_name(omf_record*);
omf_record* omf_next(omf_record*);

#define omf_foreach(omf, recptr) \
  for((recptr) = omf_begin(omf); (char*)recptr + 2 + uint16_get(&(recptr)->length)  < (char*)omf_end(omf); (recptr) = omf_next(recptr))

#ifdef __cplusplus
}
#endif
#endif /* defined OMF_H */