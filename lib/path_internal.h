#include <limits.h>

#include "utf8.h"
#include "path.h"
#include "stralloc.h"
#include "windoze.h"

#define path_isdot(p) ((p)[0] == '.' && ((p)[1] == '\0' || path_issep((p)[1])))
#define path_isdotslash(p) ((p)[0] == '.' && path_issep((p)[1]))
#define path_isdotdot(p) ((p)[0] == '.' && (p)[1] == '.' && ((p)[2] == '\0' || path_issep((p)[2])))

#define path_isabs(p) (path_issep((p)[0]) || ((p)[1] == ':' && path_issep((p)[2])))
#define path_isrel(p) (!path_isabs(p))
#define path_isname(p) ((p)[u8s_chrs((p), PATHSEP_S_MIXED, sizeof(PATHSEP_S_MIXED) - 1)] != '\0')
