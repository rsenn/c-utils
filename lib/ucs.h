#ifndef UCS_H
#define UCS_H

#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

wchar_t ucs_from_latin1(int ch);
char    ucs_to_latin1(wchar_t ch);

#ifdef __cplusplus
}
#endif
#endif /* defined UCS_H */
