/*
 * getopt - POSIX like getopt for Windows console Application
 *
 * win-c - Windows Console Library
 * Copyright (c) 2015 Koji Takami
 * Released under the MIT license
 * https://github.com/takamin/win-c/blob/master/LICENSE
 */

#include "../getopt.h"
#include "../buffer.h"
#include "../str.h"

#include <stdio.h>
#include <string.h>

char* unix_optarg = 0;
int unix_optind = 1;
int unix_opterr = 1;
int unix_optopt = 0;
buffer* unix_optbuf = NULL;

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
getopt_real(int argc, char* const argv[], const char* optstring, const struct unix_longopt* longopts, int* longindex) {
  if(unix_optbuf == NULL)
    unix_optbuf = buffer_2;

  for(;;) {
    int c;
    const char* optptr = 0;

    if(unix_optind >= argc - postpone_count) {
      c = 0;
      unix_optarg = 0;
      break;
    }
    c = *(argv[unix_optind] + nextchar);

    if(c == '\0') {
      nextchar = 0;
      ++unix_optind;
      continue;
    }

    if(nextchar == 0) {
      if(optstring[0] != '+' && optstring[0] != '-') {
        while(c != '-') {
          /* postpone non-opt parameter */

          if(!postpone_noopt(argc, argv, unix_optind)) {
            break; /* all args are non-opt param */
          }
          ++postpone_count;
          c = *argv[unix_optind];
        }
      }

      if(c != '-') {
        if(optstring[0] == '-') {
          unix_optarg = argv[unix_optind];
          nextchar = 0;
          ++unix_optind;
          return 1;
        }
        break;
      } else {
        if(str_diff(argv[unix_optind], "--") == 0) {
          unix_optind++;
          break;
        }
        ++nextchar;

        if(longopts != 0 && *(argv[unix_optind] + 1) == '-') {
          char const* spec_long = argv[unix_optind] + 2;
          size_t pos_eq = str_chr(spec_long, '=');
          ssize_t spec_len = pos_eq;
          int index_search = 0, index_found = -1;
          const struct unix_longopt* optdef = 0;

          if(spec_long[pos_eq] == '\0')
            pos_eq = 0;

          while(longopts->name != 0) {
            if(str_diff(spec_long, longopts->name) == 0 || (spec_long[pos_eq] == '=' && str_diffn(spec_long, longopts->name, spec_len) == 0)) {
              if(optdef != 0) {
                if(unix_opterr) {
                  buffer_putm_internal(unix_optbuf, "ambiguous option: ", spec_long, NULL);
                  buffer_putnlflush(unix_optbuf);
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
            if(unix_opterr) {
              buffer_putm_internal(unix_optbuf, "no such option: ", spec_long, NULL);
              buffer_putnlflush(unix_optbuf);
            }
            return '?';
          }

          switch(optdef->has_arg) {
            case no_argument:
              unix_optarg = 0;

              if(pos_eq != 0) {
                if(unix_opterr) {
                  buffer_putm_internal(unix_optbuf, "no argument for ", optdef->name, NULL);
                  buffer_putnlflush(unix_optbuf);
                }
                return '?';
              }
              break;
            case required_argument:
              if(spec_long[pos_eq] == '\0') {
                ++unix_optind;
                unix_optarg = argv[unix_optind];
              } else {
                unix_optarg = (char*)spec_long + pos_eq + 1;
              }
              break;
          }
          ++unix_optind;
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
      unix_optopt = c;

      if(unix_opterr) {
        buffer_putm_internal(unix_optbuf, argv[0], ": invalid option -- ", NULL);
        buffer_PUTC(unix_optbuf, (char)c);
        buffer_putnlflush(unix_optbuf);
      }
      ++nextchar;
      return '?';
    }

    if(*(optptr + 1) != ':') {
      nextchar++;

      if(*(argv[unix_optind] + nextchar) == '\0') {
        ++unix_optind;
        nextchar = 0;
      }
      unix_optarg = 0;
    } else {
      nextchar++;

      if(*(argv[unix_optind] + nextchar) != '\0') {
        unix_optarg = argv[unix_optind] + nextchar;
      } else {
        ++unix_optind;

        if(unix_optind < argc - postpone_count) {
          unix_optarg = argv[unix_optind];
        } else {
          unix_optopt = c;

          if(unix_opterr) {
            buffer_putm_internal(unix_optbuf, argv[0], ": option requires an argument -- ", NULL);
            buffer_PUTC(unix_optbuf, (char)c);
            buffer_putnlflush(unix_optbuf);
          }

          if(optstring[0] == ':' || ((optstring[0] == '-' || optstring[0] == '+') && optstring[1] == ':')) {
            c = ':';
          } else {
            c = '?';
          }
        }
      }
      ++unix_optind;
      nextchar = 0;
    }
    return c;
  }

  /* end of option analysis */

  /* fix the order of non-opt params to original */

  while((argc - unix_optind - postpone_count) > 0) {
    postpone(argc, argv, unix_optind);
    ++postpone_count;
  }

  nextchar = 0;
  postpone_count = 0;
  return -1;
}

int
unix_getopt(int argc, char* const argv[], const char* optstring) {
  return getopt_real(argc, argv, optstring, 0, 0);
}

int
unix_getopt_long(int argc, char* const argv[], const char* optstring, const struct unix_longopt* longopts, int* longindex) {
  return getopt_real(argc, argv, optstring, longopts, longindex);
}
