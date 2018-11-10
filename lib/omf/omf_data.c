#include "../omf.h"

range
omf_data(omf_record* rec) {
	range r;
    size_t n = uint16_get(&rec->length);

    if(n > 1024) n = 1024;

	r.start = (char*)rec;
    r.end = r.start + (r.elem_size = n);
	return r;
}