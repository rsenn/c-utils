#include "../ini.h"
#include "../str.h"
#include "../uint16.h"
#include "../scan.h"

/* buffer op writing the utf-8 text it is given to cookie as utf-16le */
static ssize_t
ini_write_utf16le(fd_type fd, void* buf, size_t len, void* arg) {
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

void
ini_write(buffer* b, ini_section_t* ini, int utf16) {
  buffer out;
  char x[1024];
  buffer_init(&out, (buffer_op_proto*)(void*)&ini_write_utf16le, 0, x, sizeof(x));
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
    for(t = ini->map.buckets; t < &ini->map.buckets[ini->map.capacity]; ++t) {
      if(!t->key || t->key == HASHMAP_TOMBSTONE)
        continue;

      buffer_put(b, MAP_KEY(t), str_len(MAP_KEY(t)));
      buffer_putc(b, '=');
      buffer_put(b, MAP_VALUE(t), str_len(MAP_VALUE(t)));
      buffer_puts(b, "\r\n");
    }
    ini = ini->next;
  }
  buffer_flush(b);
}
