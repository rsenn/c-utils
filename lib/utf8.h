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

size_t u8btowcs(wchar_t*, const char*, size_t, size_t);
int u8bwcslen(const char*, size_t);
int u8len(const char*, size_t);
size_t u8stowcs(wchar_t*, const char*, size_t);
int u8swcslen(const char*);
int u8towc(wchar_t*, const char*);
size_t wcstou8s(char*, const wchar_t*, size_t);
int wcsu8slen(const wchar_t*);
int wctou8(char*, wchar_t);
int wcu8len(const wchar_t);

#ifdef __cplusplus
}
#endif

#endif /* !defined(UTF8_H) */
/** @} */
