#include "../ini.h"
#include "../str.h"
#include "../fmt.h"
#include "../scan.h"
#include "../uint16.h"

typedef int(getchar_fn)(buffer*, int*);

static int
getchar_utf16(buffer* b, int* ptr) {
  char d[2];
  uint16 ch;
  int ret = buffer_get(b, d, 2);

  if(ret == 2) {
    uint16_unpack(d, &ch);
    *ptr = ch;
  }
  return ret;
}

static int
getchar_utf8(buffer* b, int* ptr) {
  char c;
  int ret = buffer_get(b, &c, 1);

  if(ret == 1)
    *ptr = c;
  return ret;
}

static int
getline_sa(buffer* b, stralloc* line, getchar_fn* getbyte) {
  int prev = -1, ch;
  stralloc_zero(line);

  while(getbyte(b, &ch) >= 1) {
    if(ch > 255) {
      char chars[4];
      stralloc_catb(line, chars, fmt_utf8(chars, ch));

    } else if(ch == '\\') {
      prev = ch;

      if(getbyte(b, &ch) <= 0)
        break;

      if(ch == '\n')
        continue;

      if(ch == 'n')
        ch = '\n';
      else if(ch == 'r')
        ch = '\r';
      else if(ch == 't')
        ch = '\t';
      stralloc_catc(line, ch);
    } else {
      stralloc_catc(line, ch);

      if(ch == '\n')
        break;
    }
    prev = ch;
  }
  return line->len;
}

void
ini_read(buffer* b, ini_section_t** ptr) {
  stralloc line;
  ini_section_t *s = 0, **tail = ptr;
  char* x;
  getchar_fn* getc_fn = &getchar_utf8;

  *ptr = NULL;

  if(buffer_prefetch(b, 2) < 2)
    return;

  x = buffer_peek(b);

  if(x[0] == '\377' && x[1] == '\376') {
    buffer_SKIP(b, 2);
    getc_fn = &getchar_utf16;
  } else if(x[1] == '\0') {
    getc_fn = &getchar_utf16;
  }

  for(stralloc_init(&line); getline_sa(b, &line, getc_fn); stralloc_zero(&line)) {
    size_t i, e;

    stralloc_trimr(&line, "\r\n", 2);
    stralloc_nul(&line);

    i = scan_whitenskip(line.s, line.len);

    if(i == line.len)
      continue;

    if(line.s[i] == ';')
      continue;

    if(line.s[i] == '[') {
      i++;
      e = byte_chr(&line.s[i], line.len - i, ']');
      s = ini_newb(tail, &line.s[i], e);
      tail = &s->next;
      continue;
    }

    if(s == 0)
      continue;

    e = byte_chr(&line.s[i], line.len - i, '=');

    if(i + e < line.len) {
      line.s[i + e] = '\0';
      e++;
      MAP_INSERT2(s->map, &line.s[i], e - i - 1, &line.s[i + e], line.len - (i + e));

      /*buffer_putsa(buffer_2, &line);
      buffer_putnlflush(buffer_2);*/
    }
  }
}
