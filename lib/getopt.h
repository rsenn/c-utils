/*
 * getopt - POSIX like getopt for Windows console Application
 *
 * win-c - Windows Console Library
 * Copyright (c) 2015 Koji Takami
 * Released under the MIT license
 * https://github.com/takamin/win-c/blob/master/LICENSE
 */

#if !defined(GETOPT_H) && !defined(__GETOPT_H__)
#define GETOPT_H 1
#define __GETOPT_H__ 1

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int unix_getopt(int argc, char* const argv[], const char* optstring);

extern char* unix_optarg;
extern int unix_optind, unix_opterr, unix_optopt;

#define no_argument 0
#define required_argument 1
#define optional_argument 2

struct unix_longopt {
  const char* name;
  int has_arg;
  int* flag;
  int val;
};

int unix_getopt_long(int argc, char* const* argv, const char* optstring, const struct unix_longopt*, int*);

int unix_getopt_long_only(
    int argc, char* const* argv, const char* shortopts, const struct unix_longopt* longopts, int* indexptr);
/****************************************************************************
    int getopt_long_only(int argc, char* const argv[],
            const char* optstring,
            const struct longopt* longopts, int* longindex);
****************************************************************************/
#ifdef __cplusplus
}
#endif /* __cplusplus */

#ifndef __GETOPT_H__
#define __GETOPT_H__ 1
#endif
#endif /* GETOPT_H */
