#include "unit_test.h"
#include "../lib/buffer.h"

/* extern buffer* buffer_0; */
TEST(test_buffer_0) {
}

/* extern buffer* buffer_0small; */
TEST(test_buffer_0small) {
}

/* extern buffer* buffer_1; */
TEST(test_buffer_1) {
}

/* extern buffer* buffer_1small; */
TEST(test_buffer_1small) {
}

/* extern buffer* buffer_2; */
TEST(test_buffer_2) {
}

/* int buffer_bz2(buffer*, buffer*, int compress); */
TEST(test_buffer_bz2) {
}

/* int buffer_bzip(buffer*, const char* filename, int level); */
TEST(test_buffer_bzip) {
}

/* void buffer_close(buffer* b); */
TEST(test_buffer_close) {
}

/* int buffer_deflate(buffer*, buffer* out, int level); */
TEST(test_buffer_deflate) {
}

/* void buffer_dump(buffer* out, buffer* b); */
TEST(test_buffer_dump) {
}

/* ssize_t buffer_feed(buffer* b); */
TEST(test_buffer_feed) {
}

/* int buffer_flush(buffer* b); */
TEST(test_buffer_flush) {
}

/*   { (buf), 0, 0, (len), (buffer_op_proto*)(void*)(op), NULL, buffer_free, (fd) }
void buffer_free(void* buf); */
TEST(test_buffer_free) {
}

/* int buffer_freshen(buffer* b); */
TEST(test_buffer_freshen) {
}

/* void buffer_fromarray(buffer*, array* a); /* buffer reads from array */
TEST(test_buffer_fromarray) {
}

/* void buffer_frombuf(buffer*, const char* x, size_t l); /* buffer reads from static buffer */
TEST(test_buffer_frombuf) {
}

/* void buffer_fromsa(buffer*, const stralloc* sa); /* read from sa */
TEST(test_buffer_fromsa) {
}

/* ssize_t buffer_get(buffer*, char* x, size_t len);
  (((s)->p < (s)->n) ? (*(c) = *buffer_PEEK(s), buffer_SEEK((s), 1), 1) : buffer_get((s), (c), 1)) */
TEST(test_buffer_get) {
}

/* ssize_t buffer_getc(buffer*, char* x); */
TEST(test_buffer_getc) {
}

/* ssize_t buffer_getline(buffer*, char* x, size_t len); */
TEST(test_buffer_getline) {
}

/* ssize_t buffer_getn(buffer*, char* x, size_t len); */
TEST(test_buffer_getn) {
}

/* ssize_t buffer_get_token(buffer*, char* x, size_t len, const char* charset, size_t setlen);
/* like buffer_get_token but the token ends when your predicate says so */
TEST(test_buffer_get_token) {
}

/* ssize_t buffer_get_token_pred(buffer*, char* x, size_t len, string_predicate p, void*); */
TEST(test_buffer_get_token_pred) {
}

/* int buffer_get_until(buffer*, char* x, size_t len, const char* charset, size_t setlen); */
TEST(test_buffer_get_until) {
}

/* int buffer_gzip(buffer*, const char* filename, int level); */
TEST(test_buffer_gzip) {
}

/* int buffer_inflate(buffer*, buffer* in); */
TEST(test_buffer_inflate) {
}

/* void buffer_init(buffer*, buffer_op_proto*, fd_t fd, char* y, size_t ylen);
   otherwise  buffer_init(&b,  read,  fd,  malloc(8192),  8192) */
TEST(test_buffer_init) {
}

/* void buffer_init_free(buffer*, buffer_op_proto*, fd_t fd, char* y, size_t ylen); */
TEST(test_buffer_init_free) {
}

/* int buffer_lzma(buffer*, buffer*, int compress); */
TEST(test_buffer_lzma) {
}

/* int buffer_mmapprivate(buffer*, const char* filename); */
TEST(test_buffer_mmapprivate) {
}

/* int buffer_mmapprivate_fd(buffer*, fd_t fd); */
TEST(test_buffer_mmapprivate_fd) {
}

/* int buffer_mmapread(buffer*, const char* filename); */
TEST(test_buffer_mmapread) {
}

/* int buffer_mmapread_fd(buffer*, fd_t fd);
/* reading from an fd... if it is a regular file,  then  buffer_mmapread_fd is called, */
TEST(test_buffer_mmapread_fd) {
}

/* int buffer_mmapshared_fd(buffer*, fd_t fd); */
TEST(test_buffer_mmapshared_fd) {
}

/* void buffer_munmap(void* buf); */
TEST(test_buffer_munmap) {
}

/* char* buffer_peek(buffer* b); */
TEST(test_buffer_peek) {
}

/* int buffer_peekc(buffer*, char* c); */
TEST(test_buffer_peekc) {
}

/* int buffer_prefetch(buffer*, size_t n); */
TEST(test_buffer_prefetch) {
}

/* int buffer_put8long(buffer*, unsigned long int l); */
TEST(test_buffer_put8long) {
}

/* int buffer_putalign(buffer*, const char* x, size_t len); */
TEST(test_buffer_putalign) {
}

/* int buffer_put(buffer*, const char* x, size_t len);
 * buffer_put with the known length instead */
#define buffer_puts(b, s) (__builtin_constant_p(s) ? buffer_put(b, s, __builtin_strlen(s)) : buffer_puts(b, s)) */
TEST(test_buffer_put) {
}

/* #define buffer_PUTC(s, c) (((s)->a != (s)->p) ? ((s)->x[(s)->p++] = (c), 0) : buffer_putc((s), (c)))
int buffer_putc(buffer*, char c); */
TEST(test_buffer_putc) {
}

/* int buffer_putdouble(buffer*, double d, int prec); */
TEST(test_buffer_putdouble) {
}

/* int buffer_puterror2(buffer*, int errnum); */
TEST(test_buffer_puterror2) {
}

/* int buffer_puterror(buffer* b); */
TEST(test_buffer_puterror) {
}

/* ssize_t buffer_putflush(buffer*, const char* x, size_t len);
  (__builtin_constant_p(s) ? buffer_putflush(b, s, __builtin_strlen(s)) : buffer_putsflush(b, s)) */
TEST(test_buffer_putflush) {
}

/* int buffer_putlong0(buffer*, long l, int pad); */
TEST(test_buffer_putlong0) {
}

/* int buffer_putlong(buffer*, signed long int l); */
TEST(test_buffer_putlong) {
}

/* int buffer_putlonglong(buffer*, int64 l);
int buffer_putlonglong(buffer*, int64 i); */
TEST(test_buffer_putlonglong) {
}

/* int buffer_putm_internal(buffer* b, ...);
#define buffer_putm(b, args) buffer_putm_internal(b, args, (char*)0)
#define buffer_putm(...) buffer_putm_internal(__VA_ARGS__, (char*)0)
#define buffer_putm_2(b, a1, a2) buffer_putm_internal(b, a1, a2, (char*)0)
#define buffer_putm_3(b, a1, a2, a3) buffer_putm_internal(b, a1, a2, a3, (char*)0)
#define buffer_putm_4(b, a1, a2, a3, a4) buffer_putm_internal(b, a1, a2, a3, a4, (char*)0)
#define buffer_putm_5(b, a1, a2, a3, a4, a5) buffer_putm_internal(b, a1, a2, a3, a4, a5, (char*)0)
#define buffer_putm_6(b, a1, a2, a3, a4, a5, a6) buffer_putm_internal(b, a1, a2, a3, a4, a5, a6, (char*)0)
#define buffer_putm_7(b, a1, a2, a3, a4, a5, a6, a7) buffer_putm_internal(b, a1, a2, a3, a4, a5, a6, a7, (char*)0) */
TEST(test_buffer_putm_internal) {
}

/* int buffer_putm_internal_flush(buffer* b, ...);
#define buffer_putmflush(b, args) buffer_putm_internal_flush(b, args, (char*)0)
#define buffer_putmflush(b, ...) buffer_putm_internal_flush(b, __VA_ARGS__, (char*)0) */
TEST(test_buffer_putm_internal_flush) {
}

/* int buffer_putnc(buffer*, char c, int ntimes); */
TEST(test_buffer_putnc) {
}

/* ssize_t buffer_putnlflush(buffer* b); /* put \n and flush */
TEST(test_buffer_putnlflush) {
}

/* int buffer_putns(buffer*, const char* s, int ntimes); */
TEST(test_buffer_putns) {
}

/* int buffer_putnspace(buffer*, int n); */
TEST(test_buffer_putnspace) {
}

/* int buffer_putptr(buffer*, void* ptr); */
TEST(test_buffer_putptr) {
}

/* int buffer_putsa(buffer*, const stralloc* sa); */
TEST(test_buffer_putsa) {
}

/* int buffer_putsaflush(buffer*, const stralloc* sa); */
TEST(test_buffer_putsaflush) {
}

/* int buffer_putsalign(buffer*, const char* x); */
TEST(test_buffer_putsalign) {
}

/* int buffer_puts(buffer*, const char* x);
/* as a little gcc-specific hack,  if somebody calls buffer_puts with a
#define buffer_puts(b, s) (__builtin_constant_p(s) ? buffer_put(b, s, __builtin_strlen(s)) : buffer_puts(b, s)) */
TEST(test_buffer_puts) {
}

/* ssize_t buffer_putsflush(buffer*, const char* x);
#define buffer_putsflush(b, s)                                                                                         \
  (__builtin_constant_p(s) ? buffer_putflush(b, s, __builtin_strlen(s)) : buffer_putsflush(b, s)) */
TEST(test_buffer_putsflush) {
}

/* int buffer_putspace(buffer* b); */
TEST(test_buffer_putspace) {
}

/* int buffer_putspad(buffer*, const char* x, size_t pad); */
TEST(test_buffer_putspad) {
}

/* int buffer_putulong0(buffer*, unsigned long l, int pad); */
TEST(test_buffer_putulong0) {
}

/* int buffer_putulong(buffer*, unsigned long int l); */
TEST(test_buffer_putulong) {
}

/* int buffer_putulonglong(buffer*, uint64 l);
int buffer_putulonglong(buffer*, uint64 i); */
TEST(test_buffer_putulonglong) {
}

/* int buffer_putxlong0(buffer*, unsigned long l, int pad); */
TEST(test_buffer_putxlong0) {
}

/* int buffer_putxlong(buffer*, unsigned long int l); */
TEST(test_buffer_putxlong) {
}

/* int buffer_putxlonglong0(buffer*, uint64 l, int pad); */
TEST(test_buffer_putxlonglong0) {
}

/* int buffer_putxlonglong(buffer*, uint64 l); */
TEST(test_buffer_putxlonglong) {
}

/* int buffer_read_fd(buffer*, fd_t fd); */
TEST(test_buffer_read_fd) {
}

/* void buffer_seek(buffer*, size_t len); */
TEST(test_buffer_seek) {
}

/* int buffer_skipc(buffer* b); */
TEST(test_buffer_skipc) {
}

/* int buffer_skipn(buffer*, size_t n); */
TEST(test_buffer_skipn) {
}

/* int buffer_skip_pred(buffer*, int (*pred)(int)); */
TEST(test_buffer_skip_pred) {
}

/* int buffer_skipspace(buffer* b); */
TEST(test_buffer_skipspace) {
}

/* int buffer_skip_until(buffer*, const char* charset, size_t setlen); */
TEST(test_buffer_skip_until) {
}

/* int buffer_truncfile(buffer*, const char* fn); */
TEST(test_buffer_truncfile) {
}

/* int buffer_write_fd(buffer*, fd_t fd); */
TEST(test_buffer_write_fd) {
}

#define RUN_BUFFER_TESTS() \
  RUN(test_buffer_0); \
  RUN(test_buffer_0small); \
  RUN(test_buffer_1); \
  RUN(test_buffer_1small); \
  RUN(test_buffer_2); \
  RUN(test_buffer_bz2); \
  RUN(test_buffer_bzip); \
  RUN(test_buffer_close); \
  RUN(test_buffer_deflate); \
  RUN(test_buffer_dump); \
  RUN(test_buffer_feed); \
  RUN(test_buffer_flush); \
  RUN(test_buffer_free); \
  RUN(test_buffer_freshen); \
  RUN(test_buffer_fromarray); \
  RUN(test_buffer_frombuf); \
  RUN(test_buffer_fromsa); \
  RUN(test_buffer_get); \
  RUN(test_buffer_getc); \
  RUN(test_buffer_getline); \
  RUN(test_buffer_getn); \
  RUN(test_buffer_get_token); \
  RUN(test_buffer_get_token_pred); \
  RUN(test_buffer_get_until); \
  RUN(test_buffer_gzip); \
  RUN(test_buffer_inflate); \
  RUN(test_buffer_init); \
  RUN(test_buffer_init_free); \
  RUN(test_buffer_lzma); \
  RUN(test_buffer_mmapprivate); \
  RUN(test_buffer_mmapprivate_fd); \
  RUN(test_buffer_mmapread); \
  RUN(test_buffer_mmapread_fd); \
  RUN(test_buffer_mmapshared_fd); \
  RUN(test_buffer_munmap); \
  RUN(test_buffer_peek); \
  RUN(test_buffer_peekc); \
  RUN(test_buffer_prefetch); \
  RUN(test_buffer_put); \
  RUN(test_buffer_put8long); \
  RUN(test_buffer_putalign); \
  RUN(test_buffer_putc); \
  RUN(test_buffer_putdouble); \
  RUN(test_buffer_puterror); \
  RUN(test_buffer_puterror2); \
  RUN(test_buffer_putflush); \
  RUN(test_buffer_putlong); \
  RUN(test_buffer_putlong0); \
  RUN(test_buffer_putlonglong); \
  RUN(test_buffer_putm_internal); \
  RUN(test_buffer_putm_internal_flush); \
  RUN(test_buffer_putnc); \
  RUN(test_buffer_putnlflush); \
  RUN(test_buffer_putns); \
  RUN(test_buffer_putnspace); \
  RUN(test_buffer_putptr); \
  RUN(test_buffer_puts); \
  RUN(test_buffer_putsa); \
  RUN(test_buffer_putsaflush); \
  RUN(test_buffer_putsalign); \
  RUN(test_buffer_putsflush); \
  RUN(test_buffer_putspace); \
  RUN(test_buffer_putspad); \
  RUN(test_buffer_putulong); \
  RUN(test_buffer_putulong0); \
  RUN(test_buffer_putulonglong); \
  RUN(test_buffer_putxlong); \
  RUN(test_buffer_putxlong0); \
  RUN(test_buffer_putxlonglong); \
  RUN(test_buffer_putxlonglong0); \
  RUN(test_buffer_read_fd); \
  RUN(test_buffer_seek); \
  RUN(test_buffer_skipc); \
  RUN(test_buffer_skipn); \
  RUN(test_buffer_skip_pred); \
  RUN(test_buffer_skipspace); \
  RUN(test_buffer_skip_until); \
  RUN(test_buffer_truncfile); \
  RUN(test_buffer_write_fd);




TESTS(buffer) { RUN_BUFFER_TESTS(); }
