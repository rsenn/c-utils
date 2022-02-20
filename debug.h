#ifndef DEBUG_H
#define DEBUG_H

extern const char* debug_nl;

void debug_byte(const char*, const char*, size_t);
void debug_int(const char*, int);
void debug_long(const char*, long);
void debug_str(const char*, const char*);

#ifdef STRALLOC_H
void debug_sa(const char*, stralloc*);
#endif
#ifdef SET_H
void debug_set(const char*, const set_t*, const char*);
#endif
#ifdef STRLIST_H

void debug_sl(const char* name, const strlist* l, const char* sep);
#endif
#ifdef STRARRAY_H
void debug_strarray(const char*, const strarray*);
#endif

#endif
