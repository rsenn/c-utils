
#ifndef UCS_H
#define UCS_H

#include "typedefs.h"
#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

wchar_t ucs_from_latin1_char(int ch);
char ucs_to_latin1_char(wchar_t ch);

size_t fmt_latin1_utf8(char*, char);
size_t scan_latin1_utf8(const char*, size_t, char* out);

#ifdef STRALLOC_H
size_t stralloc_latin1_to_utf8(stralloc*, const char*, size_t);
size_t stralloc_utf8_to_latin1(stralloc*, const char*, size_t);
size_t stralloc_utf8_latin1(stralloc*);
size_t stralloc_utf8_latin1(stralloc*);
#endif
size_t str_utf8_latin1(char*);

#ifdef __cplusplus
}
#endif
#endif /* defined(UCS_H) */
