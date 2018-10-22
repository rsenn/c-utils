#include <limits.h>

#include "windoze.h"

#ifdef HAVE_LINUX_LIMITS_H
#include <linux/limits.h>
#endif

#include "str.h"
#include "path.h"
#include "stralloc.h"

#if WINDOWS
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