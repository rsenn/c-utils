#include "../omf.h"

omf_record*
omf_end(omf_file* omf) {
 return (omf_record*)(((unsigned char*)omf->map) + omf->size);
}
