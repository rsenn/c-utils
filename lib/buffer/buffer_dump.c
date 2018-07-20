#include "../io_internal.h"
#include "../buffer.h"
#include "../fmt.h"

extern ssize_t buffer_dummyreadmmap();
extern unsigned long stralloc_write();

void
buffer_dump(buffer* out, buffer* b) {
  char xlong[FMT_LONG + FMT_LONG + FMT_LONG];
  unsigned long n;

  buffer_puts(out, "[ x=0x");

  buffer_putxlong(out, (unsigned long)out->x);

  buffer_puts(out, ",fd=");
  if(b->op == (void*)stralloc_write) {

    buffer_puts(out, "*sa");
  } else {
    n = fmt_long(xlong, b->fd);

    buffer_putnspace(out, 3 - n);

    buffer_put(out, xlong, n);
  }

  buffer_putspace(out);

  buffer_puts(out, "p=");

  buffer_putulong(out, b->p);

  buffer_puts(out, ",n=");

  buffer_putulong(out, b->n);

  buffer_puts(out, ",a=");

  buffer_putulong(out, b->a);

  buffer_putspace(out);
  if(b->op == (void*)read)

    buffer_puts(out, "<read>  ");
  else if(b->op == (void*)write)

    buffer_puts(out, "<write> ");
  else if(b->op == buffer_dummyreadmmap)

    buffer_puts(out, "<mmap>  ");
  else if(b->op == (void*)stralloc_write)

    buffer_puts(out, "<sa-wr> ");
  else if(b->op == NULL)

    buffer_puts(out, "NULL    ");
  else {
    n = fmt_xlong(xlong, (long long)(intptr_t)b->op);

    buffer_put(out, xlong, n);
  }

  buffer_puts(out, " ]");
}
