#ifndef DEBUG_H
#define DEBUG_H

#include "buffer.h"
#include "str.h"
#include "stralloc.h"
#include "strarray.h"
#include "strlist.h"
#include "set.h"

/* diagnostics go to debug_buf, which defaults to debug_buffer on fd 2 */
extern buffer debug_buffer, *debug_buf;

/* line terminator that debug_byte and debug_set write */
extern const char* debug_nl;

/* name: x[n] */
void debug_byte(const char* name, const char* x, size_t n);

/* name: i */
void debug_int(const char* name, int i);

/* name: value */
void debug_long(const char* name, long value);

/* name: 's', or name: NULL */
void debug_str(const char* name, const char* s);

/* name: sa */
void debug_sa(const char* name, stralloc* sa);

/* name: the elements of l */
void debug_sl(const char* name, const strlist* l, const char* sep);

/* name: the elements of stra */
void debug_strarray(const char* name, const strarray* stra);

/* name: the members of s, joined by sep */
static inline void
debug_set(const char* name, const set_t* s, const char* sep) {
  buffer_putm_internal(debug_buf, name, ": ", NULL);
  buffer_putset(debug_buf, s, sep, str_len(sep));
  buffer_putsflush(debug_buf, debug_nl);
}

#endif /* defined DEBUG_H */
