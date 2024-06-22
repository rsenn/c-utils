#include "lib/stralloc.h"
#include "lib/buffer.h"
#include "lib/str.h"
#include "lib/strlist.h"
#include "lib/mmap.h"
#include "lib/buffer.h"
#include "lib/unix.h"
#include "lib/dir.h"
#include "lib/errmsg.h"
#include "lib/open.h"

static strlist pidlist;

static const char* stat_fields[] = {"pid",        "comm",       "state",  "ppid",     "pgrp",      "session",     "tty_nr",      "tpgid",     "flags",  "minflt",      "cminflt",   "majflt",      "cmajflt", "utime",
                                    "stime",      "cutime",     "cstime", "priority", "nice",      "num_threads", "itrealvalue", "starttime", "vsize",  "rss",         "rsslim",    "startcode",   "endcode", "startstack",
                                    "kstkesp",    "kstkeip",    "signal", "blocked",  "sigignore", "sigcatch",    "wchan",       "nswap",     "cnswap", "exit_signal", "processor", "rt_priority", "policy",  "delayacct_blkio_ticks",
                                    "guest_time", "cguest_time"};
static const char* statm_fields[] = {"size", "resident", "share", "text", "lib", "data", "dt"};

void
usage(char* argv0) {
  const char* prog = str_basename(argv0);
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       prog,
                       "\n",
                       "Options\n",
                       "  -h, --help                "
                       "show this help\n",
                       "\n",
                       NULL);
  buffer_putnlflush(buffer_1);
}

int
proc_openreadclose(const char* pid, const char* file, stralloc* out) {
  int ret;
  stralloc path;
  stralloc_init(&path);
  stralloc_copys(&path, "/proc/");
  stralloc_cats(&path, pid);
  stralloc_catc(&path, '/');
  stralloc_cats(&path, file);
  stralloc_nul(&path);

  ret = openreadclose(path.s, out, 1024);
  if(ret) {
    stralloc_trimr(out, "\n", 1);
    stralloc_nul(out);
  } else
    errmsg_warnsys("ERROR: reading ", path.s, ":", 0);

  return ret;
}

void
proc_list(strlist* pids) {
  dir_t d;
  dir_type_t type;
  const char* name;

  if(dir_open(&d, "/proc") != 0) {
    errmsg_warnsys("ERROR: Opening "
                   "directory /proc: ",
                   0);
    exit(1);
  }

  while((name = dir_read(&d))) {
    type = dir_type(&d);
    if(str_equal(name, "") || str_equal(name, ".") || str_equal(name, ".."))
      continue;
    if(type != D_DIRECTORY || !(name[0] >= '0' && name[0] <= '9'))
      continue;

    strlist_push(pids, name);
  }

  dir_close(&d);
}

int
main(int argc, char* argv[]) {
  int c, index = 0;
  stralloc data;
  strlist stat, statm, cmdline;
  const char* pid;
  static const struct unix_longopt opts[] = {{"help", 0, 0, 'h'}, {0, 0, 0, 0}};

  while((c = unix_getopt_long(argc, argv, "h", opts, &index)) != -1) {
    switch(c) {
      case 'h': usage(argv[0]); return 0;

      default: usage(argv[0]); return 1;
    }
  }

  if(unix_optind < argc)
    strlist_fromv(&pidlist, &argv[unix_optind], argc - unix_optind);
  else
    proc_list(&pidlist);

  stralloc_init(&data);
  strlist_init(&cmdline, '\0');
  strlist_init(&stat, '\0');
  strlist_init(&statm, '\0');

  strlist_foreach_s(&pidlist, pid) {
    stralloc_zero(&data);
    if(!proc_openreadclose(pid, "stat", &data))
      exit(1);

    strlist_zero(&stat);
    strlist_froms(&stat, data.s, ' ');

    buffer_puts(buffer_1, "Process #");
    buffer_puts(buffer_1, pid);
    buffer_puts(buffer_1, ": ");
    //  strlist_dump_named(buffer_1, &stat, stat_fields);
    buffer_putnlflush(buffer_1);

    stralloc_zero(&cmdline.sa);

    if(!proc_openreadclose(pid, "cmdline", &cmdline.sa))
      exit(1);

    strlist_dump(buffer_1, &cmdline);
  }

  return 0;
}
