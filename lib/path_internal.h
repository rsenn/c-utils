#include <limits.h>

#include "utf8.h"
#include "path.h"
#include "stralloc.h"
#include "windoze.h"

#if WINDOWS_NATIVE
#define PATHSEP_S_MIXED "\\/"
#define path_issep(c) ((c) == '/' || (c) == '\\')
#elif WINDOWS
#define PATHSEP_S_MIXED "\\/"
#define path_issep(c) ((c) == '/' || (c) == '\\')
#else
#define PATHSEP_S_MIXED "/"
#define path_issep(c) ((c) == '/')
#endif

#define path_isabs(p) (path_issep((p)[0]) || ((p)[1] == ':' && path_issep((p)[2])))
#define path_isrel(p) (!path_isabs(p))
#define path_isname(p) ((p)[u8s_chrs((p), PATHSEP_S_MIXED, sizeof(PATHSEP_S_MIXED) - 1)] != '\0')
