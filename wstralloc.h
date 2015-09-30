/* this header file comes from libowfat, http://www.fefe.de / libowfat/ */
#ifndef STRALLOC_H
#define STRALLOC_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __pure__
#define __pure__
#endif

/* wstralloc is the internal data structure all functions are working on.
 * s is the wstring.
 * len is the used length of the wstring.
 * a is the allocated length of the wstring.
 */

typedef struct wstralloc {
  wchar_t* s;
  size_t len;
  size_t a;
} wstralloc;

/* wstralloc_init will initialize a wstralloc.
 * Previously allocated memory will not be freed; use wstralloc_free for
 * that.  To assign an empty wstring, use wstralloc_copys(sa, ""). */
void wstralloc_init(wstralloc* sa);

/* wstralloc_ready makes sure that sa has enough space allocated to hold
 * len bytes: If sa is not allocated, wstralloc_ready allocates at least
 * len bytes of space, and returns 1. If sa is already allocated, but
 * not enough to hold len bytes, wstralloc_ready allocates at least len
 * bytes of space, copies the old wstring into the new space, frees the
 * old space, and returns 1. Note that this changes sa.s.  If the
 * allocation fails, wstralloc_ready leaves sa alone and returns 0. */
int wstralloc_ready(wstralloc* sa, size_t len);

/* wstralloc_readyplus is like wstralloc_ready except that, if sa is
 * already allocated, wstralloc_readyplus adds the current length of sa
 * to len. */
int wstralloc_readyplus(wstralloc* sa, size_t len);

/* wstralloc_copyb copies the wstring buf[0], buf[1], ..., buf[len - 1] into
 * sa, allocating space if necessary, and returns 1. If it runs out of
 * memory, wstralloc_copyb leaves sa alone and returns 0. */
int wstralloc_copyb(wstralloc* sa, const wchar_t* buf, size_t len);

/* wstralloc_copys copies a \0 - terminated wstring from buf into sa,
 * without the \0. It is the same as
 * wstralloc_copyb(&sa, buf, wstr_len(buf)). */
int wstralloc_copys(wstralloc* sa, const wchar_t* buf);

/* wstralloc_copy copies the wstring stored in sa2 into sa. It is the same
 * as wstralloc_copyb(&sa, sa2.s, sa2.len). sa2 must already be allocated. */
int wstralloc_copy(wstralloc* sa, const wstralloc* sa2);

/* wstralloc_catb adds the wstring buf[0], buf[1], ... buf[len - 1] to the
 * end of the wstring stored in sa, allocating space if necessary, and
 * returns 1. If sa is unallocated, wstralloc_catb is the same as
 * wstralloc_copyb. If it runs out of memory, wstralloc_catb leaves sa
 * alone and returns 0. */
int wstralloc_catb(wstralloc* sa, const wchar_t* in, size_t len);

/* wstralloc_cats is analogous to wstralloc_copys */
int wstralloc_cats(wstralloc* sa, const wchar_t* in);

void wstralloc_zero(wstralloc* sa);

/* like wstralloc_cats but can cat more than one wstring at once */
int wstralloc_catm_internal(wstralloc* sa, ...);

#define wstralloc_catm(sa, ...) wstralloc_catm_internal(sa, __VA_ARGS__, (wchar_t *)0)
#define wstralloc_copym(sa, ...) (wstralloc_zero(sa), wstralloc_catm_internal(sa, __VA_ARGS__, (wchar_t *)0))

/* wstralloc_cat is analogous to wstralloc_copy */
int wstralloc_cat(wstralloc* sa, wstralloc* in);

/* wstralloc_append adds one byte in[0] to the end of the wstring stored
 * in sa. It is the same as wstralloc_catb(&sa, in, 1). */
int wstralloc_append(wstralloc* sa, const wchar_t* in); /* beware: this takes a pointer to 1 wchar_t */

/* wstralloc_starts returns 1 if the \0 - terminated wstring in "in", without
 * the terminating \0, is a prefix of the wstring stored in sa. Otherwise
 * it returns 0. sa must already be allocated. */
int wstralloc_starts(wstralloc* sa, const wchar_t* in) __pure__;

/* wstralloc_diff returns negative, 0, or positive, depending on whether
 * a is lexicographically smaller than, equal to, or greater than the
 * wstring b. */
int wstralloc_diff(const wstralloc* a, const wstralloc* b) __pure__;

/* wstralloc_diffs returns negative, 0, or positive, depending on whether
 * a is lexicographically smaller than, equal to, or greater than the
 * wstring b[0], b[1], ..., b[n] == '\0'. */
int wstralloc_diffs(const wstralloc* a, const wchar_t* b) __pure__;

#define wstralloc_equal(a, b) (!wstralloc_diff((a), (b)))
#define wstralloc_equals(a, b) (!wstralloc_diffs((a), (b)))

/* wstralloc_0 appends \0 */
#define wstralloc_0(sa) wstralloc_append(sa, "")

int wstralloc_nul(wstralloc* sa);

/* wstralloc_catulong0 appends a '0' padded ASCII representation of in */
int wstralloc_catulong0(wstralloc* sa, unsigned long int in, size_t n);

/* wstralloc_catlong0 appends a '0' padded ASCII representation of in */
int wstralloc_catlong0(wstralloc* sa, signed long int in, size_t n);

/* wstralloc_free frees the storage associated with sa */
void wstralloc_free(wstralloc* sa);

#define wstralloc_catlong(sa, l) (wstralloc_catlong0((sa), (l), 0))
#define wstralloc_catuint0(sa, i, n) (wstralloc_catulong0((sa), (i), (n)))
#define wstralloc_catint0(sa, i, n) (wstralloc_catlong0((sa), (i), (n)))
#define wstralloc_catint(sa, i) (wstralloc_catlong0((sa), (i), 0))

/* remove last wchar_t.  Return removed byte as unsigned short (or -1 if wstralloc was empty). */
int wstralloc_chop(wstralloc* sa);

/* remove trailing "\r\n", "\n" or "\r".  Return number of removed chars (0, 1 or 2) */
int wstralloc_chomp(wstralloc* sa);

#ifdef BUFFER_H
/* write wstralloc to buffer */
int buffer_putsa(buffer* b, wstralloc* sa);
/* write wstralloc to buffer and flush */
int buffer_putsaflush(buffer* b, wstralloc* sa);

/* these "read token" functions return 0 if the token was complete or
 * EOF was hit or -1 on error.  In contrast to the non - wstralloc token
 * functions, the separator is also put in the wstralloc; use
 * wstralloc_chop or wstralloc_chomp to get rid of it. */

/* WARNING!  These token reading functions will not clear the wstralloc!
 * They _append_ the token to the contents of the wstralloc.  The idea is
 * that this way these functions can be used on non - blocking sockets;
 * when you get signalled EAGAIN, just call the functions again when new
 * data is available. */

/* read token from buffer to wstralloc */
int buffer_get_token_sa(buffer* b, wstralloc* sa, const wchar_t* charset, size_t setlen);
/* read line from buffer to wstralloc */
int buffer_getline_sa(buffer* b, wstralloc* sa);

/* same as buffer_get_token_sa but empty sa first */
int buffer_get_new_token_sa(buffer* b, wstralloc* sa, const wchar_t* charset, size_t setlen);
/* same as buffer_getline_sa but empty sa first */
int buffer_getnewline_sa(buffer* b, wstralloc* sa);

typedef int ( * sa_predicate)(wstralloc* sa);

/* like buffer_get_token_sa but the token ends when your predicate says so */
int buffer_get_token_sa_pred(buffer* b, wstralloc* sa, sa_predicate p);
/* same, but clear sa first */
int buffer_get_new_token_sa_pred(buffer* b, wstralloc* sa, sa_predicate p);


/* make a buffer from a wstralloc.
 * Do not change the wstralloc after this! */
void buffer_fromsa(buffer* b, wstralloc* sa);
#endif

#ifdef __cplusplus
}
#endif

#endif
