/* this header file comes from libowfat, http://www.fefe.de / libowfat/ */
#ifndef STR_H
#define STR_H

#include <stddef.h>
#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __pure__
#define __pure__
#endif

/* wstr_copy copies leading bytes from in to out until \0.
 * return number of copied bytes. */
size_t wstr_copy(wchar_t* out, const wchar_t* in);

/* wstr_diff returns negative, 0, or positive, depending on whether the
 * wstring a[0], a[1], ..., a[n] == '\0' is lexicographically smaller than,
 * equal to, or greater than the wstring b[0], b[1], ..., b[m - 1] == '\0'.
 * If the wstrings are different, wstr_diff does not read bytes past the
 * first difference. */
int wstr_diff(const wchar_t* a, const wchar_t* b) __pure__;

/* wstr_diffn returns negative, 0, or positive, depending on whether the
 * wstring a[0], a[1], ..., a[n] == '\0' is lexicographically smaller than,
 * equal to, or greater than the wstring b[0], b[1], ..., b[m - 1] == '\0'.
 * If the wstrings are different, wstr_diffn does not read bytes past the
 * first difference. The wstrings will be considered equal if the first
 * limit characters match. */
int wstr_diffn(const wchar_t* a, const wchar_t* b, size_t limit) __pure__;

#ifdef __dietlibc__
#include <wstring.h>
#define wstr_len(foo) wstrlen(foo)
#else
/* wstr_len returns the index of \0 in s */
size_t wstr_len(const wchar_t* s) __pure__;
#endif

/* wstr_chr returns the index of the first occurance of needle or \0 in haystack */
size_t wstr_chr(const wchar_t* haystack, wchar_t needle) __pure__;

/* wstr_rchr returns the index of the last occurance of needle or \0 in haystack */
size_t wstr_rchr(const wchar_t* haystack, wchar_t needle) __pure__;

/* wstr_start returns 1 if the b is a prefix of a, 0 otherwise */
int wstr_start(const wchar_t* a, const wchar_t* b) __pure__;

/* convenience shortcut to test for wstring equality */
#define wstr_equal(s, t) (!wstr_diff((s), (t)))

#ifdef __cplusplus
}
#endif

#endif
