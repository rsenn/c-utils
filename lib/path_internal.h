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
#define PATHLISTSEP_C ';'
#define PATHLISTSEP_S ";"
#else
#define PATHSEP_C '/'
#define PATHSEP_S "/"
#define PATHLISTSEP_C ':'
#define PATHLISTSEP_S ":"
#endif

#ifndef PATH_MAX
#if WINDOWS_NATIVE
#include <windows.h>
#endif
#endif
#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif

#if WINDOWS_NATIVE
#define PATHSEP_S_MIXED "\\"
#define path_issep(c) ((c) == '\\')
#elif WINDOWS
#define PATHSEP_S_MIXED "\\/"
#define path_issep(c) ((c) == '/' || (c) == '\\')
#else
#define PATHSEP_S_MIXED "/"
#define path_issep(c) ((c) == '/')
#endif

#define path_isabs(p) (path_issep((p)[0]) || ((p)[1] == ':' && path_issep((p)[2])))
#define path_isrel(p) (!path_isabs(p))
#define path_isname(p) ((p)[str_chr((p), '/')] != '\0')

