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

#define path_isdot(p) ((p)[0] == '.' && ((p)[1] == '\0' || path_issep((p)[1])))
#define path_isdotslash(p) ((p)[0] == '.' && path_issep((p)[1]))
#define path_isdotdot(p) ((p)[0] == '.' && (p)[1] == '.' && ((p)[2] == '\0' || path_issep((p)[2])))

/* drive prefix "C:" (WINDOWS only) */
#if WINDOWS
#define path_isdrive(p) (isalnum((p)[0]) && (p)[1] == ':')
#else
#define path_isdrive(p) 0
#endif

/* short-circuits, so "" and "a" are never read past their terminator */
#define path_isabs(p) (path_issep((p)[0]) || (path_isdrive(p) && path_issep((p)[2])))
#define path_isrel(p) (!path_isabs(p))

/* a bare name, no separator */
#define path_isname(p) ((p)[u8s_chrs((p), PATHSEP_S_MIXED, sizeof(PATHSEP_S_MIXED) - 1)] != '\0')
