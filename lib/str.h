/* this header file comes from libowfat, http://www.fefe.de / libowfat/ */
#ifndef STR_H
#define STR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __pure__
#define __pure__
#endif

/* str_copy copies leading bytes from in to out until \0.
 * return number of copied bytes. */
size_t str_copy(char* out, const char* in);
size_t str_copyn(void* p1, const void* p2, size_t max);

/* str_diff returns negative, 0, or positive, depending on whether the
 * string a[0], a[1], ..., a[n] == '\0' is lexicographically smaller than,
 * equal to, or greater than the string b[0], b[1], ..., b[m - 1] == '\0'.
 * If the strings are different, str_diff does not read bytes past the
 * first difference. */
int str_diff(const char* a, const char* b) __pure__;

/* str_diffn returns negative, 0, or positive, depending on whether the
 * string a[0], a[1], ..., a[n] == '\0' is lexicographically smaller than,
 * equal to, or greater than the string b[0], b[1], ..., b[m - 1] == '\0'.
 * If the strings are different, str_diffn does not read bytes past the
 * first difference. The strings will be considered equal if the first
 * limit characters match. */
int str_diffn(const char* a, const char* b, size_t limit) __pure__;

size_t str_len(const char *in);

/* str_chr returns the index of the first occurance of needle or \0 in haystack */
size_t str_chr(const char* haystack, char needle) __pure__;

/* str_rchr returns the index of the last occurance of needle or \0 in haystack */
size_t str_rchr(const char* haystack, char needle) __pure__;
size_t str_rchrs(const char* haystack, char needles[], size_t);

/* str_start returns 1 if the b is a prefix of a, 0 otherwise */
int str_start(const char* a, const char* b) __pure__;

char *str_istr(char *s, char *find);
char *str_tok(char *s, const char *delim, char **ptrptr);
char *str_dup(const char *s);
char *str_ndup(const char *s, size_t n);

struct tm;

char* str_ptime(const char* s,const char* format, struct tm* tm);

char*
str_basename(char* s) ;

/* returned string must be free'd */
char *str_decamelize(const char *s);

int str_case_diff(const void *p1, const void *p2);
int str_case_diffn(const void *p1, const void *p2, size_t n);
size_t str_case_equal(const void *s, const void *t);
size_t str_case_start(const void *p1, const void *p2);
size_t str_cat(void *p1, const void *p2);
size_t str_copyb(void *p1, const void *p2, size_t max);
size_t str_find(const void *s, const void *what);
size_t str_findb(const void *p1, const void *what, size_t len);
void str_lower(void *str);

struct tm;

char* str_ptime(const char* s,const char* format, struct tm* tm);

char*
str_basename(char* s) ;
/* convenience shortcut to test for string equality */
#define str_equal(s, t) (!str_diff((s), (t)))

#ifdef __cplusplus
}
#endif

#endif
int str_end(const char* a, const char* b);
