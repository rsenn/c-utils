#include "mmap.h"
#include "buffer.h"

char hexdigits[] = "0123456789abcdef";

size_t n;
unsigned char* p;

void
print(unsigned char* p, size_t i, unsigned char from, unsigned char to) {
  buffer_puts(buffer_2, "0x");
  buffer_putxlong(buffer_2, i);
  buffer_putspace(buffer_2);
  buffer_puts(buffer_2, "0x");
  buffer_putc(buffer_2, hexdigits[from >> 4]);
  buffer_putc(buffer_2, hexdigits[from & 0x0f]);
  buffer_putspace(buffer_2);
  buffer_puts(buffer_2, "0x");
  buffer_putc(buffer_2, hexdigits[to >> 4]);
  buffer_putc(buffer_2, hexdigits[to & 0x0f]);
}

void
patch(unsigned char* p, size_t i, unsigned char from, unsigned char to) {
    buffer_puts(buffer_2, "patch ");
    print(p, i, from, to);
  if(p[i] == from) {
    p[i] = to;
    buffer_puts(buffer_2, " ok");
  } else {
    buffer_puts(buffer_2, " fail");
  }
  buffer_putnlflush(buffer_2);
}

int
main(int argc, char* argv[]) {
	
  p = mmap_private(argv[1], &n);
	
	patch(p, 0x00076608, 0x90, 0xeb);
  patch(p, 0x0023f89c, 0xdf, 0x75);
  patch(p, 0x00076626, 0x8b, 0x75);
  patch(p, 0x000b23c9, 0x10, 0x85);
  patch(p, 0x00076609, 0x48, 0x2a);

  mmap_unmap(p, n); 
}