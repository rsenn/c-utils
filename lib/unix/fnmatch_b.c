#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <ctype.h>
#include <stdlib.h>
#include "../fnmatch.h"
#include "../str.h"

#define NOTFIRST 128

#define STRUCT_CHARCLASS(c)                                                                        \
  { #c, is##c }

static struct charclass {
  char* class;
  int (*istype)(int);
} allclasses[] = {
    STRUCT_CHARCLASS(alnum),
    STRUCT_CHARCLASS(alpha),
    STRUCT_CHARCLASS(ascii),
    /*    STRUCT_CHARCLASS(blank),*/
    STRUCT_CHARCLASS(cntrl),
    /*    STRUCT_CHARCLASS(csym),
        STRUCT_CHARCLASS(csymf),*/
    STRUCT_CHARCLASS(digit),
    STRUCT_CHARCLASS(graph),
    STRUCT_CHARCLASS(lower),
    STRUCT_CHARCLASS(print),
    STRUCT_CHARCLASS(punct),
    STRUCT_CHARCLASS(space),
    STRUCT_CHARCLASS(upper),
    STRUCT_CHARCLASS(xdigit),
};

/* look for "class:]" in pattern */
static struct charclass*
charclass_lookup(const char* pattern) {
  unsigned int i;

  for(i = 0; i < sizeof(allclasses) / sizeof(*allclasses); i++) {
    int len = str_len(allclasses[i].class);
    if(!str_diffn(pattern, allclasses[i].class, len)) {
      pattern += len;
      if(str_diffn(pattern, ":]", 2))
        goto noclass;
      return &allclasses[i];
    }
  }
noclass:
  return NULL;
}

static int
match(char c, char d, int flags) {
  return (flags & FNM_CASEFOLD) ? tolower(c) == tolower(d) : c == d;
}

int
fnmatch_b(const char* p, size_t plen, const char* s, size_t slen, int flags) {
  size_t i = 0, j = 0;
  if(s[j] == 0) {
    while(p[i] == '*') ++i;
    return i < plen ? FNM_NOMATCH : 0;
  }
  if(s[j] == '.' && p[i] != '.' && (flags & FNM_PERIOD)) {
    /* don't match if FNM_PERIOD and this is the first char */
    if(!(flags & NOTFIRST))
      return FNM_NOMATCH;
    /* don't match if FNM_PERIOD and FNM_PATHNAME and previous was '/' */
    if((flags & (FNM_PATHNAME)) && s[-1] == '/')
      return FNM_NOMATCH;
  }
  flags |= NOTFIRST;
  switch(p[i]) {
    case '[': {
      int neg = 0;
      const char* start; /* first member of character class */

      ++i;
      if(s[j] == '/' && flags & FNM_PATHNAME)
        return FNM_NOMATCH;
      if(p[i] == '!') {
        neg = 1;
        ++i;
      }
      start = p;
      while(p[i]) {
        int res = 0;

        if(p[i] == ']' && p != start)
          break;
        if(p[i] == '[' && p[1] == ':') {
          /* MEMBER - stupid POSIX char classes */
          const struct charclass* cc;

          if(!(cc = charclass_lookup(p + 2)))
            goto invalidclass;
          p += str_len(cc->class) + 4;
          if(flags & FNM_CASEFOLD && (cc->istype == isupper || cc->istype == islower)) {
            res = islower(tolower(s[j]));
          } else {
            res = ((*(cc->istype))(s[j]));
          }
        } else {
        invalidclass:
          if(p[1] == '-' && p[2] != ']') {
            /* MEMBER - character range */
            if(s[j] >= p[i] && s[j] <= p[2])
              res = 1;
            if(flags & FNM_CASEFOLD) {
              if(tolower(s[j]) >= tolower(p[i]) && tolower(s[j]) <= tolower(p[2]))
                res = 1;
            }
            p += 3;
          } else {
            /* MEMBER - literal character match */
            res = match(p[i], s[j], flags);
            ++i;
          }
        }
        if((res && !neg) || ((neg && !res) && p[i] == ']')) {
          while(p[i] && p[i] != ']') ++i;
          return fnmatch_b(p + !!p[i], plen - !!p[i], s + 1, slen - 1, flags);
        } else if(res && neg)
          return FNM_NOMATCH;
      }
    } break;
    case '\\':
      if(flags & FNM_NOESCAPE) {
        if(s[j] == '\\')
          return fnmatch_b(p + 1, plen - 1, s + 1, slen - 1, flags);
      } else {
        if(s[j] == p[1])
          return fnmatch_b(p + 2, plen - 2, s + 1, slen - 1, flags);
      }
      break;
    case '*':
      if((s[j] == '/' && flags & FNM_PATHNAME) || fnmatch_b(p, plen, s + 1, slen + 1, flags))
        return fnmatch_b(p + 1, plen - 1, s, slen - 1, flags);
      return 0;
    case 0:
      if(*s == 0 || (*s == '/' && (flags & FNM_LEADING_DIR)))
        return 0;
      break;
    case '?': {
      if(s[j] == '/' && flags & FNM_PATHNAME)
        break;
    }
      return fnmatch_b(p + 1, plen - 1, s + 1, slen - 1, flags);
    default: {
      if(match(p[i], s[j], flags))
        return fnmatch_b(p + 1, plen - 1, s + 1, slen - 1, flags);
      break;
    }
  }
  return FNM_NOMATCH;
}
