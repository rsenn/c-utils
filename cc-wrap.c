#include "lib/windoze.h"
#include "lib/strlist.h"
#include "lib/strarray.h"
#include "lib/buffer.h"
#include "lib/path_internal.h"
#include "lib/process.h"
#include "lib/env.h"
#include "lib/errmsg.h"

#if WINDOWS_NATIVE
#include <process.h>
#else
#include <unistd.h>
#endif

static stralloc cmd, realcmd, fullcmd, specs;
static const char* ext = "";
static strlist path;

int
path_lookup(const char* cmd, stralloc* out) {
  char* s;

  strlist_foreach_s(&path, s) {
    stralloc_copys(out, s);
    stralloc_catc(out, PATHSEP_C);
    stralloc_cats(out, cmd);
    stralloc_nul(out);

    if(path_exists(out->s))
      return 1;
  }
  return 0;
}

int
main(int argc, char* argv[]) {
  size_t i;
  stralloc sa;
  strarray v;
  strlist args;
  char** av;
  int ret;
  const char* pathstr;
  strlist_init(&args, '\0');

  pathstr = env_get("PATH");

  strlist_init(&path, '\0');
  strlist_froms(&path, pathstr, PATHLISTSEP_C);

  errmsg_iam(argv[0]);

  stralloc_copys(&cmd, argv[0]);
  if(stralloc_endb(&cmd, ".exe", 4)) {
    cmd.len -= 4;
    ext = ".exe";
  }
  stralloc_copy(&realcmd, &cmd);
  stralloc_cats(&realcmd, ".real");
  stralloc_cats(&realcmd, ext);
  stralloc_nul(&realcmd);

  if(!stralloc_contains(&realcmd, PATHSEP_S)) {
    if(path_lookup(realcmd.s, &fullcmd))
      stralloc_copy(&realcmd, &fullcmd);
  }

  stralloc_copys(&specs, "-specs=");
  stralloc_cat(&specs, &realcmd);
  if((i = stralloc_finds(&specs, ".real"))) {
    specs.len = i;
  }
  stralloc_cats(&specs, ".specs");
  stralloc_nul(&specs);

  for(i = 1; i < argc; ++i) {
    strlist_push(&args, argv[i]);
  }

  strlist_unshift(&args, specs.s);
  strlist_unshift(&args, path_basename(realcmd.s));

  stralloc_init(&sa);
  strlist_joins(&args, &sa, "' '");
  stralloc_nul(&sa);
  // strarray_joins(&v, &sa, "'\n'");

  if(!path_exists(realcmd.s)) {
    errmsg_warnsys("exists ", realcmd.s, " ('", sa.s, "''): ", 0);
    return 127;
  }

#if 0
  buffer_puts(buffer_1, "cmd: '");
  buffer_putsa(buffer_1, &cmd);
  buffer_puts(buffer_1, ext);
  buffer_puts(buffer_1, "'");
  buffer_putnlflush(buffer_1);
  buffer_puts(buffer_1, "realcmd: '");
  buffer_putsa(buffer_1, &realcmd);
  buffer_puts(buffer_1, "'");
  buffer_putnlflush(buffer_1);
#endif
  #ifdef DEBUG
  buffer_puts(buffer_2, "execvp: '");
  buffer_putsa(buffer_2, &sa);
  buffer_puts(buffer_2, "'");
  buffer_putnlflush(buffer_2);
#endif

  av = strlist_to_argv(&args);
  ret = execvp(realcmd.s, av);

  if(ret == -1) {
    errmsg_warnsys("execvp:", 0);
    return 1;
  }

  return 0;
}
