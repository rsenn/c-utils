

/* Copyright (C) 1991, 1992, 1993 Free Software Foundation, Inc.

   NOTE: The canonical source of this file is maintained with the GNU C
   Library.  Bugs can be reported to bug-glibc@prep.ai.mit.edu.

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#include "../fnmatch.h"
#include <ctype.h>
#include <errno.h>

#if defined(STDC_HEADERS) || !defined(isascii)
#define ISASCII(c) 1
#else
#define ISASCII(c) isascii(c)
#endif

#define ISUPPER(c) (ISASCII(c) && isupper(c))

#define fnmatch unix_fnmatch

/* Match STRING against the filename pattern PATTERN, returning zero if
   it matches, nonzero if not.  */
int
fnmatch(const char* pattern, const char* string, int flags) {
  const char *p = pattern, *n = string;
  char c;

  /* Note that this evalutes C many times.  */
#define FOLD(c) ((flags & FNM_CASEFOLD) && ISUPPER(c) ? tolower(c) : (c))

  while((c = *p++) != '\0') {
    c = FOLD(c);

    switch(c) {
      case '?':
        if(*n == '\0')
          return FNM_NOMATCH;
        else if((flags & FNM_FILE_NAME) && *n == '/')
          return FNM_NOMATCH;
        else if((flags & FNM_PERIOD) && *n == '.' && (n == string || ((flags & FNM_FILE_NAME) && n[-1] == '/')))
          return FNM_NOMATCH;
        break;

      case '\\':
        if(!(flags & FNM_NOESCAPE)) {
          c = *p++;
          c = FOLD(c);
        }

        if(FOLD(*n) != c)
          return FNM_NOMATCH;
        break;

      case '*':
        if((flags & FNM_PERIOD) && *n == '.' && (n == string || ((flags & FNM_FILE_NAME) && n[-1] == '/')))
          return FNM_NOMATCH;

        for(c = *p++; c == '?' || c == '*'; c = *p++, ++n)

          if(((flags & FNM_FILE_NAME) && *n == '/') || (c == '?' && *n == '\0'))
            return FNM_NOMATCH;

        if(c == '\0')
          return 0;

        {
          char c1 = (!(flags & FNM_NOESCAPE) && c == '\\') ? *p : c;
          c1 = FOLD(c1);

          for(--p; *n != '\0'; ++n)

            if((c == '[' || FOLD(*n) == c1) && fnmatch(p, n, flags & ~FNM_PERIOD) == 0)
              return 0;
          return FNM_NOMATCH;
        }

      case '[': {
        /* Nonzero if the sense of the character class is inverted.  */
        int not ;

        if(*n == '\0')
          return FNM_NOMATCH;

        if((flags & FNM_PERIOD) && *n == '.' && (n == string || ((flags & FNM_FILE_NAME) && n[-1] == '/')))
          return FNM_NOMATCH;

        not = (*p == '!' || *p == '^');

        if(not )
          ++p;

        c = *p++;

        for(;;) {
          char cstart = c, cend = c;

          if(!(flags & FNM_NOESCAPE) && c == '\\')
            cstart = cend = *p++;

          cstart = cend = FOLD(cstart);

          if(c == '\0') /* [ (unterminated) loses.  */
            return FNM_NOMATCH;

          c = *p++;
          c = FOLD(c);

          if((flags & FNM_FILE_NAME) && c == '/') /* [/] can never match.  */
            return FNM_NOMATCH;

          if(c == '-' && *p != ']') {
            cend = *p++;

            if(!(flags & FNM_NOESCAPE) && cend == '\\')
              cend = *p++;

            if(cend == '\0')
              return FNM_NOMATCH;
            cend = FOLD(cend);

            c = *p++;
          }

          if(FOLD(*n) >= cstart && FOLD(*n) <= cend)
            goto matched;

          if(c == ']')
            break;
        }

        if(!not )
          return FNM_NOMATCH;
        break;

      matched:;
        /* Skip the rest of the [...] that already matched.  */

        while(c != ']') {
          if(c == '\0') /* [... (unterminated) loses.  */
            return FNM_NOMATCH;

          c = *p++;

          if(!(flags & FNM_NOESCAPE) && c == '\\') /* XXX 1003.2d11 is unclear if this is right.  */
            ++p;
        }

        if(not )
          return FNM_NOMATCH;
      } break;

      default:

        if(c != FOLD(*n))
          return FNM_NOMATCH;
    }

    ++n;
  }

  if(*n == '\0')
    return 0;

  if((flags & FNM_LEADING_DIR) && *n == '/') /* The FNM_LEADING_DIR flag says that "foo*" matches
                                                "foobar/frobozz".  */
    return 0;

  return FNM_NOMATCH;

#undef FOLD
}
