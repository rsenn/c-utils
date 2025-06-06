/**
 * @defgroup   buffer
 * @brief      BUFFER module.
 * @{
 */
/* this header file comes from libowfat,  http://www.fefe.de/libowfat/ */
#ifndef BUFFER_H
#define BUFFER_H

#include "typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

struct buffer;

typedef ssize_t buffer_op_sys(fd_type, void*, size_t);
typedef ssize_t buffer_op_proto(fd_type, void*, size_t, struct buffer*);
typedef ssize_t buffer_op_fn();
typedef void buffer_deinit(struct buffer*);

typedef buffer_op_fn* buffer_op_ptr;

typedef struct buffer {
  char* x;                        /* actual buffer space */
  size_t p;                       /* current position */
  size_t n;                       /* current size of string in buffer */
  size_t a;                       /* allocated buffer size */
  buffer_op_proto* op;            /* use read(2) or write(2) */
  void* cookie;                   /* used internally by the to-stralloc
                                   * buffers, and for buffer chaining */
  void (*deinit)(struct buffer*); /* called to munmap/free cleanup,
                                   * with a pointer to the buffer as argument */
  fd_type fd;                     /* passed as first argument to op */
} buffer;

#define BUFFER_INIT(op, fd, buf, len) \
  { (buf), 0, 0, (len), (buffer_op_proto*)(void*)(op), NULL, NULL, (fd) }
#define BUFFER_INIT_FREE(op, fd, buf, len) \
  { (buf), 0, 0, (len), (buffer_op_proto*)(void*)(op), NULL, buffer_free, (fd) }
#define BUFFER_INIT_READ(op, fd, buf, len) BUFFER_INIT(op, fd, buf, len) /*obsolete*/
#define BUFFER_INSIZE 65535
#define BUFFER_OUTSIZE 32768

void buffer_init(buffer*, buffer_op_proto*, fd_type fd, char* y, size_t ylen);
void buffer_init_free(buffer*, buffer_op_proto*, fd_type fd, char* y, size_t ylen);
void buffer_free(buffer*);
void buffer_munmap(buffer*);
int buffer_mmapread(buffer*, const char* filename);
int buffer_mmapread_fd(buffer*, fd_type fd);
int buffer_mmapprivate(buffer*, const char* filename);
int buffer_mmapprivate_fd(buffer*, fd_type fd);
int buffer_mmapshared(buffer*, const char* filename);
int buffer_mmapshared_fd(buffer*, fd_type fd);
void buffer_close(buffer* b);

/* reading from an fd... if it is a regular file,  then  buffer_mmapread_fd
   is
   called, otherwise  buffer_init(&b,  read,  fd,  malloc(8192),  8192) */
int buffer_read_fd(buffer*, fd_type fd);

int buffer_flush(buffer* b);
int buffer_put(buffer*, const char* x, size_t len);
int buffer_putalign(buffer*, const char* x, size_t len);
ssize_t buffer_putflush(buffer*, const char* x, size_t len);
int buffer_puts(buffer*, const char* x);
int buffer_putsalign(buffer*, const char* x);
ssize_t buffer_putsflush(buffer*, const char* x);

#if defined(__GNUC__) && !defined(__LIBOWFAT_INTERNAL) && !defined(__dietlibc__) && !defined(NO_BUILTINS)
/* as a little gcc-specific hack,  if somebody calls buffer_puts with a
 * constant string,  where we know its length at compile-time,  call
 * buffer_put with the known length instead */
#define buffer_puts(b, s) (__builtin_constant_p(s) ? buffer_put(b, s, __builtin_strlen(s)) : buffer_puts(b, s))
#define buffer_putsflush(b, s) (__builtin_constant_p(s) ? buffer_putflush(b, s, __builtin_strlen(s)) : buffer_putsflush(b, s))
#endif

int buffer_putm_internal(buffer* b, ...);
int buffer_putm_internal_flush(buffer* b, ...);

#if defined(__BORLANDC__) || defined(__LCC__)
#define buffer_putm(b, args) buffer_putm_internal(b, args, (char*)0)
#define buffer_putmflush(b, args) buffer_putm_internal_flush(b, args, (char*)0)
#else
#define buffer_putm(...) buffer_putm_internal(__VA_ARGS__, (char*)0)
#define buffer_putmflush(b, ...) buffer_putm_internal_flush(b, __VA_ARGS__, (char*)0)
#endif
#define buffer_putm_2(b, a1, a2) buffer_putm_internal(b, a1, a2, (char*)0)
#define buffer_putm_3(b, a1, a2, a3) buffer_putm_internal(b, a1, a2, a3, (char*)0)
#define buffer_putm_4(b, a1, a2, a3, a4) buffer_putm_internal(b, a1, a2, a3, a4, (char*)0)
#define buffer_putm_5(b, a1, a2, a3, a4, a5) buffer_putm_internal(b, a1, a2, a3, a4, a5, (char*)0)
#define buffer_putm_6(b, a1, a2, a3, a4, a5, a6) buffer_putm_internal(b, a1, a2, a3, a4, a5, a6, (char*)0)
#define buffer_putm_7(b, a1, a2, a3, a4, a5, a6, a7) buffer_putm_internal(b, a1, a2, a3, a4, a5, a6, a7, (char*)0)

int buffer_putspace(buffer*);
int buffer_putnlflush(buffer*); /* put \n and flush */

#define buffer_PUTC(s, c) (((s)->a != (s)->p) ? ((s)->x[(s)->p++] = (c), 0) : buffer_putc((s), (c)))

ssize_t buffer_get(buffer*, char*, size_t);
ssize_t buffer_getc(buffer*, char*);
ssize_t buffer_getn(buffer*, char*, size_t);

/* read bytes until the destination buffer is full (len bytes),  end of
 * file is reached or the read char is in charset (setlen bytes).  An
 * empty line when looking for \n will write '\n' to x and return 0.  If
 * EOF is reached,  \0 is written to the buffer */
ssize_t buffer_get_token(buffer*, char*, size_t, const char*, size_t);
ssize_t buffer_getline(buffer*, char*, size_t);
int buffer_skip_until(buffer*, const char*, size_t);

/* this predicate is given the string as currently read from the buffer
 * and is supposed to return 1 if the token is complete,  0 if not. */
typedef int (*string_predicate)(const char*, size_t, void*);

/* like buffer_get_token but the token ends when your predicate says so */
ssize_t buffer_get_token_pred(buffer*, char*, size_t, string_predicate, void*);

char* buffer_peek(buffer*);
int buffer_peekc(buffer*, char*);
void buffer_seek(buffer*, size_t);
void buffer_rewind(buffer*);

int buffer_skipc(buffer*);
int buffer_skipn(buffer*, size_t);

#define buffer_EOF(b) (buffer_feed((b)) == 0)
#define buffer_PEEK(b) ((b)->x + (b)->p)
#define buffer_LEN(b) ((b)->n - (b)->p)
#define buffer_SKIP(b, len) ((b)->p += (len))
#define buffer_SEEK(b, len) ((b)->n += (len))
#define buffer_EMPTY(b) ((b)->p == (b)->n)

#define buffer_GETC(b, c) (((b)->p < (b)->n) ? (*(c) = *buffer_PEEK(b), buffer_SKIP((b), 1), 1) : buffer_get((b), (c), 1))

#define buffer_BEGIN(b) ((b)->x)
#define buffer_END(b) ((b)->x + (b)->n)
#define buffer_HEADROOM(b) ((b)->a - (b)->n)
#define buffer_SPACE(b) ((b)->a - (b)->p)

#define buffer_MOVE(b) \
  do { \
    size_t len; \
    if((len = buffer_LEN(b)) > 0) \
      byte_copy((b)->x, len, buffer_PEEK(b)); \
    (b)->n -= (b)->p; \
    (b)->p = 0; \
  } while(0);

#define buffer_SCAN(b, fn) \
  do { \
    size_t len = (fn)(buffer_PEEK(b), buffer_LEN(b)); \
    buffer_SKIP((b), (len)); \
  } while(0);

int buffer_putulong(buffer*, unsigned long);
int buffer_put8long(buffer*, unsigned long);
int buffer_putxlong(buffer*, unsigned long);
int buffer_putlong(buffer*, signed long);

int buffer_putdouble(buffer*, double, int);

int buffer_puterror(buffer*);
int buffer_puterror2(buffer*, int);

extern buffer* buffer_0;
extern buffer* buffer_0small;
extern buffer* buffer_1;
extern buffer* buffer_1small;
extern buffer* buffer_2;

#ifdef STRALLOC_H
/* write stralloc to buffer */
int buffer_putsa(buffer*, const stralloc* sa);
/* write stralloc to buffer and flush */
int buffer_putsaflush(buffer*, const stralloc* sa);

/* these "read token" functions return 0 if the token was complete or
 * EOF was hit or -1 on error.  In contrast to the non-stralloc token
 * functions,  the separator is also put in the stralloc; use
 * stralloc_chop or stralloc_chomp to get rid of it. */

/* WARNING!  These token reading functions will not clear the stralloc!
 * They _append_ the token to the contents of the stralloc.  The idea is
 * that this way these functions can be used on non-blocking sockets;
 * when you get signalled EAGAIN,  just call the functions again when new
 * data is available. */

/* read token from buffer to stralloc */
int buffer_get_token_sa(buffer*, stralloc* sa, const char* charset, size_t setlen);
/* read line from buffer to stralloc */
int buffer_getline_sa(buffer*, stralloc* sa);

/* same as buffer_get_token_sa but empty sa first */
int buffer_get_new_token_sa(buffer*, stralloc* sa, const char* charset, size_t setlen);
/* same as buffer_getline_sa but empty sa first */
int buffer_getnewline_sa(buffer*, stralloc* sa);

typedef int (*sa_predicate)(stralloc* sa, void*);

/* like buffer_get_token_sa but the token ends when your predicate says so
 */
int buffer_get_token_sa_pred(buffer*, stralloc* sa, sa_predicate p, void*);
/* same,  but clear sa first */
int buffer_get_new_token_sa_pred(buffer*, stralloc* sa, sa_predicate p, void*);

/* make a buffer from a stralloc.
 * Do not change the stralloc after this! */
void buffer_fromsa(buffer*, const stralloc* sa); /* read from sa */
int buffer_tosa(buffer* b, stralloc* sa);        /* write to sa,  auto-growing it */

int buffer_gettok_sa(buffer*, stralloc* sa, const char* charset, size_t setlen);

int buffer_getln(buffer*, stralloc*, int*, int);

int buffer_put_escaped(buffer*, const void*, size_t, size_t (*escape)(char*, int));
int buffer_putsa_escaped(buffer*, const stralloc* sa, size_t (*escape)(char*, int));
#endif

void buffer_frombuf(buffer*, const char* x, size_t l); /* buffer reads from static buffer */
int buffer_copybuf(buffer*, const char* x, size_t l);
#ifdef ARRAY_H
void buffer_fromarray(buffer*, array* a); /* buffer reads from array */
#endif
void buffer_dump(buffer* out, buffer* b);

int buffer_putc(buffer*, char c);
int buffer_putnspace(buffer*, int n);

int buffer_putptr(buffer*, void* ptr);
int buffer_putulong0(buffer*, unsigned long l, int pad);
int buffer_putlong0(buffer*, long l, int pad);
int buffer_putxlong0(buffer*, unsigned long l, int pad);

int buffer_skipspace(buffer* b);
int buffer_skip_pred(buffer*, int (*pred)(int));

int buffer_putfmt_args(buffer*, const char* x, size_t len, size_t (*escape)(char*, int, void*, void*, void*, void*), void* args[]);
int buffer_putfmt(buffer* b, const char* x, size_t len, size_t (*escape)(char*, int), ...);

int buffer_puts_escaped(buffer* b, const char* s, size_t (*escape)(char*, int));

int buffer_put_quoted(buffer* b, const char* x, size_t len);

/** If buffer is empty, read in more data
 *
 * @param      b     buffer object
 *
 * @return     new available bytes
 */
ssize_t buffer_feed(buffer* b);

/** Read as much bytes as possible. If read position is not 0,
 *  move buffer contents to the beginning of the buffer.
 *
 * @param      b     buffer object
 *
 * @return     new available bytes
 */
ssize_t buffer_freshen(buffer* b);

/** Prefetch \param n bytes from current read position
 *
 * @param      b     buffer object
 * @param      n     bytes to prefetch
 *
 * @return     new available bytes
 */
ssize_t buffer_prefetch(buffer*, size_t n);

int buffer_appendfile(buffer* b, const char* fn);
int buffer_readfile(buffer* b, const char* fn);
int buffer_truncfile(buffer* b, const char* fn);
int buffer_writefile(buffer* b, const char* fn);

int buffer_lzma(buffer*, buffer*, int compress);
int buffer_bz2(buffer*, buffer*, int compress);
int buffer_brotli(buffer*, buffer*, int compress);

int buffer_putnc(buffer*, char c, int ntimes);
int buffer_putns(buffer*, const char* s, int ntimes);

int buffer_putspad(buffer*, const char* x, size_t pad);
int buffer_putspadstart(buffer*, const char* x, size_t pad);

int buffer_deflate(buffer*, buffer* out, int level);
int buffer_inflate(buffer*, buffer* in);

int buffer_gunzip(buffer*, const char* filename);
int buffer_gunzip_fd(buffer*, fd_type fd);
int buffer_gzip(buffer*, const char* filename, int level);
int buffer_gzip_fd(buffer*, fd_type fd, int level);
int buffer_bunzip(buffer*, const char* filename);
int buffer_bunzip_fd(buffer*, fd_type fd);
int buffer_bzip(buffer*, const char* filename, int level);
int buffer_bzip_fd(buffer*, fd_type fd, int level);

int buffer_get_until(buffer*, char* x, size_t len, const char* charset, size_t setlen);

int buffer_write_fd(buffer*, fd_type fd);

#ifdef UINT64_H
int buffer_putlonglong(buffer*, int64 l);
int buffer_putulonglong(buffer*, uint64 l);
int buffer_putxlonglong(buffer*, uint64 l);
int buffer_putulonglong(buffer*, uint64 i);
int buffer_putlonglong(buffer*, int64 i);
int buffer_putxlonglong0(buffer*, uint64 l, int pad);
int buffer_putulonglongpad(buffer*, uint64, int pad);
#endif

#ifdef TAI_H
int buffer_puttai(buffer*, const struct tai*);
#endif
int buffer_copy(buffer* out, buffer* in);
void buffer_putstr(buffer* b, const char* x);
void buffer_realloc(buffer* b, size_t ylen);

int buffer_putxlong0u(buffer*, unsigned long l, int pad);

#define buffer_LINE(b) byte_line((b)->x, (b)->p)
#define buffer_COLUMN(b) byte_column((b)->x, (b)->p)

#ifdef __cplusplus
}
#endif

#endif
/** @} */
