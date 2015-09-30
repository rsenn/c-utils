/* this header file comes from libowfat, http://www.fefe.de / libowfat/ */
#ifndef WORD_H
#define WORD_H

/* for size_t: */
#include <stddef.h>
#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __pure__
#define __pure__
#endif

/* word_chr returns the smallest integer i between 0 and len - 1
 * inclusive such that one[i] equals needle, or len if not found. */
size_t word_chr(const void* haystack, size_t len, short needle) __pure__;

/* word_rchr returns the largest integer i between 0 and len - 1 inclusive
 * such that one[i] equals needle, or len if not found. */
size_t word_rchr(const void* haystack, size_t len, short needle) __pure__;

/* word_copy copies in[0] to out[0], in[1] to out[1], ... and in[len - 1]
 * to out[len - 1]. */
void word_copy(void* out, size_t len, const void* in);

/* word_copyr copies in[len - 1] to out[len - 1], in[len - 2] to out[len - 2],
 * ... and in[0] to out[0] */
void word_copyr(void* out, size_t len, const void* in);

/* word_diff returns negative, 0, or positive, depending on whether the
 * string a[0], a[1], ..., a[len - 1] is lexicographically smaller
 * than, equal to, or greater than the string b[0], b[1], ...,
 * b[len - 1]. When the strings are different, word_diff does not read
 * words past the first difference. */
int word_diff(const void* a, size_t len, const void* b) __pure__;

/* word_zero sets the words out[0], out[1], ..., out[len - 1] to 0 */
void word_zero(void* out, size_t len);

void word_fill(void* out, size_t len, int c);

#define word_equal(s, n, t) (!word_diff((s), (n), (t)))

int word_equal_notimingattack(const void* a, size_t len, const void* b) __pure__;

#ifdef __cplusplus
}
#endif

#endif
