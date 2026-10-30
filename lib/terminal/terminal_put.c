#include "../terminal_internal.h"
#include "../byte.h"

#if WINDOWS_NATIVE
#define CHUNK 1024

/* complete_len is the length of the prefix of s[n] that does not end in
 * the middle of a UTF-8 sequence. */
static size_t
complete_len(const char* s, size_t n) {
  size_t i = n, back = 0;
  unsigned char c;

  while(i > 0 && back < 3 && ((unsigned char)s[i - 1] & 0xc0) == 0x80) {
    --i;
    ++back;
  }
  if(i == 0)
    return n;
  c = (unsigned char)s[i - 1];
  if(c >= 0xc0 && back + 1 < (c >= 0xf0 ? 4u : c >= 0xe0 ? 3u : 2u))
    return i - 1;
  return n;
}
#endif

void
terminal_put(struct terminal* t, const char* s, size_t n) {
#if WINDOWS_NATIVE
  char buf[CHUNK + 4];
  WCHAR wide[CHUNK + 4];
  size_t m = t->ntail, take, c;
  DWORD done;
  int w;

  byte_copy(buf, m, t->tail);

  while(n) {
    take = n < CHUNK ? n : CHUNK;
    byte_copy(buf + m, take, s);
    m += take;
    s += take;
    n -= take;
    c = complete_len(buf, m);

    if(c && (w = MultiByteToWideChar(CP_UTF8, 0, buf, (int)c, wide, CHUNK + 4)) > 0)
      WriteConsoleW((HANDLE)t->handle, wide, (DWORD)w, &done, 0);
    m -= c;
    byte_copy(buf, m, buf + c);
  }
  byte_copy(t->tail, m, buf);
  t->ntail = (unsigned)m;
#else
  buffer_put(t->out, s, n);
#endif
}
