#include "../likely.h"
#include "../rangecheck.h"
#include "../typedefs.h"
#include "../uint16.h"

/* does an UTF-16 string starting at "ptr" lie in buf[0..len-1]? */
int
range_str2inbuf(const void* buf, size_t len, const void* stringstart) {
  const uint16* x;
  const uint16* y;
  if(!range_ptrinbuf(buf, len, x = stringstart))
    return 0;
  y = (const uint16*)((char*)x + len);
  for(; x + 1 <= y && *x; ++x)
    ;
  return (x + 1 <= y);
}

