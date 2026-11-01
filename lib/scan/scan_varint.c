#include "../scan.h"

size_t
scan_varint(const char* in, size_t len, unsigned long long* n) {
  size_t i;
  unsigned long long l;

  if(len == 0)
    return 0;

  for(l = 0, i = 0; i < len; ++i) {
    if(i * 7 + 7 > sizeof(l) * 8) /* value too large to fit in destination integer */
      return 0;
    l += (unsigned long long)(in[i] & 0x7f) << (i * 7);

    if(!(in[i] & 0x80)) {
      if(in[i] == 0)
        return 0; /* libowfat: reject non-minimal encoding */
      *n = l;
      return i + 1;
    }
  }
  return 0;
}

/* the value of a varint of the types bool, enum, int32, uint32, int64, uint64 */
size_t
scan_pb_type0_int(const char* dest, size_t len, unsigned long long* l) {
  return scan_varint(dest, len, l);
}
