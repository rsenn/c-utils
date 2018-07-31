#ifndef _UTF8
#define _UTF8 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <wchar.h>

int    u8len(const char*, size_t count);
size_t u8stowcs(wchar_t*, const char* pu, size_t count);
int    u8swcslen(const char*);
int    u8towc(wchar_t*, const char* u, size_t count);
size_t wcstou8s(char*, const wchar_t* pw, size_t count);
int    wcsu8slen(const wchar_t*);
int    wctou8(char*, wchar_t w);
int    wcu8len(const wchar_t);

#ifdef __cplusplus
}
#endif
#endif
