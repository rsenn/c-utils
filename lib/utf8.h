/**
 * @defgroup   utf8
 * @brief      UTF8 module.
 * @{
 */
#ifndef UTF8_H
#define UTF8_H

#define _UTF8 1

#include <stdlib.h>
#include <sys/types.h>
#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t u8b_chrs(const char*, size_t, const char[], size_t);
int u8b_diff(const char*, size_t, const char*, size_t);
int u8b_len(const char*, size_t);
size_t u8b_rchrs(const char*, size_t, const char[], size_t);
size_t u8b_to_wcs(wchar_t*, const char*, size_t, size_t);
size_t u8s_chrs(const char*, const char[], size_t);
int u8s_diff(const char*, const char*);
int u8s_len(const char*);
size_t u8s_rchrs(const char*, const char[], size_t);
size_t u8s_to_wcs(wchar_t*, const char*, size_t);
int u8_len(const char*, size_t);
int u8_to_wc(wchar_t*, const char*);
size_t wcs_to_u8s(char*, const wchar_t*, size_t);
int wcs_u8slen(const wchar_t*);
int wc_to_u8(char*, wchar_t);
int wc_u8len(const wchar_t);

#ifdef HAVE_STRALLOC_H
size_t wcs_to_u8sa(stralloc*, const wchar_t*);
#endif

#ifdef __cplusplus
}
#endif

#endif /* !defined(UTF8_H) */
/** @} */
