/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef BYTE_H
#define BYTE_H

/* for size_t: */
#include <stddef.h>

#if !defined(_MSC_VER) && !defined(__MSYS__) && !defined(__CYGWIN__) && !defined(__BORLANDC__)
#include <inttypes.h>
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __pure__
#define __pure__
#endif

/* byte_chr returns the smallest integer i between 0 and len-1
 * inclusive such that one[i] equals needle, or len if not found. */
size_t byte_chr(const void* haystack, size_t len, char needle) __pure__;

/* byte_rchr returns the largest integer i between 0 and len-1 inclusive
 * such that one[i] equals needle, or len if not found. */
size_t byte_rchr(const void* haystack,size_t len,char needle) __pure__;

/* byte_copy copies in[0] to out[0], in[1] to out[1], ... and in[len-1]
 * to out[len-1]. */
void byte_copy(void* out, size_t len, const void* in);

/* byte_copyr copies in[len-1] to out[len-1], in[len-2] to out[len-2],
 * ... and in[0] to out[0] */
void byte_copyr(void* out, size_t len, const void* in);

/* byte_diff returns negative, 0, or positive, depending on whether the
 * string a[0], a[1], ..., a[len-1] is lexicographically smaller
 * than, equal to, or greater than the string b[0], b[1], ...,
 * b[len-1]. When the strings are different, byte_diff does not read
 * bytes past the first difference. */
int byte_diff(const void* a, size_t len, const void* b) __pure__;

/* byte_zero sets the bytes out[0], out[1], ..., out[len-1] to 0 */
void byte_zero(void* out, size_t len);

//#define byte_equal(s,n,t) (!byte_diff((s),(n),(t)))

int byte_equal_notimingattack(const void* a, size_t len,const void* b) __pure__;

void byte_fill(void *out, size_t len, int c);

int byte_case_diff(const void *x1, unsigned int len, const void *x2);
unsigned int byte_case_equal(const void *s, unsigned int len, const void *t);
void byte_copyr(void *out, size_t len, const void *in);
unsigned int byte_count(const void *s, unsigned int n, char c);
int byte_diff(const void *a, size_t len, const void *b);
unsigned int byte_equal(const void *s, unsigned int n, const void *t);
void byte_fill(void *out, size_t len, int c);
void byte_lower(void *s, unsigned int len);
void byte_upper(void* s, unsigned int len);

size_t byte_findb(const void* haystack, size_t hlen, const void* what, size_t wlen);
size_t byte_finds(const void* haystack, size_t hlen, const char* what);

#if defined(__i386__) || defined(__x86_64__)
#define UNALIGNED_ACCESS_OK
#endif

#ifdef STRALLOC_H
size_t byte_fmt(const char *in, size_t in_len, stralloc *out, size_t (*fmt_function)(char *, unsigned int ch));
size_t byte_scan(const char *in, size_t in_len, stralloc *out, size_t (*scan_function)(const char *, char *));
#endif

#ifdef __cplusplus
}
#endif

#endif

