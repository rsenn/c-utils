#include "ini.h"
#include "lib/str.h"
#include "lib/fmt.h"
#include "lib/alloc.h"
#include "lib/stralloc.h"
#include "lib/buffer.h"
#include "lib/uint16.h"
#include "lib/scan.h"

ssize_t
buffer_write_utf16le(fd_t fd, void* buf, size_t len, void* arg) {
  size_t i = 0;
  ssize_t r = 0;
  unsigned int ch;
  buffer* b = arg;
  while(i < len) {
    char x[2];
    size_t n = scan_utf8(&((char*)buf)[i], len - i, &ch);
    if(n > 0) {
      uint16_pack(x, ch);
    } else {
      uint16_pack(x, ((char*)buf)[i]);
      n = 1;
    }
    buffer_put(b->cookie, x, 2);

    i += n;
    r += 2;
  }
  return i;
}

const char*
ini_get(ini_section_t* ini, const char* key) {
  return MAP_GET(ini->map, (char*)key, str_len(key) + 1);
}

void
ini_set(ini_section_t* ini, const char* key, const char* value) {
  MAP_INSERT(ini->map, (char*)key, str_len(key) + 1, (char*)value, str_len(value) + 1);
}

void
ini_set_sa(ini_section_t* ini, stralloc* key, stralloc* value) {
  stralloc_nul(key);
  stralloc_nul(value);
  ini_set(ini, key->s, value->s);
}

void
ini_set_long(ini_section_t* ini, const char* key, long value) {
  char buf[FMT_LONG + 1];
  size_t n;

  n = fmt_long(buf, value);
  buf[n] = '\0';
  return ini_set(ini, key, buf);
}

ini_section_t*
ini_newb(ini_section_t** ptr, const char* name, size_t namelen) {
  ini_section_t* ini = alloc(sizeof(ini_section_t));

  ini_init(ini);

  MAP_NEW(ini->map);

  stralloc_init(&ini->name);
  stralloc_copyb(&ini->name, name, namelen);

  ini->next = NULL;
  *ptr = ini;

  return ini;
}

ini_section_t*
ini_new(ini_section_t** ptr, const char* name) {
  return ini_newb(ptr, name, str_len(name));
}

static int utf16 = 0;

void
ini_write(buffer* b, ini_section_t* ini, int utf16) {
  buffer out;
  char x[1024];
  buffer_init(&out, (buffer_op_sys*)(void*)&buffer_write_utf16le, 0, x, sizeof(x));
  out.cookie = b;
  if(utf16) {
    buffer_putsflush(b, "\377\376");
    b = &out;
  }
  while(ini) {

    MAP_PAIR_T t;
    buffer_putc(b, '[');
    buffer_putsa(b, &ini->name);
    buffer_put(b, "]\r\n", 3);
    MAP_FOREACH(ini->map, t) {
      buffer_put(b, MAP_KEY(t), str_len(MAP_KEY(t)));
      buffer_putc(b, '=');
      buffer_put(b, MAP_VALUE(t), str_len(MAP_VALUE(t)));
      buffer_puts(b, "\r\n");
    }
    ini = ini->next;
  }
  buffer_flush(b);
}

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
  ini_section_t* s = 0;
  char* x;
  getchar_fn* getc_fn = &getchar_utf8;

  *ptr = NULL;
  if(buffer_prefetch(b, 2) < 2)
    return;

  x = buffer_peek(b);

  if(x[0] == '\377' && x[1] == '\376') {
    buffer_skipn(b, 2);
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
      s = ini_newb(ptr, &line.s[i], e);
      continue;
    }

    if(s == 0)
      continue;

    e = byte_chr(&line.s[i], line.len - i, '=');

    if(i + e < line.len) {
      line.s[i + e] = '\0';
      e++;
      MAP_INSERT(s->map, &line.s[i], e - i - 1, &line.s[i + e], line.len - (i + e));

      buffer_putsa(buffer_2, &line);
      buffer_putnlflush(buffer_2);
    }
  }
}
