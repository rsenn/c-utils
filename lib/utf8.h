#ifndef _UTF8
#define _UTF8 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <wchar.h>

size_t u8stowcs(wchar_t *, const char *, size_t);
size_t wcstou8s(char *, const wchar_t *, size_t);

#ifdef __cplusplus
}
#endif
#endif
