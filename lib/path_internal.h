#include <limits.h>

#include "windoze.h"

#ifdef __linux__
#include <linux/limits.h>
#endif

#include "str.h"
#include "path.h"
#include "stralloc.h"

#if WINDOWS_NATIVE
#define PATHSEP_C '\\'
#define PATHSEP_S "\\"
#else
#define PATHSEP_C '/'
#define PATHSEP_S "/"
#endif

#ifndef PATH_MAX
#if WINDOWS_NATIVE
#include <windows.h>
#endif
#ifdef MAX_PATH
#define PATH_MAX MAX_PATH
#endif
#endif

#define path_issep(c) ((c) == '/' || (c) == '\\')

#define path_isabs(p) (path_issep((p)[0]) || ((p)[1] == ':' && path_issep((p)[2])))
#define path_isrel(p) (!path_isabs(p))
#define path_isname(p) ((p)[str_chr((p), '/')] != '\0')

