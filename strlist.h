#ifndef STRLIST_H
#define STRLIST_H

#ifdef __dietlibc__
#include <sys/cdefs.h>
#endif
#ifdef _MSC_VER
#include <crtdefs.h>
#endif

#ifndef __pure__
#define __pure__
#endif

#include <sys/types.h>

/* strlist is the internal data structure all functions are working on.
 * s is the string.
 * len is the used length of the string.
 * a is the allocated length of the string.
 */

typedef struct strlist {
  char* s;
  size_t len;
  size_t a;
} strlist;

/* strlist_init will initialize a strlist.
 * Previously allocated memory will not be freed; use strlist_free for
 * that.  To assign an empty string, use strlist_copys(sa,""). */
void strlist_init(strlist* sa);

/* strlist_ready makes sure that sa has enough space allocated to hold
 * len bytes: If sa is not allocated, strlist_ready allocates at least
 * len bytes of space, and returns 1. If sa is already allocated, but
 * not enough to hold len bytes, strlist_ready allocates at least len
 * bytes of space, copies the old string into the new space, frees the
 * old space, and returns 1. Note that this changes sa.s.  If the
 * allocation fails, strlist_ready leaves sa alone and returns 0. */
#ifdef DEBUG
int strlist_readydebug(const char *file, unsigned int line, strlist* sa,size_t len);
#define strlist_ready(sa, len) strlist_readydebug(__FILE__, __LINE__, (sa), (len))
#else
int strlist_ready(strlist* sa,size_t len);
#endif /* DEBUG */

/* strlist_readyplus is like strlist_ready except that, if sa is
 * already allocated, strlist_readyplus adds the current length of sa
 * to len. */
#ifdef DEBUG
int strlist_readyplusdebug(const char *file, unsigned int line, strlist* sa,size_t len);
#define strlist_readyplus(sa, len) strlist_readyplusdebug(__FILE__, __LINE__, (sa), (len))
#else
int strlist_readyplus(strlist* sa,size_t len);
#endif /* DEBUG */

/* strlist_copyb copies the string buf[0], buf[1], ..., buf[len-1] into
 * sa, allocating space if necessary, and returns 1. If it runs out of
 * memory, strlist_copyb leaves sa alone and returns 0. */
int strlist_copyb(strlist* sa,const char* buf,size_t len);

/* strlist_copys copies a \0-terminated string from buf into sa,
 * without the \0. It is the same as
 * strlist_copyb(&sa,buf,str_len(buf)). */
int strlist_copys(strlist* sa,const char* buf);

/* strlist_copy copies the string stored in sa2 into sa. It is the same
 * as strlist_copyb(&sa,sa2.s,sa2.len). sa2 must already be allocated. */
int strlist_copy(strlist* sa,const strlist* sa2);

/* strlist_catb adds the string buf[0], buf[1], ... buf[len-1] to the
 * end of the string stored in sa, allocating space if necessary, and
 * returns 1. If sa is unallocated, strlist_catb is the same as
 * strlist_copyb. If it runs out of memory, strlist_catb leaves sa
 * alone and returns 0. */
int strlist_catb(strlist* sa,const char* in,size_t len);

int strlist_write(strlist* sa,const char *in, size_t len);
int strlist_catc(strlist* sa,const unsigned char c);

/* strlist_cats is analogous to strlist_copys */
int strlist_cats(strlist* sa,const char* in);

void strlist_zero(strlist* sa);
#define strlist_ZERO(sa) (sa)->len=0;

/* like strlist_cats but can cat more than one string at once */
int strlist_catm_internal(strlist* sa,...);

#define strlist_catm(sa,...) strlist_catm_internal(sa,__VA_ARGS__,0)
#define strlist_copym(sa,...) (strlist_zero(sa) && strlist_catm_internal(sa,__VA_ARGS__,0))

/* strlist_cat is analogous to strlist_copy */
int strlist_cat(strlist* sa,strlist* in);

/* strlist_append adds one byte in[0] to the end of the string stored
 * in sa. It is the same as strlist_catb(&sa,in,1). */
int strlist_append(strlist* sa,const char* in); /* beware: this takes a pointer to 1 char */

/* strlist_starts returns 1 if the \0-terminated string in "in", without
 * the terminating \0, is a prefix of the string stored in sa. Otherwise
 * it returns 0. sa must already be allocated. */
int strlist_starts(strlist* sa,const char* in) __pure__;

/* strlist_diff returns negative, 0, or positive, depending on whether
 * a is lexicographically smaller than, equal to, or greater than the
 * string b. */
int strlist_diff(const strlist* a,const strlist* b) __pure__;

/* strlist_diffs returns negative, 0, or positive, depending on whether
 * a is lexicographically smaller than, equal to, or greater than the
 * string b[0], b[1], ..., b[n]=='\0'. */
int strlist_diffs(const strlist* a,const char* b) __pure__;

#define strlist_equal(a,b) (!strlist_diff((a),(b)))
#define strlist_equals(a,b) (!strlist_diffs((a),(b)))

/* strlist_0 appends \0 */
#define strlist_0(sa) strlist_append(sa,"")

int strlist_nul(strlist *sa);

/* strlist_catulong0 appends a '0' padded ASCII representation of in */
int strlist_catulong0(strlist* sa,size_t in,size_t n);

/* strlist_catlong0 appends a '0' padded ASCII representation of in */
int strlist_catlong0(strlist* sa,signed long int in,size_t n);

/* strlist_free frees the storage associated with sa */
#ifdef DEBUG
void strlist_freedebug(const char *file, unsigned int line, strlist* sa);
#define strlist_free(sa) strlist_freedebug(__FILE__, __LINE__, (sa))
#else
void strlist_free(strlist* sa);
#endif /* DEBUG */
#define strlist_FREE(sa) \
do { \
  if((sa)->s) shell_free((sa)->s); \
  (sa)->s = 0; \
} while(0);


#define strlist_catlong(sa,l) (strlist_catlong0((sa),(l),0))
#define strlist_catuint0(sa,i,n) (strlist_catulong0((sa),(i),(n)))
#define strlist_catint0(sa,i,n) (strlist_catlong0((sa),(i),(n)))
#define strlist_catint(sa,i) (strlist_catlong0((sa),(i),0))

/* remove last char.  Return removed byte as unsigned char (or -1 if strlist was empty). */
int strlist_chop(strlist* sa);

/* remove trailing "\r\n", "\n" or "\r".  Return number of removed chars (0,1 or 2) */
int strlist_chomp(strlist* sa);

void strlist_move(strlist* to, strlist* from);
  
int strlist_remove(strlist* sa, size_t pos, size_t n);
int strlist_insertb(strlist* sa, const char *s, size_t pos, size_t n);

#ifdef DEBUG
int strlist_truncdebug(const char *file, unsigned int line, strlist *sa,size_t n);
#define strlist_trunc(sa, n) strlist_truncdebug(__FILE__, __LINE__, (sa), (n))
#else
int strlist_trunc(strlist *sa,size_t n);  
#endif /* DEBUG */
  
#ifdef BUFFER_H
/* write strlist to buffer */
int buffer_putsa(buffer* b,strlist* sa);
/* write strlist to buffer and flush */
int buffer_putsaflush(buffer* b,strlist* sa);

/* these "read token" functions return 0 if the token was complete or
 * EOF was hit or -1 on error.  In contrast to the non-strlist token
 * functions, the separator is also put in the strlist; use
 * strlist_chop or strlist_chomp to get rid of it. */

/* WARNING!  These token reading functions will not clear the strlist!
 * They _append_ the token to the contents of the strlist.  The idea is
 * that this way these functions can be used on non-blocking sockets;
 * when you get signalled EAGAIN, just call the functions again when new
 * data is available. */

/* read token from buffer to strlist */
int buffer_get_token_sa(buffer* b,strlist* sa,const char* charset);
/* read line from buffer to strlist */
int buffer_getline_sa(buffer* b,strlist* sa);

/* same as buffer_get_token_sa but empty sa first */
int buffer_get_new_token_sa(buffer* b,strlist* sa,const char* charset,size_t setlen);
/* same as buffer_getline_sa but empty sa first */
int buffer_getnewline_sa(buffer* b,strlist* sa);

typedef int (*sa_predicate)(strlist* sa);

/* like buffer_get_token_sa but the token ends when your predicate says so */
int buffer_get_token_sa_pred(buffer* b,strlist* sa,sa_predicate p);
/* same, but clear sa first */
int buffer_get_new_token_sa_pred(buffer* b,strlist* sa,sa_predicate p);


/* make a buffer from a strlist.
 * Do not change the strlist after this! */
void buffer_fromsa(buffer* b,strlist* sa);
#endif

#endif
