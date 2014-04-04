#ifndef STR_H
#define STR_H

#ifdef __dietlibc__
#include <sys/cdefs.h>
#endif
#ifndef __pure__
#define __pure__
#endif

/* str_copy copies leading bytes from in to out until \0.
 * return number of copied bytes. */
unsigned long str_copy(char *out,const char *in);

unsigned long str_copyn(char *out,const char *in, unsigned long n);

/* str_diff returns negative, 0, or positive, depending on whether the
 * string a[0], a[1], ..., a[n]=='\0' is lexicographically smaller than,
 * equal to, or greater than the string b[0], b[1], ..., b[m-1]=='\0'.
 * If the strings are different, str_diff does not read bytes past the
 * first difference. */
int str_diff(const char *a,const char *b) __pure__;

/* str_diffn returns negative, 0, or positive, depending on whether the
 * string a[0], a[1], ..., a[n]=='\0' is lexicographically smaller than,
 * equal to, or greater than the string b[0], b[1], ..., b[m-1]=='\0'.
 * If the strings are different, str_diffn does not read bytes past the
 * first difference. The strings will be considered equal if the first
 * limit characters match. */
int str_diffn(const char *a,const char *b,unsigned long limit) __pure__;

/* str_len returns the index of \0 in s */
unsigned long str_len(const char *s) __pure__;

/* str_chr returns the index of the first occurance of needle or \0 in haystack */
unsigned long str_chr(const char *haystack,char needle) __pure__;

/* str_rchr returns the index of the last occurance of needle or \0 in haystack */
unsigned long str_rchr(const char *haystack,char needle) __pure__;

/* str_start returns 1 if the b is a prefix of a, 0 otherwise */
int str_start(const char *a,const char *b) __pure__;

/* convenience shortcut to test for string equality */
#define str_equal(s,t) (!str_diff((s),(t)))

#endif
