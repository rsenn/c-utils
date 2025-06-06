#ifndef DEBUG_H
#define DEBUG_H

#include "lib/buffer.h"
#include "lib/str.h"

extern buffer debug_buffer, *debug_buf;
extern const char* debug_nl;

void debug_byte(const char*, const char*, size_t);
void debug_int(const char*, int);
void debug_long(const char*, long);
void debug_str(const char*, const char*);

#ifdef STRALLOC_H
void debug_sa(const char*, stralloc*);
#endif

#ifdef SET_H
static inline void
debug_set(const char* name, const set_t* s, const char* sep) {
  buffer_putm_internal(debug_buf, name, ": ", NULL);
  buffer_putset(debug_buf, s, sep, str_len(sep));
  buffer_putsflush(debug_buf, debug_nl);
}
#endif

#ifdef STRLIST_H
void debug_sl(const char* name, const strlist* l, const char* sep);
#endif

#ifdef STRARRAY_H
void debug_strarray(const char*, const strarray*);
#endif

#endif
