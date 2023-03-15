#ifndef GENMAKEFILE_TYPES_H
#define GENMAKEFILE_TYPES_H

#include "../../lib/stralloc.h"

typedef enum { OS_WIN, OS_MAC, OS_LINUX } os_type;
typedef enum { LANG_C, LANG_CXX } lang_type;
typedef enum { PREPROCESS = 0, COMPILE, LIB, LINK, MKDIR, CLEAN, NUM_COMMANDS } command_type;

typedef struct {
  enum { X86, ARM, PIC } arch;
  enum { _14, _16, _32, _64 } bits;
} machine_type;

typedef struct {
  os_type os;
  enum { NTOS, UNIX } type;
} system_type;

typedef struct {
  const char *src, *inc, *obj, *lib, *slib, *bin, *pps;
} exts_t;

typedef struct {
  machine_type mach;
  system_type sys;
  stralloc chip;
  int build_type;
  int lang;
} config_t;

#endif /* defined(GENMAKEFILE_TYPES_H) */
