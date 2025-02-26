#include "../ihex.h"
#include "../slist.h"
#include "../uint32.h"
#include "../byte.h"

size_t
ihex_read_at(ihex_file* ihf, uint32 at, char* x, size_t n) {
  ihex_record* rec;
  char* start = x;
  union {
    uint32 off32;
    struct {
      uint16 lo16;
      uint16 hi16;
    };
  } o;

  if((rec = ihex_record_at(ihf, at, &o.off32))) {
    uint32 p = at - o.off32;
    uint32 end;

    while(rec && n > 0) {
      size_t num = rec->length - p;

      if(n < num)
        num = n;
      byte_copy(x, num, &rec->data[p]);
      n -= num;
      x += num;
      end = o.off32 + rec->length;
      rec = (ihex_record*)rec->link.next;

      if(!rec)
        break;
      o.lo16 = rec->offset;
      p = 0;

      while(n > 0 && end++ < o.off32) {
        *x++ = '\0';
        n--;
      }
    }
  }
  return x - start;
}
