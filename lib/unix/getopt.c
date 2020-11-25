/*
 * getopt - POSIX like getopt for Windows console Application
 *
 * win-c - Windows Console Library
 * Copyright (c) 2015 Koji Takami
 * Released under the MIT license
 * https://github.com/takamin/win-c/blob/master/LICENSE
 */

#if 1 //! defined(HAVE_GETOPT) || !defined(HAVE_GETOPT_LONG)

#include "../getopt.h"
#include "../buffer.h"
#include "../str.h"

#include <stdio.h>
#include <string.h>

#if 1 // def HAVE_GETOPT
#define getopt unix_getopt
#define getopt_long unix_getopt_long
#define getopt_real unix_getopt_real
#define optind unix_optind
#define optarg unix_optarg
#define opterr unix_opterr
#define optopt unix_optopt
#define optbuf unix_optbuf
#endif

char* optarg = 0;
int optind = 1;
int opterr = 1;
int optopt = 0;
buffer* optbuf = NULL;

int postpone_count = 0;
int nextchar = 0;

static void
postpone(int argc, char* const argv[], int index) {
  char** nc_argv = (char**)argv;
  char* p = nc_argv[index];
  int j = index;
  for(; j < argc - 1; j++) {
    nc_argv[j] = nc_argv[j + 1];
  }
  nc_argv[argc - 1] = p;
}

static int
postpone_noopt(int argc, char* const argv[], int index) {
  int i = index;
  for(; i < argc; i++) {
    if(*(argv[i]) == '-') {
      postpone(argc, argv, index);
      return 1;
    }
  }
  return 0;
}

static int
getopt_real(int argc,
            char* const argv[],
            const char* optstring,
            const struct longopt* longopts,
            int* longindex) {
  if(optbuf == NULL)
    optbuf = buffer_2;
  while(1) {
    int c;
    const char* optptr = 0;
    if(optind >= argc - postpone_count) {
      c = 0;
      optarg = 0;
      break;
    }
    c = *(argv[optind] + nextchar);
    if(c == '\0') {
      nextchar = 0;
      ++optind;
      continue;
    }
    if(nextchar == 0) {
      if(optstring[0] != '+' && optstring[0] != '-') {
        while(c != '-') {
          /* postpone non-opt parameter */
          if(!postpone_noopt(argc, argv, optind)) {
            break; /* all args are non-opt param */
          }
          ++postpone_count;
          c = *argv[optind];
        }
      }
      if(c != '-') {
        if(optstring[0] == '-') {
          optarg = argv[optind];
          nextchar = 0;
          ++optind;
          return 1;
        }
        break;
      } else {
        if(str_diff(argv[optind], "--") == 0) {
          optind++;
          break;
        }
        ++nextchar;
        if(longopts != 0 && *(argv[optind] + 1) == '-') {
          char const* spec_long = argv[optind] + 2;
          size_t pos_eq = str_chr(spec_long, '=');
          ssize_t spec_len = pos_eq;
          int index_search = 0, index_found = -1;
          const struct longopt* optdef = 0;
          if(spec_long[pos_eq] == '\0')
            pos_eq = 0;
          while(longopts->name != 0) {
            if(str_diffn(spec_long, longopts->name, spec_len) == 0) {
              if(optdef != 0) {
                if(opterr) {
                  buffer_putm_internal(optbuf, "ambiguous option: ", spec_long, 0);
                  buffer_putnlflush(optbuf);
                }
                return '?';
              }
              optdef = longopts;
              index_found = index_search;
            }
            longopts++;
            index_search++;
          }
          if(optdef == 0) {
            if(opterr) {
              buffer_putm_internal(optbuf, "no such option: ", spec_long, 0);
              buffer_putnlflush(optbuf);
            }
            return '?';
          }
          switch(optdef->has_arg) {
            case no_argument:
              optarg = 0;
              if(pos_eq != 0) {
                if(opterr) {
                  buffer_putm_internal(optbuf, "no argument for ", optdef->name, 0);
                  buffer_putnlflush(optbuf);
                }
                return '?';
              }
              break;
            case required_argument:
              if(spec_long[pos_eq] == '\0') {
                ++optind;
                optarg = argv[optind];
              } else {
                optarg = (char*)pos_eq + 1;
              }
              break;
          }
          ++optind;
          nextchar = 0;
          if(longindex != 0) {
            *longindex = index_found;
          }
          if(optdef->flag != 0) {
            *optdef->flag = optdef->val;
            return 0;
          }
          return optdef->val;
        }
        continue;
      }
    }
    optptr = strchr(optstring, c);
    if(optptr == NULL) {
      optopt = c;
      if(opterr) {
        buffer_putm_internal(optbuf, argv[0], ": invalid option -- ", 0);
        buffer_PUTC(optbuf, (char)c);
        buffer_putnlflush(optbuf);
      }
      ++nextchar;
      return '?';
    }
    if(*(optptr + 1) != ':') {
      nextchar++;
      if(*(argv[optind] + nextchar) == '\0') {
        ++optind;
        nextchar = 0;
      }
      optarg = 0;
    } else {
      nextchar++;
      if(*(argv[optind] + nextchar) != '\0') {
        optarg = argv[optind] + nextchar;
      } else {
        ++optind;
        if(optind < argc - postpone_count) {
          optarg = argv[optind];
        } else {
          optopt = c;
          if(opterr) {
            buffer_putm_internal(optbuf, argv[0], ": option requires an argument -- ", 0);
            buffer_PUTC(optbuf, (char)c);
            buffer_putnlflush(optbuf);
          }
          if(optstring[0] == ':' ||
             ((optstring[0] == '-' || optstring[0] == '+') && optstring[1] == ':')) {
            c = ':';
          } else {
            c = '?';
          }
        }
      }
      ++optind;
      nextchar = 0;
    }
    return c;
  }

  /* end of option analysis */

  /* fix the order of non-opt params to original */
  while((argc - optind - postpone_count) > 0) {
    postpone(argc, argv, optind);
    ++postpone_count;
  }

  nextchar = 0;
  postpone_count = 0;
  return -1;
}

#if 1 // ndef HAVE_GETOPT
int
getopt(int argc, char* const argv[], const char* optstring) {
  return getopt_real(argc, argv, optstring, 0, 0);
}
#endif

#if 1 // ndef HAVE_GETOPT_LONG
int
getopt_long(int argc,
            char* const argv[],
            const char* optstring,
            const struct longopt* longopts,
            int* longindex) {
  return getopt_real(argc, argv, optstring, longopts, longindex);
}
#endif

/********************************************************
int getopt_long_only(int argc, char* const argv[],
        const char* optstring,
        const struct longopt* longopts, int* longindex)
{
    return -1;
}
********************************************************/
#endif /* !defined(HAVE_GETOPT) || !defined(HAVE_GETOPT_LONG) */
