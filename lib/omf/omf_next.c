#include "../omf.h"

omf_record*
omf_next(omf_record* rec) {
	uint16 l = uint16_get(&rec->length);
	unsigned char* r = (unsigned char*)rec;
    if(l > 1024) l = 1024;

	rec = (omf_record*)(r + 3 + l);
    return rec;
}