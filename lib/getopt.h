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

int getopt(int argc, char *const argv[], const char *optstring);

extern char *optarg;
extern int optind, opterr, optopt;

#define no_argument 0
#define required_argument 1
#define optional_argument 2

struct longopt {
  const char *name;
  int has_arg;
  int *flag;
  int val;
};

int getopt_long (int argc, char * const *argv, const char *optstring, const struct longopt *, int *);                                                                                                  

     int getopt_long_only(int argc, char  *  const  *argv,  const
     char   *shortopts,   const   struct  longopt  *longopts,  int
     *indexptr);
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

