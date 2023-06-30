#include "lib/windoze.h"
#include "lib/strlist.h"
#include "lib/array.h"
#include "lib/strarray.h"
#include "lib/stralloc.h"
#include "lib/buffer.h"
#include "lib/path.h"
#include "lib/process.h"
#include "lib/env.h"
#include "lib/errmsg.h"
#include "lib/byte.h"
#include "lib/unix.h"
#include "lib/getopt.h"

#if WINDOWS_NATIVE
#include <process.h>
#else
//#include <unistd.h>
#endif

static stralloc cmd, execbin, root, fullcmd, specs;
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

void
setup_env() {
  if(!env_get("MSYS2_PATH"))
    env_set("MSYS2_PATH", "/usr/local/bin:/usr/bin:/bin");
  if(!env_get("MSYS2_PREFIX"))
    env_set("MSYS2_PREFIX",
            "B:"
            "\\PortableApps\\MSYS2Porta"
            "ble\\App\\msys32\\usr");
  if(!env_get("MSYS2_ROOT"))
    env_set("MSYS2_ROOT",
            "B:"
            "\\PortableApps\\MSYS2Porta"
            "ble\\App\\msys32");
  if(!env_get("MSYSTEM"))
    env_set("MSYSTEM", "MSYS");
  if(!env_get("MSYSTEM_CARCH"))
    env_set("MSYSTEM_CARCH", "i686");
  if(!env_get("MSYSTEM_CHOST"))
    env_set("MSYSTEM_CHOST", "i686-pc-msys");
  if(!env_get("MSYSTEM_PREFIX"))
    env_set("MSYSTEM_PREFIX", "/usr");
}

/**
 * Show command line usage
 */
void
usage(char* errmsg_argv0) {
  buffer_putm_internal(buffer_1, "Usage: ", str_basename(errmsg_argv0), " [sources...]\n", "\n", "Options\n", "  -h, --help                " "show this help\n", "\n", NULL);
  buffer_putnlflush(buffer_1);
}

int
main(int argc, char* argv[], char* envp[]) {
  int i;
  stralloc sa;
  strarray v;
  strlist args;
  char** av;
  int ret;
  const char* pathstr;
  static int index, c, verbose;
  const struct unix_longopt opts[] = {
      {"help", 0, 0, 'h'},
      {"verbose", 0, &verbose, 'v'},
      {"exec", 0, 0, 'e'},
      {"root", 0, 0, 'r'},
      {0, 0, 0, 0},
  };
  strlist_init(&args, '\0');

  errmsg_iam(argv[0]);
  for(;;) {
    c = unix_getopt_long(argc, argv, "hve:", opts, &index);
    if(c == -1)
      break;
    if(c == 0)
      continue;

    switch(c) {
      case 'h': usage(argv[0]); break;

      case 'v': verbose++; break;
      case 'r': stralloc_copys(&root, unix_optarg); break;
      case 'e': stralloc_copys(&execbin, unix_optarg); break;

      default:
        buffer_putm_internal(buffer_2, "Unrecognized option `", argv[unix_optind], "'\n", "Try `", argv[0], "\" --help' for more " "information");
        buffer_putnlflush(buffer_2);
        return 1;
    }
  }

  pathstr = env_get("PATH");

  strlist_init(&path, '\0');
  strlist_froms(&path, pathstr, PATHLISTSEP_C);

  errmsg_iam(argv[0]);

  stralloc_copys(&cmd, argv[0]);
  /*  if(stralloc_endb(&cmd, ".exe", 4))
    { cmd.len -= 4; ext = ".exe";
    }*/
  stralloc_copy(&execbin, &cmd);
  stralloc_cats(&execbin, ".real");
  stralloc_cats(&execbin, ext);
  stralloc_nul(&execbin);

  if(!stralloc_contains(&execbin, PATHSEP_S)) {
    if(path_lookup(execbin.s, &fullcmd))
      stralloc_copy(&execbin, &fullcmd);
  }

  stralloc_copys(&specs, "-specs=");
  stralloc_cat(&specs, &execbin);
  if((i = stralloc_finds(&specs, ".real"))) {
    specs.len = i;
  }
  stralloc_cats(&specs, ".specs");
  stralloc_nul(&specs);

  for(i = 1; i < argc; ++i) {
    strlist_push(&args, argv[i]);
  }

  strlist_unshift(&args, specs.s);
  strlist_unshift(&args, path_basename(execbin.s));

  stralloc_init(&sa);
  strlist_joins(&args, &sa, "' '");
  stralloc_nul(&sa);

  /*
  // strarray_joins(&v, &sa, "'\n'");

  if(!path_exists(execbin.s)) {
    errmsg_warnsys("exists ", execbin.s,
" ('", sa.s, "''): ", 0); return 127;
  }

#if 0
  buffer_puts(buffer_1, "cmd: '");
  buffer_putsa(buffer_1, &cmd);
  buffer_puts(buffer_1, ext);
  buffer_puts(buffer_1, "'");
  buffer_putnlflush(buffer_1);
  buffer_puts(buffer_1, "execbin: '");
  buffer_putsa(buffer_1, &execbin);
  buffer_puts(buffer_1, "'");
  buffer_putnlflush(buffer_1);
#endif
#ifdef DEBUG
  buffer_puts(buffer_2, "execve: '");
  buffer_putsa(buffer_2, &sa);
  buffer_puts(buffer_2, "'");
  buffer_putnlflush(buffer_2);
#endif
*/
  av = strlist_to_argv(&args);
  ret = execve(execbin.s, av, envp);

  if(ret == -1) {
    errmsg_warnsys("execve:", 0);
    return 1;
  }

  return 0;
}
