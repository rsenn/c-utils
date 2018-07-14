#include "stdint.h"
#include "stralloc.h"
#include "sa_stream.h"
#include "error.h"
#include "char.h"

unsigned int stralloc_qp_decode(stralloc *out, const stralloc *in)
{
  static stralloc tmp = STRALLOC;
  sa_stream stream;
  unsigned int state;
  byte_t nibble;
  char value;
  char ch;

  if(!in->len) return stralloc_erase(out);
  if(!stralloc_erase(&tmp)) return 0;
  sa_stream_init(&stream, in);
  state = 0;
  while(sa_stream_getc(&stream, &ch)) {
    switch (state) {
      case 0:
        if(ch == '=') {  /* escape char */
          state = 1;
          break;
        }
        if(!char_isprint(ch) && ch != '\t') {
	  errno = error_proto;
	  return 0;
        }
        if(!stralloc_append(&tmp, &ch)) return 0;
        break;
      case 1:  /* first hex digit */
        nibble = char_hex_table[(byte_t)ch];
        if(nibble > 15) {
	  errno = error_proto;
	  return 0;
        }
        value = (char)(nibble << 4);
        state = 2;
        break;
      case 2:  /* first hex digit */
        nibble = char_hex_table[(byte_t)ch];
        if(nibble > 15) {
	  errno = error_proto;
	  return 0;
        }
        value |= (char)nibble;
        if(!stralloc_append(&tmp, &value)) return 0;
        state = 0;
        break;
      default:  /* never happens */
	errno = error_proto;
	return 0;
    }
  }
  if(state) {
    errno = error_proto;
    return 0;
  }
  return stralloc_copy(out, &tmp);
}
