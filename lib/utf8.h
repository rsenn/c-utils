#ifndef _UTF8
#define _UTF8 1
#include <stdlib.h>
#include <wchar.h>

size_t u8stowcs(wchar_t *, const char *, size_t);
size_t wcstou8s(char *, const wchar_t *, size_t);

#endif
