/**
 * @defgroup   ucs
 * @brief      UCS module.
 * @{
 */

#ifndef UCS_H
#define UCS_H

#include "typedefs.h"
#include "uint32.h"
#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

wchar_t ucs_from_latin1_char(int ch);
char ucs_to_latin1_char(wchar_t ch);

/* scan_utf8_ucs4 decodes one code point, returns the bytes used, 0 if invalid */
size_t scan_utf8_ucs4(const char* src, size_t n, uint32* dest);
/* fmt_ucs4_utf8 encodes one code point, returns the length, dest may be NULL */
size_t fmt_ucs4_utf8(char* dest, uint32 c);
/* ucs_tolower is a simple Latin, Greek and Cyrillic case fold */
uint32 ucs_tolower(uint32 c);

/* Punycode (RFC 3492) without the xn-- prefix. fmt returns the length (dest may
 * be NULL), scan takes the capacity in *max and returns n, 0 on error. */
size_t fmt_punycode(char* dest, const uint32* src, size_t n);
size_t scan_punycode(const char* src, size_t n, uint32* dest, size_t* max);

/* IDNA label conversion of a dotted domain name, return the length, 0 on error */
size_t ucs_idna_to_ascii(char* dest, size_t max, const char* src, size_t n);
size_t ucs_idna_to_unicode(char* dest, size_t max, const char* src, size_t n);

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
/** @} */
