#include "../xml.h"
#include "../str.h"
#include "../hmap.h"

HMAP_DB*
xml_vattributes(const char* name, const char* value, va_list args) {
HMAP_DB* db;
 hmap_init(1024, &db);
	do {
		hmap_set(db, name, str_len(name), value, str_len(value)+1);
 
    name = va_arg(args, const char*);
    value = va_arg(args, const char*);

	} while(name && value);
	return db;
}

HMAP_DB*
xml_attributes(const char* name, const char* value, ...) {
HMAP_DB* ret;
	va_list args;
	va_start(args, value);
	ret = xml_vattributes(name, value, args);
	va_end(args);
	return ret;
}