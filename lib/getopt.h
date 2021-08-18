#ifndef GETOPT_H
#define GETOPT_H 1

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

#endif /* GETOPT_H */
