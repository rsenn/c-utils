#ifndef UCS_H
#define UCS_H

#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

wchar_t ucs_from_latin1_char(int ch);
char    ucs_to_latin1_char(wchar_t ch);

size_t  fmt_latin1_utf8(char*, char);
size_t  scan_latin1_utf8(const char*, size_t, char* out);

#ifdef __cplusplus
}
#endif
#endif /* defined UCS_H */
