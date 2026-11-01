#include "../ihex.h"
#include "../alloc.h"
#include "../byte.h"

/* Removes the data below address `addr`: records that end at or before it are
 * deleted, a record that crosses it loses its first bytes. Returns the number
 * of data bytes removed. This is what the sdcc build needs for a program that
 * is loaded behind a bootloader (its crt0 puts a reset vector at 0 whatever
 * the linker script says) and replaces an awk script that only dropped whole
 * records. */
size_t
ihex_strip_below(ihex_file* ihf, uint32 addr) {
  struct list_head *el, *next;
  uint32 base = 0;
  size_t removed = 0;

  for(el = ihf->records.next; el != &ihf->records; el = next) {
    ihex_record* r = (ihex_record*)el;
    uint32 a, cut;

    next = el->next;
    if(ihex_record_base(r, &base) || r->type != IHEX_DATA)
      continue;
    a = base + r->offset;
    if(a >= addr)
      continue;
    if(a + r->length <= addr) {
      removed += r->length;
      list_del(el);
      alloc_free(r);
      continue;
    }
    cut = addr - a; /* the record crosses addr: keep its tail */
    byte_copy(r->data, r->length - cut, r->data + cut);
    r->offset += cut;
    r->length -= cut;
    r->checksum = ihex_record_checksum(r);
    removed += cut;
  }
  return removed;
}
