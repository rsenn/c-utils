/*
 * Copyright (C) 2002 Uwe Ohse,
 * uwe@ohse.de This is free software,
 * licensed under the terms of the GNU
 * General Public License Version 2, of
 * which a copy is stored at:
 *    http://www.ohse.de/uwe/licenses/GPL-2
 * Later versions may or may not apply,
 * see http://www.ohse.de/uwe/licenses/
 * for information after a newer version
 * has been published.
 */
#include <time.h>
//#include "attributes.h"
//#include "ftpparse.h"
#include "lib/buffer.h"
#include "lib/stralloc.h"
#include "lib/scan.h"
#include "lib/ip4.h"
#include "lib/alloc.h"
#include "lib/byte.h"
//#include "getln.h"
//#include "ftplib.h"
//#include "strhash.h"
#include "lib/fmt.h"
//#include "fs64.h"
//#include "uogetopt.h"
//#include "uotime.h"
//#include "uo_wildmat.h"
//#include "urlparse.h"
//#include "timeoutio.h"
//#include "ftpcopy.h"
//#include "bailout.h"
#include "lib/str.h"
//#include "error.h"
#include "lib/open.h"
//#include "readwrite.h"
//#include "close.h"
//#include "nowarn.h"
//#include "get_cwd.h"
//#include "api_futimes.h"
//#include "api_utimes.h"
#include "lib/case.h"
//#include "mdtm.h"
//#include "api_dir.h"
//#include "mysleep.h"
#include "lib/socket.h"
//#include "ssort.h"
#include "lib/env.h"
#include <sys/stat.h>

#define TAI2UNIX(t) ((t)->x - 4611686018427387914ULL)
#define MAX_SLEEP 8 /* should really be enough */

buffer io_i;
buffer io_o;
static buffer io_d;
static stralloc io_d_mem;
int data_sock = -1;
static strhash hash_ids;
static strhash hash_fns;
static const char* o_user;
static const char* o_pass;
static const char* o_acct;
unsigned long o_loglevel = 1;
unsigned long o_login_sleep = 5;
static int may_mlsx = 1;
static int o_do_delete = 1;
static int o_dry_run = 0;
static int o_symlink_hack = 0;
static int o_directories_only = 0;
static int o_tolower = 0;
static int o_mdtm = 0;
static const char* o_list_options = 0;
static const char* o_inex_file;
static stralloc o_exclude = STRALLOC_INIT;
static unsigned long o_max_days = 0;
static unsigned long o_max_size = 0;
static unsigned long o_max_deletes = 0;
static unsigned long count_deletes = 0;
static unsigned long o_tries = 1;
static unsigned long o_rate_limit = 0;
static unsigned long o_max_depth = 4294967295UL;
static int o_interactive;
static int o_ignore_time;
static int o_ignore_size;
static int o_ascii_listings;
static int o_v4_only;
static int o_v6_only;
static int o_no_rest;
static int o_progress;
unsigned long o_timeout = 30;
stralloc remoteip;
stralloc pasv_response_ips;
int o_force_select;
static int o_bps;

static int loop(stralloc* r_dir, stralloc* l_dir);
static void usage(void) attribute_noreturn;

MKTIMEOUTREAD(o_timeout)

static int
do_symlink(const char* f, const char* t) {
  static stralloc s = STRALLOC_INIT;
  unsigned int l1 = 0;
  unsigned int slash = 0;
  const char* p;

  if(o_dry_run)
    return 0;

  while(f[l1] && t[l1] == f[l1]) {
    if(t[l1] == '/')
      slash = l1;
    l1++;
  }

  if(!f[l1] && t[l1] == '/')
    slash = l1;

  if(!slash) {
    if(!stralloc_cats(&s, f) || !stralloc_0(&s))
      oom();
    return symlink(s.s, t);
  }

  if(!f[l1]) {
    static stralloc x;

    if(!stralloc_copys(&x, f))
      oom();

    if(!stralloc_append(&x, "/"))
      oom();

    if(!stralloc_0(&x))
      oom();
    f = x.s;
  }
  slash++;
  p = t + slash + 1;
  s.len = 0;

  while(*p) {
    if(*p == '/')

      if(!stralloc_cats(&s, "../"))
        oom();
    p++;
  }

  if(!stralloc_cats(&s, f + slash))
    oom();

  if(s.len == 0)

    if(!stralloc_cats(&s, "."))
      oom();

  if(!stralloc_0(&s))
    oom();
  return symlink(s.s, t);
}

static void
hash_it(struct ftpparse* x, stralloc* l_dir, int id_also) {
  stralloc* t;
  t = canon(l_dir);

  if(1 != strhash_enter(&hash_fns, 1, t->s, t->len, 0, 0, 0))
    oom();

  if(!x->idlen || !id_also) {
    if(1 != strhash_enter(&hash_ids, 1, t->s, t->len, 1, t->s, t->len))
      oom();
  } else {
    if(1 != strhash_enter(&hash_ids, 1, x->id, x->idlen, 1, t->s, t->len))
      oom();
  }
}

static void
handle_rate_limit(struct taia* start, unsigned long bytes, struct taia* now) {
  struct taia diff;

  double sec;

  double bps;

  double slp;

  /* no use */

  if(bytes < 8192)
    return;

  if(bytes < o_rate_limit * 2)
    return;

  taia_sub(&diff, now, start);
  sec = taia_approx(&diff);

  if(sec <= 0.001)
    sec = 0.01;

  bps = bytes / sec;

  if(bps <= o_rate_limit)
    return;

  slp = sec * (bps / (double)o_rate_limit) - sec;

  if(slp <= 0)
    return;

  if(slp > MAX_SLEEP)
    /* try to not cause timeouts: this
     * should get the TCP recv windows
     * empty enough after a few seconds.
     */
    slp = MAX_SLEEP;

  mysleep(slp);
}

/* this eats more memory than really
 * needed. */
static void
remove_dir(stralloc* s) {
  stralloc sa = STRALLOC_INIT;
  const char* e;
  int flag;

  if(o_dry_run)
    return;

  if(0 == unlink(s->s) || errno == error_noent)
    return;

  if(-1 == api_dir_read(&sa, s->s))
    xbailout(100, errno, "failed to open/read ", s->s, 0, 0);

  s->len--;

  if(!stralloc_append(s, "/"))
    oom();

  for(e = api_dir_walkstart(&sa, &flag); e; e = api_dir_walknext(&sa, &flag)) {
    int pos;

    if(e[0] == '.') {
      if(e[1] == '.' && e[2] == '\0')
        continue;

      if(e[1] == '\0')
        continue;
    }
    pos = s->len;

    if(!stralloc_cats(s, e))
      oom();

    if(!stralloc_0(s))
      oom();

    if(-1 == unlink(s->s))
      remove_dir(s);
    s->len = pos;
  }
  api_dir_free(&sa);
  s->len--; /* remove trailing / */
  s->s[s->len++] = 0;

  if(-1 == rmdir(s->s))
    xbailout(100, errno, "failed to rmdir ", s->s, 0, 0);
}

static int
identical_time(time_t there, int theretype, time_t here) {
  time_t allowed = 0;
  time_t d;

  switch(theretype) {
    case FTPPARSE_MTIME_REMOTESECOND: /* time zone is unknown */
    case FTPPARSE_MTIME_LOCAL:        /* time
                                         is
                                         correct
                                       */
      allowed = 0;
      break;
    case FTPPARSE_MTIME_REMOTEMINUTE: /* time zone and secs are unknown */ allowed = 60; break;
    case FTPPARSE_MTIME_UNKNOWN:
    case FTPPARSE_MTIME_REMOTEDAY: /* time
                                      zone
                                      and
                                      time
                                      of
                                      day
                                      are
                                      unknown
                                    */
      allowed = 86400;
      break;
  }

  if(here > there)
    d = here - there;
  else
    d = there - here;

  if(d > allowed)
    return 0;
  return 1;
}

static int
download(struct ftpparse* x, stralloc* r_dir, stralloc* l_dir) {
  char* p;
  int fd;
  const char* e;
  const char* slash;
  const char* cc;
  static stralloc tmpfn = STRALLOC_INIT;
  buffer save;
  static stralloc savemem = STRALLOC_INIT;
  static int rest_flag = 0;
  struct stat st;
  char* fptr;
  unsigned int flen;
  int found;
  struct taia start;
  uint64 bytes = 0;
  time_t mtime;
  int pasv_retry = 0;
  uint64 expected_bytes;
  int progress_flag;
  int mtimetype;
  struct taia last_report;

  if(o_directories_only) {
    hash_it(x, l_dir, 1);
    return 0;
  }

  if(o_bps || o_rate_limit)
    taia_now(&start);

  e = l_dir->s + l_dir->len;

  for(cc = slash = l_dir->s; cc != e; cc++)

    if(*cc == '/')
      slash = cc;

  if(*slash == '/')
    slash++;

  if(!stralloc_copys(&tmpfn, ".tmp.") || !stralloc_catb(&tmpfn, slash, e - slash)) /* l_dir has \0 */
    oom();

  mtime = TAI2UNIX(&x->mtime);
  mtimetype = x->mtimetype;

  if(o_mdtm) {
    struct tai t;

    switch(modtime_request(r_dir->s, &t)) {
      case -1: o_mdtm = 0; break;
      case 0: break;
      case 1:
        mtime = TAI2UNIX(&t);
        mtimetype = FTPPARSE_MTIME_LOCAL; /* well,
                                             so
                                             it
                                             is
                                             supposed
                                             to
                                             be
                                           */
        break;
    }
  }

  found = strhash_lookup(&hash_ids, x->id, x->idlen, &fptr, &flen);

  if(0 == stat(l_dir->s, &st)) {

    int identical = 1;

    if(!o_ignore_size && st.st_size != (off_t)x->size)
      identical = 0;

    if(identical && !o_ignore_time)
      identical = identical_time(mtime, x->mtimetype, st.st_mtime);

    if(identical) {
      if(found) {
        if(0 == link(fptr, tmpfn.s) || 0 == do_symlink(fptr, tmpfn.s)) {
          /* delete this in case both
           * files are links to the
           * same inode */
          unlink(l_dir->s);

          if(-1 == rename(tmpfn.s, l_dir->s)) {
            int er = errno;
            unlink(tmpfn.s);
            xbailout(100, er, "failed to rename ", tmpfn.s, " to ", l_dir->s);
          }
          hash_it(x, l_dir, 0);

          if(o_loglevel > 1)

            do_log2(l_dir->s, ": is a link\n");
          return 1;
        }
        xbailout(100,
                 errno,
                 "failed to create                  ink from ",
                 fptr,
                 " to ",
                 l_dir->s);
      }
      hash_it(x, l_dir, !found);

      if(o_loglevel > 1)

        do_log2(l_dir->s, ": identical file found\n");
      return 1;
    }

    if(o_loglevel > 2) {
      char nb[FMT_ULONG];

      do_log2(l_dir->s, ": lfacts: ");
      nb[fmt_ulong(nb, st.st_mtime)] = 0;

      do_log2(nb, " ");
      nb[fmt_ulong(nb, st.st_size)] = 0;

      do_log2(nb, "\n");
    }
  }

  if(found) {
    if(o_dry_run) {
      if(o_loglevel > 1)

        do_log4(l_dir->s, "would be (sym)linked to ", fptr, "\n");
      return 1;
    }

    if(0 == link(fptr, tmpfn.s) || 0 == do_symlink(fptr, tmpfn.s)) {
      if(-1 == rename(tmpfn.s, l_dir->s)) {
        int er = errno;
        unlink(tmpfn.s);
        xbailout(100, er, "failed to rename ", tmpfn.s, " to ", l_dir->s);
      }

      if(o_loglevel > 1)

        do_log4(l_dir->s, " (sym)linked to ", fptr, "\n");
      hash_it(x, l_dir, 1);
      return 1;
    }
  }

  if(o_max_size && x->size > o_max_size) {
    if(o_loglevel > 1)

      do_log2(l_dir->s, ": too large\n");
    return 1;
  }

  if(o_max_days && mtime + o_max_days * 86400 < uo_now()) {
    hash_it(x, l_dir, 1);

    if(o_loglevel > 1)

      do_log2(l_dir->s, ": too old\n");
    return 1;
  }

  if(o_dry_run) {
    if(o_loglevel)

      do_log2(l_dir->s,
              ": dry-run non-download               uccessful\n");
    hash_it(x, l_dir, 1);
    return 1;
  }

  if(o_ascii_listings) {
    if(data_sock != -1) {
      close(data_sock);
      data_sock = -1;
    }
    sx2("TYPE I");
  }

  /* complete tmp name */
  {
    char nb[FMT_ULONG];
    tmpfn.len--; /* \0 */

    if(!stralloc_catb(&tmpfn, ".", 1) || !stralloc_catb(&tmpfn, nb, fmt_uint64(nb, mtime)) || !stralloc_catb(&tmpfn, ".", 1) ||
       !stralloc_catb(&tmpfn, nb, fmt_uint64(nb, x->size)) || !stralloc_0(&tmpfn))

      if(!o_no_rest)
        unlink(tmpfn.s);
  }
  expected_bytes = x->size;
retry_open:
  fd = open_excl_mode(tmpfn.s, 0644);

  if(-1 == fd) {
    if(error_exist != errno)
      xbailout(100, errno, "failed to open_excl ", tmpfn.s, ", temporary file for ", l_dir->s);
    fd = open_append(tmpfn.s);

    if(-1 == fd)
      xbailout(100, errno, "failed to open_append ", tmpfn.s, ", temporary file for ", l_dir->s);
    rest_flag = 1;
  }

  if(!stralloc_ready(&savemem, BUFFER_OUTSIZE))
    oom();
  buffer_init(&save, (buffer_op)write, fd, savemem.s, BUFFER_OUTSIZE);

retry_pasv:

  if(data_sock == -1)
    data_sock = do_pasv();

  if(o_rate_limit) {
    /* set socket receive buffer to low
     * value */

    if(o_rate_limit * 8 < 65536)
      sockrecbuf(data_sock, MAX_SLEEP * o_rate_limit);
    else {
      /* could use getsockopt, but would
       * run into socklen_t
       * (un)availability */
      sockrecbuf(data_sock, 65536);     /* this is ok */
      sockrecbuf(data_sock, 65536 * 4); /* and this fails on
                                           some systems. */
    }
  }

  if(rest_flag) {
    char nb[FMT_ULONG];
    uint64 pos = 0;

    if(1 == rest_flag) {
      struct stat fs;

      if(-1 == fstat(fd, &fs))
        xbailout(100, errno, "failed to fstat ", tmpfn.s, ", temporary file for ", l_dir->s);
      pos = fs.st_size;
      expected_bytes -= pos;
      rest_flag = 2;
    } else
      /* to make sure that REST 0 is
       * given after any try to do REST.
       * FTP is somewhat strange and
       * implementations are stranger.
       */
      rest_flag = 0;
    nb[fmt_uint64(nb, pos)] = 0;
    cmdwrite2("REST ", nb);
    p = ccread();

    if(*p != '3' || p[1] != '5') {
      cmdwrite2("REST ", "0");
      ccread();
      rest_flag = 0;

      if(-1 == unlink(tmpfn.s))
        xbailout(100, errno, "failed to unlink ", tmpfn.s, ", temporary file for ", l_dir->s);
      close(fd);
      goto retry_open;
    }

    if(rest_flag)

      do_log4(l_dir->s, ": restarting at ", nb, "\n");
  }
  cmdwrite2("RETR ", r_dir->s);
  p = ccread();

  if(!p)
    eof_or_error(111, errno, "failed to read RETR answer", 0, 0);

  if(*p != '1') {
    if(!pasv_retry++ && str_start(p, "425")) {
      /* at least one ftp server seems
       * to close the data connection in
       * case a RETR is done on a
       * directory. So work around ...
       */
      close(data_sock);
      data_sock = -1;
      goto retry_pasv;
    }
    warning(0, "got unwanted answer to `RETR ", r_dir->s, "': ", p);
    unlink(tmpfn.s);
    close(fd);
    return 0;
  }
  buffer_init(&io_d, (buffer_op)TIMEOUTREADFN(o_timeout), data_sock, io_d_mem.s, BUFFER_INSIZE);
  progress_flag = 0;
  taia_now(&last_report);

  for(;;) {
    int l;
    char* q;
    struct taia now;
    l = buffer_feed(&io_d);

    if(l == -1)
      xbailout(111, errno, "failed to read from remote", 0, 0, 0);
    bytes += l;

    if(o_progress || o_rate_limit)
      taia_now(&now);

    if(o_progress) {
      char nb[FMT_ULONG];
      static unsigned int back;
      unsigned int t;
      struct taia d;
      taia_sub(&d, &now, &last_report);

      if(taia_approx(&d) >= 1 || (back && !l)) {
        static uint32 old_window_x;
        /* note: should be larger than
         * 20+20+4+2+2+3 == 51 */
        static char bs[] = "\b\b\b\b\b\b\b\b\b\b\b\b\b"
                           "\b\b\b\b\b\b\b\b\b\b\b\b\b"
                           "\b"
                           "\b\b\b\b\b\b\b\b\b\b\b\b\b"
                           "\b\b\b\b\b\b\b\b\b\b\b\b\b"
                           "\b";

        if(!old_window_x)
          old_window_x = window_x;

        if(old_window_x != window_x) {
          while(old_window_x) {
            int written;
            written = write(2, bs, old_window_x > sizeof(bs) ? sizeof(bs) : old_window_x);

            if(written <= 0) {
              write(2, "\n", 1);
              break;
            }
            old_window_x -= written;
          }
          old_window_x = window_x;
          progress_flag = 0;
        }

        if(!progress_flag) {
          int off = 0;
          uint32 need;
          uint32 space = window_x;
          need = fmt_uint64(nb, expected_bytes ? expected_bytes : 0xffffffffUL);
          space -= need * 2 + 4 + 2;

          if(expected_bytes) {
            space -= 8;
          }

          if(l_dir->len - 1 > space) {
            off = l_dir->len - 1 - space + 3;
            write(2, "...", 3);
          }
          write(2, l_dir->s + off, l_dir->len - 1 - off);
          write(2, ": ", 2);
          progress_flag = 1;
          back = 0;
        }

        if(back)
          write(2, bs, back);
        t = fmt_uint64(nb, bytes);
        write(2, nb, t);
        write(2, " of ", 4);
        back = t + 4;
        t = fmt_uint64(nb, expected_bytes);
        write(2, nb, t);
        back += t;

        if(expected_bytes) {
          write(2, " (", 2);
          back += 2;
          t = fmt_uint64(nb, 100 * bytes / expected_bytes);
          write(2, nb, t);
          back += t;
          /* the trailing space is for
           * terminals not implementing
           * \b, and as a safeguard in
           * case the window size is
           * reduced */
          write(2, "%) ", 3);
          back += 3;
        }
        last_report = now;
      }
    }

    if(l == 0)
      break;

    q = buffer_peek(&io_d);

    if(-1 == buffer_put(&save, q, l))
      xbailout(111, errno, "failed to write to ", tmpfn.s, 0, 0);
    buffer_seek(&io_d, l);

    if(o_rate_limit)
      handle_rate_limit(&start, bytes, &now);
  }

  if(o_progress && progress_flag)
    write(2, "\n", 1);
  close(data_sock);
  x2("RETR finish");

  if(buffer_flush(&save))
    xbailout(111, errno, "failed to write to ", tmpfn.s, 0, 0);
  data_sock = -1;

  if(-1 == fsync(fd))
    xbailout(111, errno, "failed to fsync ", tmpfn.s, 0, 0);

  if(-1 == api_futimes_1(fd, mtime, 0, mtime, 0))
    warning(errno, "failed to futimes ", tmpfn.s, 0, 0);

  if(-1 == close(fd))
    xbailout(111, errno, "failed to close ", tmpfn.s, 0, 0);

  /* some version of reiserfs on linux
   * didn't honor utimes() if it was
   * called before the close(). */

  if(-1 == api_futimes_2(tmpfn.s, mtime, 0, mtime, 0))
    warning(errno, "failed to utimes ", tmpfn.s, 0, 0);

  if(-1 == rename(tmpfn.s, l_dir->s)) {
    remove_dir(l_dir);

    if(-1 == rename(tmpfn.s, l_dir->s)) {
      int er = errno;
      unlink(tmpfn.s);
      xbailout(100, er, "failed to rename ", tmpfn.s, " to ", l_dir->s);
    }
  }

  if(expected_bytes != bytes) {
    char nb[FMT_ULONG];
    /* This _most_ often means that the
     * connection was closed suddenly.
     * Note that it also can mean that
     * the file has been changed between
     * the getting of the listing and
     * the download, and that everything
     * is fine now. No, that's not
     * unlikely. Try mirroring a big
     * directory over a slow connection.
     * It's hard to do the "right" thing
     * here.
     * XXX get a listing of that file
     * and check the size?
     * XXX size is uint64, we print
     * ulong
     */
    nb[fmt_uint64(nb, expected_bytes)] = 0;

    do_log2(l_dir->s, ": warning: expected ");

    do_log2(nb, " bytes, but got ");
    nb[fmt_ulong(nb, bytes)] = 0;

    do_log2(nb, "\n");
  }
  hash_it(x, l_dir, 1);

  if(o_loglevel)

    do_log2(l_dir->s, ": download successful");

  if(o_bps) {
    struct taia stop;
    struct taia diff;
    unsigned long sec;
    char nb[FMT_ULONG];
    unsigned long bps;
    const char* what;
    /* could use double arithmetics, but
     * what for? */
    taia_now(&stop);
    taia_sub(&diff, &stop, &start);
    sec = taia_approx(&diff);

    if(sec == 0)
      sec = 1;

    do_log1(", ");
    bps = bytes / sec;

    if(bps < 10000) {
      nb[fmt_ulong(nb, bps)] = 0;
      what = " B/s";
    } else if(bps / 1024 < 10000) {
      nb[fmt_ulong(nb, bps / 1024)] = 0;
      what = " KB/s";
    } else {
      nb[fmt_ulong(nb, bps / (1024 * 1024))] = 0;
      what = " MB/s";
    }

    do_log2(nb, what);
  }

  if(o_loglevel)

    do_log1("\n");
  return 1;
}

static int
handle_exclude(stralloc* ca) {
  char* end = o_exclude.s + o_exclude.len;
  char* ptr = o_exclude.s;
  const char* info = 0; /* keep gcc quiet */
  int exclude = 0;

  while(ptr != end) {
    int flag = (*ptr == '-');
    ptr++;

    if(uo_wildmat(ptr, ca->s, ca->len - 1)) {
      if(exclude != flag) {
        exclude = flag;
        info = ptr;

        if(o_loglevel > 3) {
          write(1, ca->s, ca->len - 1);

          do_log4(": matched `", info, "', -> ", exclude ? "exclude" : "include");
          write(1, "\n", 1);
        }
      }
    }
    ptr += str_len(ptr) + 1;
  }

  if(!info)
    info = "end-of-list (default)";

  if(exclude) {
    if(o_loglevel > 1) {
      write(1, ca->s, ca->len - 1);

      do_log3(": excluded, matched `", info, "'\n");
    }
    return 1;
  }

  if(o_loglevel > 3) {
    write(1, ca->s, ca->len - 1);

    do_log3(": included, matched `", info, "'\n");
  }
  return 0;
}

static int
handle_hackish_symlink(struct ftpparse* x, stralloc* l_dir) {
  unsigned int pos;
  stralloc t = STRALLOC_INIT;
  stralloc idstr = STRALLOC_INIT;

  if(!x->symlink)
    xbailout(100, 0, "bad coding", 0, 0, 0);

  if(!*x->symlink)
    xbailout(100, 0, "bad coding", 0, 0, 0);

  if(!stralloc_copy(&t, l_dir))
    oom();
  t.s[t.len - 1] = '/';
  pos = t.len;

  if(!stralloc_catb(&t, x->symlink, x->symlinklen) || !stralloc_0(&t))
    oom();

  if(o_tolower)
    case_lowers(t.s + pos);

  if(o_loglevel > 1) {
    do_log1("symlink '");

    do_logmem(x->name, x->namelen);

    do_logmem(" -> ", 4);

    do_logmem(t.s, t.len - 1);

    do_log1("'\n");
  }

  if(!stralloc_copy(&idstr, canon(&t)))
    oom();

  if(!stralloc_copy(&t, l_dir))
    oom();
  t.s[t.len - 1] = '/';

  if(!stralloc_catb(&t, x->name, x->namelen))
    oom();

  if(!stralloc_0(&t))
    oom();
  unlink(t.s);
  remove_dir(&t);

  if(0 != do_symlink(idstr.s, t.s)) {
    if(errno == error_isdir)
      remove_dir(&t);

    if(0 != do_symlink(idstr.s, t.s))
      xbailout(100,
               errno,
               "failed to create symlink                rom ",
               idstr.s,
               " to ",
               t.s);
  }
  hash_it(x, &t, 1);
  stralloc_free(&t);
  stralloc_free(&idstr);
  return 1;
}

static int
handle_directory(struct ftpparse* x, stralloc* r_dir, stralloc* l_dir) {
  int found;
  char* fptr;
  unsigned int flen;
  int done = 0;
  unsigned int lpos = l_dir->len;

  l_dir->s[l_dir->len - 1] = '/';

  if(!stralloc_catb(l_dir, x->name, x->namelen) || !stralloc_0(l_dir))
    oom();

  if(o_tolower)
    case_lowers(l_dir->s + lpos);
  found = strhash_lookup(&hash_ids, x->id, x->idlen, &fptr, &flen);

  if(found) {
    if(o_loglevel > 1)

      do_log4(l_dir->s,
              ": (sym)linking, ID               dentical to `",
              fptr,
              "'\n");
    remove_dir(l_dir);

    if(0 != do_symlink(fptr, l_dir->s))
      xbailout(111,
               errno,
               "failed to create                ymlink from ",
               fptr,
               " to ",
               l_dir->s);
    hash_it(x, l_dir, 1);

    done = 1;
  } else {
    /* remote is a new directory. */
    /* local may be a symlink to a
     * directory we already saw */
    struct stat st;

    if(0 == lstat(l_dir->s, &st))

      if(!S_ISDIR(st.st_mode))
        unlink(l_dir->s);

    /* hash it now: "dir" -> "."
     * symlinks */
    hash_it(x, l_dir, 1);

    if(1 == loop(r_dir, l_dir)) {
      if(!o_dry_run)

        if(-1 == api_utimes(l_dir->s, TAI2UNIX(&x->mtime), 0, TAI2UNIX(&x->mtime), 0))
          warning(errno,
                  "failed to call                   times on ",
                  l_dir->s,
                  0,
                  0);

      done = 1;
    }
  }
  l_dir->len = lpos;
  l_dir->s[l_dir->len - 1] = '\0';
  return done;
}

/* note: this is not so much an
 * optimization, but rather a workaround
 * for a misbehaving server, which
 * seemed to refuse to do a third
 * listing on a directory (ftpcopy
 * happens to do two when it tries to
 * find out what the command line
 * argument really is).
 */
static unsigned int initial_count;
static stralloc initial_dirdata;
static stralloc initial_dirname;

static int
fcmp(const void* va, const void* vb) {
  const struct ftpparse* a = va;
  const struct ftpparse* b = vb;
  unsigned int ml = a->namelen;
  int x;

  if(b->namelen < ml)
    ml = b->namelen;
  x = byte_diff(a->name, ml, b->name);

  if(x)
    return x;

  if(a->namelen < b->namelen)
    return 1;

  if(a->namelen > b->namelen)
    return -1;
  return 0; /* shouldn't happen, though
             */
}

static struct ftpparse*
parsethem(stralloc* d, int is_mlsx, uint32* rcount) {
  char *s, *e;
  struct ftpparse* x;
  uint32 count = 0;
  s = d->s;
  e = s + d->len;

  while(s < e) {
    count++;
    s += str_len(s) + 1;
  }
  x = (void*)alloc(count * sizeof(*x));

  if(!x)
    oom();
  count = 0;
  s = d->s;

  while(s < e) {
    int ok;
    unsigned int l = str_len(s);

    if(is_mlsx)
      ok = ftpparse_mlsx(x + count, s, l, 0);
    else
      ok = ftpparse(x + count, s, l, o_eat_leading_spaces);

    if(ok && x[count].flagbrokenmlsx)
      warn_broken_mlsx();

    if(!ok)

      do_log3("cannot parse LIST line: ", s, "\r\n");
    else
      count++;
    s += l + 1;
  }
  mssort((void*)x, count, sizeof(struct ftpparse), fcmp);
  *rcount = count;
  return x;
}

static int
loop(stralloc* r_dir, stralloc* l_dir) {
  stralloc dirdata = STRALLOC_INIT;
  int olddirfd;
  int listno;
  int pasv_retries = 0;
  int flag_is_initial = 0;
  int is_mlsx = 0;
  struct ftpparse* parsed;
  uint32 count;
  uint32 i;
  char* ans;

  cmdwrite2("CWD ", r_dir->s);

  ans = ccread();

  if(!ans)
    eof_or_error(111, errno, "failed to read CWD answer", 0, 0);

  if(*ans != '2')
    return 0;

  if(initial_dirname.s)

    if(initial_dirname.len == r_dir->len)

      if(byte_equal(initial_dirname.s, r_dir->len, r_dir->s))
        flag_is_initial = 1;

  if(flag_is_initial)
    goto skip_listing;

  if(may_mlsx)
    listno = 0;
  else
    listno = 1;

  if(o_ascii_listings) {
    if(data_sock != -1) {
      close(data_sock);
      data_sock = -1;
    }
    sx2("TYPE A");
  }
retry_listing_pasv:

  if(data_sock == -1)
    data_sock = do_pasv();
retry_listing:

  if(listno == 0) {
    cmdwrite1("MLSD");
    is_mlsx = 1;
  } else if(o_list_options)
    cmdwrite2("LIST ", o_list_options);
  else
    cmdwrite1("LIST");
  ans = ccread();

  if(!ans)
    eof_or_error(111, errno, "failed to read LIST answer", 0, 0);

  if(!pasv_retries++ && str_start(ans, "425")) {
    /* in case ftp server lost track of
     * the open PASV connection */
    close(data_sock);
    data_sock = -1;
    goto retry_listing_pasv;
  }

  if(listno == 0 && *ans == '5') {
    listno = 1;

    if(!str_start(ans, "501")) { /* 501: MSLD
                                    on file */
      may_mlsx = 0;
      is_mlsx = 0;
    }
    goto retry_listing;
  }

  if(*ans != '1')
    return 0;
skip_listing:
  olddirfd = open_read(".");

  if(!o_dry_run)

    if(-1 == chdir(l_dir->s)) {
      if(errno != error_exist)
        remove_dir(l_dir);

      if(-1 == mkdir(l_dir->s, 0755))
        xbailout(100, errno, "failed to mkdir ", l_dir->s, 0, 0);

      if(-1 == chdir(l_dir->s))
        xbailout(100, errno, "failed to chdir ", l_dir->s, 0, 0);
    }

  if(flag_is_initial) {
    if(!stralloc_copy(&dirdata, &initial_dirdata))
      oom();
  } else {
    if(-1 == ftp_read_list(data_sock, &dirdata))
      xbailout(111,
               errno,
               "failed to read remote                irectory",
               0,
               0,
               0);

    close(data_sock);
    data_sock = -1;
    x2("LIST");
  }
  parsed = parsethem(&dirdata, is_mlsx, &count);

  for(i = 0; i < count; i++) {
    if(parsed[i].name[0] == '.' && (parsed[i].namelen == 1 || (parsed[i].namelen == 2 && parsed[i].name[1] == '.'))) {
      if(o_loglevel > 1) {
        do_log2(l_dir->s, "/");

        if(o_tolower)
          case_lowerb(parsed[i].name, parsed[i].namelen);

        do_logmem(parsed[i].name, parsed[i].namelen);

        do_log1(": ignored\n");
      }
    } else if(parsed[i].name[byte_chr(parsed[i].name, parsed[i].namelen, '/')] == '/') {
      /* file name with a slash in it:
       * no good. Can't happen and has
       * security implications. */

      do_log2(l_dir->s, "/");

      if(o_tolower)
        case_lowerb(parsed[i].name, parsed[i].namelen);

      do_logmem(parsed[i].name, parsed[i].namelen);

      do_log1(": ignored\n");
    } else {
      size_t rpos = r_dir->len;
      size_t lpos = l_dir->len;
      int done = 0;

      if(o_loglevel > 2) {
        char nb1[FMT_ULONG];
        char nb2[FMT_ULONG];
        static stralloc lo;
        nb1[fmt_ulong(nb1, TAI2UNIX(&parsed[i].mtime))] = 0;
        nb2[fmt_ulong(nb2, parsed[i].size)] = 0;

        if(!stralloc_copyb(&lo, parsed[i].name, parsed[i].namelen))
          oom();

        if(o_tolower)
          case_lowerb(lo.s, lo.len);

        do_log2(l_dir->s, "/");

        do_logmem(lo.s, lo.len);

        do_log4(": facts: ", nb1, " ", nb2);

        do_log1("\n");
      }

      r_dir->s[r_dir->len - 1] = '/';

      if(!stralloc_catb(r_dir, parsed[i].name, parsed[i].namelen) || !stralloc_0(r_dir))
        oom();

      if(o_exclude.len) {
        stralloc* ca;
        ca = canon(r_dir);

        if(o_tolower)
          case_lowerb(ca->s, ca->len);

        done = handle_exclude(ca);
      }

      if(!done && o_symlink_hack && parsed[i].symlink)

        if(parsed[i].format == FTPPARSE_FORMAT_LS)

          done = handle_hackish_symlink(&parsed[i], l_dir);

      if(parsed[i].flagtrycwd && !done) {
        if(o_max_depth) {
          o_max_depth--;

          done = handle_directory(&parsed[i], r_dir, l_dir);
          o_max_depth++;
        } else {
          done = 1;

          if(o_loglevel) {
            stralloc sa = STRALLOC_INIT;

            if(!stralloc_cats(&sa, l_dir->s))
              oom();

            if(!stralloc_cats(&sa, "/"))
              oom();

            if(!stralloc_catb(&sa, parsed[i].name, parsed[i].namelen))
              oom();

            if(!stralloc_0(&sa))
              oom();

            do_log2(sa.s, ": not entered\n");
            stralloc_free(&sa);
          }
        }
      }

      if(parsed[i].flagtryretr && !done) {
        l_dir->s[l_dir->len - 1] = '/';

        if(!stralloc_catb(l_dir, parsed[i].name, parsed[i].namelen) || !stralloc_0(l_dir))
          oom();

        if(o_tolower)
          case_lowers(l_dir->s + lpos);

        if(1 == download(&parsed[i], r_dir, l_dir))

          done = 1;
        l_dir->len = lpos;
        l_dir->s[l_dir->len - 1] = '\0';
      }
      r_dir->len = rpos;
      r_dir->s[r_dir->len - 1] = '\0';
    }
  }
  alloc_free((char*)parsed);
  stralloc_free(&dirdata);

  if(-1 == fchdir(olddirfd)) /* ECANTHAPPEN,
                                but we'd be in
                                trouble */
    xbailout(111,
             errno,
             "failed to fchdir to              pper level directory",
             0,
             0,
             0);
  close(olddirfd); /* ECANTHAPPEN */
  return 1;        /* done */
}

static void
cwd_slash(void) {
  char* p;
  cmdwrite1("CWD /");
  p = ccread();

  if(!p)
    eof_or_error(111, errno, "failed to read 'CWD /' answer", 0, 0);

  if(*p != '2')
    xbailout(100, 0, "failed to 'CWD /': ", p, 0, 0);
}

/* try to find out information about the
 * remote start directory. Due to LIST
 * ambiguities this will only work
 * reliably with publicfile. Be
 * paranoid.
 */
static void
initialdirectory(stralloc* dirdata, struct ftpparse* fp, stralloc* r_dir) {
  char* p;
  int count;

  if(data_sock == -1)
    data_sock = do_pasv();

  cmdwrite2("LIST ", r_dir->s);
  p = ccread();

  if(!p)
    eof_or_error(111, errno, "failed to read LIST answer", 0, 0);

  if(*p != '1')
    xbailout(111,
             errno,
             "failed to read initial              irectory LIST answer",
             0,
             0,
             0);
  count = ftp_read_list(data_sock, dirdata);
  close(data_sock);
  data_sock = -1;
  x2("LIST");

  if(1 != count)
    /* XXX could try LIST .. and parse
     * the answer */
    return; /* doesn't help us. */

  if(1 != ftpparse(fp, dirdata->s, str_len(dirdata->s), o_eat_leading_spaces))
    return;

  if(fp->namelen != r_dir->len - 1 || byte_diff(fp->name, fp->namelen, r_dir->s))
    fp->idlen = 0; /* used in onedirpair() */
}
/* try to find out whether the remote
 * target is a file or directory. we
 * can't reliably use LIST: "LIST xxx"
 * will return a list with one element
 * "xxx" if xxx is a file or "xxx" is a
 * directory with exactly one file,
 * which is called "xxx".
 *
 * We _could_ use LIST with an EPLF
 * supporting FTP server, but we cannot
 * reliable detect that.
 */
static int
initialentity(struct ftpparse* fp, stralloc* r_dir) {
  int count = 0;
  char* p;
  static stralloc dirdata = STRALLOC_INIT; /* we return a
                                              pointer into
                                              that */

  cwd_slash();
  cmdwrite2("MLST ", r_dir->s);
  p = ccread_oneline();

  if(!p)
    eof_or_error(111, errno, "failed to read MLST answer", 0, 0);

  if(*p == '2') {
    for(;;) {
      p = ccread_oneline();

      if(str_start(p, "250 "))
        break;

      if(str_start(p, "250-"))
        continue;

      if(!stralloc_copys(&dirdata, p))
        oom();
    }

    if(ftpparse_mlsx(fp, dirdata.s, dirdata.len, 1))

      if(fp->flagtrycwd && !fp->flagtryretr)
        return 1;
    return 0;
  }

  while(str_len(p) > 3 && p[3] == '-') {
    /* we were in single-line mode */
    p = ccread();

    if(!p)
      eof_or_error(111, errno, "failed to read MLST answer", 0, 0);
  }

  cmdwrite2("CWD ", r_dir->s);
  p = ccread();

  if(!p)
    eof_or_error(111, errno, "failed to read CWD answer", 0, 0);

  if(*p == '2') {
    /* may be a directory.
     * Note: publicfile allows "CWD" to
     * a file, but LIST fails.
     */

    if(data_sock == -1)
      data_sock = do_pasv();

    cmdwrite1("LIST");
    p = ccread();

    if(!p)
      eof_or_error(111, errno, "failed to read LIST answer", 0, 0);

    if(*p == '1') {
      count = ftp_read_list(data_sock, &dirdata);
      close(data_sock);
      data_sock = -1;
      x2("LIST");
      cwd_slash();

      if(-1 != count) {
        /* it's a directory */
        initial_count = count;

        if(!stralloc_copy(&initial_dirdata, &dirdata))
          oom();

        if(!stralloc_copy(&initial_dirname, r_dir))
          oom();
        initialdirectory(&dirdata, fp, r_dir);
        return 1;
      }
    } else {
      close(data_sock);
      data_sock = -1;
      cwd_slash();
    }
  }

  if(data_sock == -1)
    data_sock = do_pasv();

  cmdwrite2("LIST ", r_dir->s);
  p = ccread();

  if(!p)
    eof_or_error(111, errno, "failed to read LIST answer", 0, 0);

  if(*p != '1')
    xbailout(100,
             0,
             "failed to LIST the              emote directory: ",
             p,
             0,
             0);

  count = ftp_read_list(data_sock, &dirdata);

  if(-1 == count)
    xbailout(111, errno, "cannot read remote directory", 0, 0, 0);
  close(data_sock);
  data_sock = -1;
  x2("LIST");

  if(count == 0) /* typically happens if first
                    entity is nonexistant */
    xbailout(111,
             0,
             "remote file/directory              oesn't exist",
             0,
             0,
             0);

  if(count != 1)
    xbailout(111,
             0,
             "too many entries return              y 'LIST ",
             r_dir->s,
             "'",
             0);
  /* might be a directory with one file,
   * or a file */
  /* try to get the date. We need it
   * later _if_ it is a file */
  ftpparse(fp, dirdata.s, dirdata.len, o_eat_leading_spaces);
  fp->flagtrycwd = 0;
  fp->flagtryretr = 1;
  return 0;
}

static void delete(stralloc* dn) {
  stralloc sa = STRALLOC_INIT;
  const char* e;
  int flag;
  size_t pos;

  if(-1 == api_dir_read(&sa, dn->s))
    xbailout(111, errno, "failed to open/read ", dn->s, 0, 0);
  dn->len--;
  pos = dn->len;

  for(e = api_dir_walkstart(&sa, &flag); e; e = api_dir_walknext(&sa, &flag)) {
    struct stat st;
    int found;

    stralloc* t;
    dn->len = pos;

    if(e[0] == '.' && (!e[1] || (e[1] == '.' && e[2] == 0)))
      continue;

    if(!stralloc_append(dn, "/"))
      oom();

    if(!stralloc_cats(dn, e))
      oom();

    if(!stralloc_0(dn))
      oom();

    if(-1 == lstat(dn->s, &st))
      continue;

    if(S_ISDIR(st.st_mode)) {
      delete(dn);
      t = canon(dn);
      found = strhash_lookup(&hash_fns, t->s, t->len, 0, 0);

      if(!found) {
        if(o_dry_run)

          do_log2(dn->s,
                  ": not found on remote,                   ould be deleted\n");
        else {
          if(-1 == rmdir(dn->s))
            xbailout(100, errno, "failed to rmdir ", dn->s, 0, 0);

          if(o_loglevel)

            do_log2(dn->s,
                    ": not found on                     emote, deleted\n");
        }
      } else {
        if(o_loglevel > 3)

          do_log2(dn->s, ": found on remote\n");
      }
      continue;
    }
    t = canon(dn);
    found = strhash_lookup(&hash_fns, t->s, t->len, 0, 0);

    if(!found) {
      if(o_max_deletes)

        if(count_deletes++ >= o_max_deletes) {
          if(o_loglevel)

            do_log2(dn->s,
                    ": not deleted due to                     -max-deletes\n");
          continue;
        }

      if(o_dry_run) {
        do_log2(dn->s,
                ": not found on remote,                 ould be deleted\n");
      } else {
        if(-1 == unlink(dn->s)) {
          unsigned int er = errno;

          if(-1 == rmdir(dn->s))
            xbailout(111, er, "failed to unlink ", dn->s, 0, 0);
        }

        if(o_loglevel)

          do_log2(dn->s,
                  ": not found on                   emote, deleted\n");
      }
    } else {
      if(o_loglevel > 3)

        do_log2(dn->s, ": found on remote\n");
    }
  }
  dn->len = pos;

  if(!stralloc_0(dn))
    oom();
  api_dir_free(&sa);
}

static void
usage(void) {
  xbailout(2,
           0,
           "usage: ftpcopy [options]            ost[:port] remotedir            localdir]\n"
           "   or: ftpcopy [options]            tp://host[:port]/remotedir            localdir]\n"
           "  use the --help option to get             description of the options.",
           0,
           0,
           0);
}

static int
callback_exclude(uogetopt_env* e, uogetopt2* g, char* s) {
  (void)g;
  (void)e;

  if(!stralloc_catb(&o_exclude, "-", 1) || !stralloc_cats(&o_exclude, s) || !stralloc_0(&o_exclude))
    oom();
  return 0;
}
static int
callback_include(uogetopt_env* e, uogetopt2* g, char* s) {
  (void)g;
  (void)e;

  if(!stralloc_catb(&o_exclude, "+", 1) || !stralloc_cats(&o_exclude, s) || !stralloc_0(&o_exclude))
    oom();
  return 0;
}
static void
include_exclude_file(const char* fname) {
  buffer io;
  stralloc s = STRALLOC_INIT;
  char spc[BUFFER_INSIZE];
  int fd = open_read(fname);

  if(-1 == fd)
    xbailout(111, errno, "failed to open_read ", fname, 0, 0);
  buffer_init(&io, (buffer_op)read, fd, spc, sizeof(spc));

  for(;;) {
    int gotlf;

    if(-1 == getln(&io, &s, &gotlf, '\n'))
      xbailout(111, errno, "failed to read from ", fname, 0, 0);

    if(s.len == 0)
      break;

    if(!gotlf)
      xbailout(111,
               errno,
               "unterminated line read                rom ",
               fname,
               0,
               0);
    s.len--;

    if(!s.len)
      continue; /* ignore empty lines */

    if(!stralloc_0(&s))
      oom(); /* cant happen */

    if(s.s[0] == '+')
      callback_include(0, 0, s.s + 1);
    else if(s.s[0] == '-')
      callback_exclude(0, 0, s.s + 1);
    else if(s.s[0] != '#')
      xbailout(111,
               0,
               fname,
               ": unknown tag at start                f line ",
               s.s,
               0);
  }
  close(fd);
  stralloc_free(&s);
}
static int
callback_ip(uogetopt_env* e, uogetopt2* g, char* s) {
  (void)g;
  (void)e;

  while(s && *s) {
    char ip[4];
    unsigned int x;
    x = ip4_scan(s, ip);

    if(!x || (s[x] != ',' && s[x] != 0))
      xbailout(2,
               0,
               "cannot parse IP                ddress at `",
               s,
               "'",
               0);

    if(!stralloc_catb(&pasv_response_ips, ip, 4))
      oom();
    s += x;

    if(*s)
      s++;
  }
  return 0;
}

static uogetopt2 myopts[] = {
    {0,
     "",
     uogo_label,
     0,
     0,
     0,
     "Connect / login / username /      assword options:",
     0,
     0},
    COMMON_OPT_user,
    COMMON_OPT_pass,
    COMMON_OPT_account,
    COMMON_OPT_tries,
    COMMON_OPT_data_connect_retries,
    COMMON_OPT_sleep,
    COMMON_OPT_v4,
    COMMON_OPT_v6,

    {0, "", uogo_label, 0, 0, 0, "Verbosity options:", 0, 0},
    {'l',
     "loglevel",
     uogo_ulong,
     0,
     &o_loglevel,
     0,
     "Controls the amount of logging      one.",
     "  0: nothing except warnings and      rror messages.\n"
     "  1: downloads and deletes (this      s the default).\n"
     "  2: links/symlinks created,      iles we already got.\n"
     "  3: useless stuff.",
     0},
    {0,
     "bps",
     uogo_flag,
     UOGO_NOARG,
     &o_bps,
     1,
     "Log transfer rates.",
     "This option causes ftpcopy to      og byte / kilobyte / megabyte      er second      nformation after successful      ransfers.\n"
     "This option was added in version      .3.9.",
     0},
    {0,
     "progress",
     uogo_flag,
     UOGO_NOARG,
     &o_progress,
     1,
     "Report progress to stderr.",
     "This will print a report of the      ownload every      econd: a short form of the file      ame, the bytes      ot and expected and the      ercentage received.\n"
     "This option was added in version      .6.0.",
     0},
    {0, "", uogo_label, 0, 0, 0, "File selection options:", 0, 0},
    {'m',
     "max-days",
     uogo_ulong,
     0,
     &o_max_days,
     0,
     "Download only files modified in      he last DAYS.",
     "Locally existing copies of the      ot downloaded files will be      ept. The      efault is not to restrict the      ge of files.",
     "DAYS"},
    {0,
     "max-size",
     uogo_ulong,
     0,
     &o_max_size,
     0,
     "Download only files up to      AXBYTES length.",
     "Locally existing copies of      verlong files will be deleted      uring the      lean-up step. The default is      ot to restrict the file size.\n"
     "This option was added in version      .5.1.",
     "MAXBYTES"},
    {'x',
     "exclude",
     callback_exclude,
     0,
     callback_exclude,
     0,
     /*2345678901234567890123456789012345678901234567890
      */
     "Exclude paths matching WILDCARD.",
     "If WILDCARD matches the full      ath of the remote file then the      ile will      ot be downloaded. WILDCARD is a      hell style wildcard expression,      ot a      egular expression like those of      rep. You can repeat this option      s      ften as you want, and you can      ntermix it with the --include      ption.\n"
     "If both includes and excludes      re used then the last matching      ne will      e honored. The list starts with      n implicit '--include *'.\n"
     "If the --tolower option is used      ogether with --exclude or      -include then      he in/exclude patterns have to      e written in lower case.\n"
     "This option was added in version      .3.0.",
     "WILDCARD"},
    {'i',
     "include",
     callback_include,
     0,
     callback_include,
     0,
     "Include paths matching WILDCARD.",
     "This is the opposite of the      -exclude option.\n"
     "It was added in version 0.3.0.",
     "WILDCARD"},
    {'X',
     "in-exclude-file",
     uogo_string,
     0,
     &o_inex_file,
     0,
     "Read in/exclude patterns from      ILE.",
     "The include and exclude patterns      re read from a file. If the      irst      haracter of a line is a '+' the      emainder of the line is treated      s      n argument of a --include      ptiona and if it is a '-' it is      reated as      n argument to a --exclude      ption. Lines starting with a      #' are ignored.\n"
     "FILE will be read after any      -include and --exclude options      iven on the      ommand line have been read.\n"
     "This option was added in version      .6.6.",
     "FILE"},
    {0,
     "ignore-size",
     uogo_flag,
     UOGO_NOARG,
     &o_ignore_size,
     1,
     "Ignore file size.",
     "Do not compare file sizes when      hecking the remote file has to      e      ownloaded.\n"
     "This option was added in version      .4.4.",
     0},
    {0,
     "ignore-time",
     uogo_flag,
     UOGO_NOARG,
     &o_ignore_time,
     1,
     "Ignore modification times.",
     "Do not compare file modification      imes when checking the remote      ile      as to be downloaded. This      ption may be combined with      -ignore-size,      n which case a file will never      e downloaded regardless of      hanges      n file size or modification      ime. In other words: ftpcopy      ill not      ownload any updates.\n"
     "This option was added in version      .4.4.",
     0},
    {0,
     "max-depth",
     uogo_ulong,
     0,
     &o_max_depth,
     0,
     "Descend at most LEVEL      irectories.",
     "  0 means `do not enter sub      irectories at all',\n"
     "  1 means `enter      ub-directories, but not their      ub-directories'.\n"
     "The default is 2^32-1 meaning      enter all'.",
     0},
    {0, "", uogo_label, 0, 0, 0, "Deletion options:", 0, 0},
    {'n',
     "no-delete",
     uogo_flag,
     UOGO_NOARG,
     &o_do_delete,
     0,
     "Do not delete files.",
     "This influences the cleanup step      hen getting rid      f things the server doesn't      ave anymore. It does not      top ftpcopy from deleting files      hen it detects something in      t's way      uring a download.",
     0},
    {'M',
     "max-deletes",
     uogo_ulong,
     0,
     &o_max_deletes,
     0,
     "Do not delete more then COUNT      iles.",
     "This option may be useful to      imit the impact      f a tempoary loss of files on      he server. This only influences      he      leanup step and does not stop      tpcopy to delete files in it's      ay      uring a download.      he default is 0, meaning      nlimited.\n"
     "This option was added in version      .4.5.",
     "COUNT"},

    {0, "", uogo_label, 0, 0, 0, "Operational options:", 0, 0},
    {'d',
     "directories-only",
     uogo_flag,
     UOGO_NOARG,
     &o_directories_only,
     1,
     "Only create the directory      ierarchie.",
     "Do not download files. Any file      n the tree will be deleted      nless      he -n option is also given.\n"
     "This option will be removed in      uture versions, unless someone      bjects.",
     0},
    {0,
     "dry-run",
     uogo_flag,
     UOGO_NOARG,
     &o_dry_run,
     1,
     "Don't do anything.",
     "ftpcopy will only show what      ould be done.\n"
     "This option was added in version      .3.6.",
     0},
    COMMON_OPT_timeout,
    {0,
     "rate-limit",
     uogo_ulong,
     0,
     &o_rate_limit,
     0,
     "Limit file download speed.",
     "Limit the transfer rate of file      ownloads to about that many      ytes per      econds. The implementation is      rude and simple, by sleeping up      o one      econd between network reads,      nd therefore does not even try      o limit      he rate exactly to that number.      n the other hand it usually      orks and      s unlikely to break things by      ausing timeouts.\n"
     "The default is unlimited.\n"
     "This option was added in version      .4.7.",
     "BYTES_PER_SECOND"},
    {0,
     "interactive",
     uogo_flag,
     UOGO_NOARG,
     &o_interactive,
     1,
     "Read directories from stdin.",
     "This option tells ftpcopy to      gnore any directories given on      he      ommand line, and to read      ommands from the standard      nput. Each      ommand consists of two lines,      he first being a directory on      he      emote server, and the second a      ocal directory.      tpcopy will print an      ND-OF-COPY line after each      peration.\n"
     "This option was added in version      .3.6 and will be removed in      uture versions, unless someone      bjects.",
     0},

    {0, "", uogo_label, 0, 0, 0, "Workaround options:", 0, 0},
    COMMON_OPT_ascii_listings,
    COMMON_OPT_list_options,
    {'s',
     "symlink-hack",
     uogo_flag,
     UOGO_NOARG,
     &o_symlink_hack,
     1,
     "Deal with symbolic links.",
     "This is only useful to mirror      ites which create listings      hrough /bin/ls, and will fail      f a file name in a link      ontains a       -> ' sequence.",
     0},
    COMMON_OPT_force_select,
    {0,
     "mdtm",
     uogo_flag,
     UOGO_NOARG,
     &o_mdtm,
     1,
     "Use the MDTM command to get the      emote time.",
     "The default is to take the times      rom the directory listings.      his doesn't      ork if the server implements an      nferior listing format (most      o) and      oesn't send time stamps in      niversal coordinated time      UTC). The damage      aused by this is limited to      ile time stamps being wrong by       few hours.\n"
     "This option makes ftpcopy send a      DTM command for any file      t might want to download. The      rawback is that this eats      erformance: ftpcopy usually      ends just one command for a      omplete      irectory its traverses. With      he --mdtm option it has to      end an additional command for      ny file.\n"
     "This option was added in version      .3.10.",
     0},
    COMMON_OPT_pasv_ip,
    {0,
     "no-resume",
     uogo_flag,
     UOGO_NOARG,
     &o_no_rest,
     1,
     "Do not try to resume downloads.",
     "The REST command, needed to      esume a failed\n"
     "download, is badly specified and      ikely to be misinterpreted and      implemented. Use this option in      ase of trouble.\n"
     "This option was added in version      .6.0.",
     0},
    {0,
     "tolower",
     uogo_flag,
     UOGO_NOARG,
     &o_tolower,
     1,
     "Change all local file names to      owercase.",
     "Use this only if you are      bsolutely sure that the remote      ide does not      ontain any files or directories      hose lower cased names collide      ith each other. Otherwise this      ption will waste bandwidth.\n"
     "If this option is used together      ith the --exclude or --include      ptions then      he in/exclude patterns have to      e written in lower case.\n"
     "This option was added in version      .3.8.",
     0},
    COMMON_OPT_eat_leading_spaces,

    {0, "", uogo_label, 0, 0, 0, "Help options:", 0, 0},
    {0,
     "include-exclude-help",
     uogo_print_help,
     UOGO_NOARG | UOGO_EXIT | UOGO_NOLHD,
     0,
     1,
     "How --include and --exclude      ork.",
     /* 12345678901234567890123456789012345678901234567890
      */
     "In- and exclude lists are      nternally mixed together,      eeping the      rder in which they were given.      he list starts with an implicit      include *'. ftpcopy honors the      ast match.\n\n"
     "The wildcard matching is done      gainst the full remote path of      he      ile. The `/' character has no      pecial meaning for the matching      nd      s treated like any other.\n\n"
     "Note: you have to include top      evel directories of files or      irectories you want to include.      omething like this will NOT      ork:\n"
     "    --exclude '*' --include      /w/h/e/r/e/file.c'\n"
     "You need to include /w, /w/h and      o on.",
     0},
    {0,
     "examples",
     uogo_print_help,
     UOGO_NOARG | UOGO_EXIT | UOGO_NOLHD,
     0,
     1,
     "Show usage examples.",
     /* 12345678901234567890123456789012345678901234567890
      */
     "mirror cr.yp.to:\n"
     "  ftpcopy  \\\n"
     "  --exclude '*.cdb'  \\\n"
     "  --exclude      *software/precompiled*' \\\n"
     "  cr.yp.to /      private/file/0/mirror/cr.yp.to\n"
     "This means:\n"
     "  * i'm not interested in .cdb      iles.\n"
     "  * precompiled stuff is also      ot downloaded.\n"
     "  * the host to connect to is      r.yp.to.\n"
     "  * the remote directory is /,      nd\n"
     "  * and      private/file/0/mirror/cr.yp.to      s the local directory.",
     0},
    {0, "author", uogo_print_help, UOGO_NOARG | UOGO_HIDDEN | UOGO_EXIT | UOGO_NOLHD, 0, 0, "Show author.", "Uwe Ohse, <uwe@ohse.de>.", 0},
    {0,
     "copyright",
     uogo_print_help,
     UOGO_NOARG | UOGO_HIDDEN | UOGO_EXIT | UOGO_NOLHD,
     0,
     0,
     "Show copyright.",
     "Copyright (C) 2003 Uwe Ohse.\n\n"
     "The software comes with NO      ARRANTY, to the extent      ermitted by law.\n\n"
     "This package is published unter      he terms of the GNU General      ublic License      ersion 2. Later versions of the      PL may or may not apply, see      ttp://www.ohse.de/uwe/licenses/"
     "\n",
     0},
    {0,
     "see-also",
     uogo_print_help,
     UOGO_NOARG | UOGO_EXIT | UOGO_NOLHD,
     0,
     0,
     "Where to find related      nformation.",
     "ftpls(1) lists ftp directories.\n"
     "ftpcp(1) is a frontend for      tpcopy.\n" COMMON_RELATED_INFO,
     0},

    {0, 0, 0, 0, 0, 0, 0, 0, 0} /* --help and --version */
};

static void
chdir_mkdir(const char* s) {
  if(-1 == chdir(s)) {
    if(-1 == mkdir(s, 0755)) {
      if(errno != error_exist)
        xbailout(111, errno, "failed to mkdir ", s, 0, 0);
    }

    if(-1 == chdir(s))
      xbailout(111, errno, "failed to chdir to ", s, 0, 0);
  }
}

static int
onedirpair(stralloc* remote, stralloc* local) {
  struct stat st;
  struct ftpparse fp;
  strhash_destroy(&hash_ids);
  strhash_destroy(&hash_fns);

  if(-1 == strhash_create(&hash_ids, 16, 32, strhash_hash))
    oom();

  if(-1 == strhash_create(&hash_fns, 16, 32, strhash_hash))
    oom();
  byte_zero((char*)&fp, sizeof(fp));

  if(remote->s[0] != '/') {
    /* i like absolute paths. */
    stralloc t = STRALLOC_INIT;

    if(!stralloc_copy(&t, remote))
      oom();

    if(!stralloc_copyb(remote, "/", 1))
      oom();

    if(!stralloc_cat(remote, &t))
      oom();
    stralloc_free(&t);
  }

  if(!initialentity(&fp, remote)) {
    /* 'remote' is a file */
    unsigned int slash;
    /* 'local' now is a file, too */
    slash = str_rchr(local->s, '/');
    local->s[slash] = 0;
    chdir_mkdir(local->s);
    local->s[slash] = '/';

    if(0 == stat(local->s, &st) && S_ISDIR(st.st_mode)) {
      slash = str_rchr(remote->s, '/');
      local->len--;

      if(local->s[local->len - 1] == '/')
        slash++; /* don't need a second
                    / */

      if(!stralloc_cats(local, remote->s + slash) || !stralloc_0(local))
        oom();
    }

    if(download(&fp, remote, local))
      return 0;
  }
  /* 'remote' is a directory */
  chdir_mkdir(local->s);

  if(fp.idlen)
    /* for the weird symlinks to the
     * servers root */
    hash_it(&fp, local, 1);

  if(loop(remote, local)) {
    if(o_do_delete)
      delete(local);
    return 0;
  }
  return 1;
}
static uogetopt_env optenv = {0,
                              PACKAGE,
                              VERSION,
                              "usage: ftpcopy [options]                               ost[:port] remotedir [localdir]\n"
                              "   or: ftpcopy [options]                               tp://host[:port]/remotedir                               localdir]",
                              "create and maintain a ftp mirror.",
                              "ftpcopy copies a FTP site                               ecursivly. It afterwards deletes                               ll files in                               he local directory tree which                               ere not found on the remote                               ite.\n\n"
                              "local-directory defaults to `.' -                               he current working directory -                               f the                               -no-delete option is used.                               ocal-directory is not needed if                               he                               -interactive option is used.\n"
                              "Otherwise you must provide a                               ocal-directory argument.\n",
                              COMMON_BUGREPORT_INFO,
                              0,
                              0,
                              0,
                              0,
                              uogetopt_out,
                              myopts};

int
main(int argc, char** argv) {
  char* local_start_dir = 0;
  const char *host, *remotedir, *localdir = 0;
  stralloc d1 = STRALLOC_INIT;
  stralloc d2 = STRALLOC_INIT;
  stralloc proto = STRALLOC_INIT;
  stralloc user = STRALLOC_INIT;
  stralloc pass = STRALLOC_INIT;
  stralloc hostport = STRALLOC_INIT;
  stralloc rest = STRALLOC_INIT;
  int ldirfd;

  bailout_progname(argv[0]);
  flag_bailout_fatal_begin = 3;
  optenv.program = flag_bailout_log_name;

  if(!stralloc_ready(&io_d_mem, BUFFER_INSIZE))
    oom();

  uogetopt_parse(&optenv, &argc, argv);

  if(o_v4_only && o_v6_only)
    xbailout(2,
             0,
             "the --v4 and --v6 options are              utally exclusive",
             0,
             0,
             0);

  if(o_v4_only)
    socket_flag_noipv6 = 1;

  if(o_v6_only)
    socket_flag_noipv4 = 1;

  if(argc < 2 || argc > 4)
    usage();
  setup_window_size();
  callback_ip(0, 0, env_get("FTPCOPY_ALLOW_PASV_IP"));

  if(o_inex_file)
    include_exclude_file(o_inex_file);

  if(urlparse(argv[1], &proto, &user, &pass, &hostport, &rest)) {
    if(!stralloc_0(&proto))
      oom();

    if(!str_equal(proto.s, "ftp"))
      xbailout(100, 0, "URL type `", proto.s, "' is not supported", 0);

    if(!hostport.len)
      xbailout(100, 0, "empty host in URL", 0, 0, 0);

    if(!stralloc_0(&hostport))
      oom();
    host = hostport.s;

    if(!rest.len)

      if(!stralloc_append(&rest, "/"))
        oom();

    if(!stralloc_0(&rest))
      oom();
    remotedir = rest.s;

    if(!o_user && user.len) {
      if(!stralloc_0(&user))
        oom();
      o_user = user.s;
    }

    if(!o_pass && pass.len) {
      if(!stralloc_0(&pass))
        oom();
      o_pass = pass.s;
    }

    if(argv[2]) {
      localdir = argv[2];

      if(argv[3])
        usage();
    }
  } else {
    host = argv[1];

    if(!argv[2])

      if(!o_interactive)
        usage();
    remotedir = argv[2];

    if(argv[3])
      localdir = argv[3];
  }

  if(!localdir && !o_interactive) {
    char dotdir[] = ".";

    if(o_do_delete)
      xbailout(100,
               0,
               "default `localdir' (.)                ot allowed without                he -n option",
               0,
               0,
               0);
    localdir = dotdir;
  }

  if(!o_user)
    o_user = "anonymous";

  if(!o_pass)
    o_pass = "anonymous@example.invalid";

  if(remotedir) { /* might be empty in
                     interactive mode */

    if(!stralloc_copys(&d1, remotedir))
      oom();

    if(!stralloc_0(&d1))
      oom();
  }

  local_start_dir = get_cwd();

  if(!local_start_dir)
    xbailout(111,
             errno,
             "failed to get current              irectory",
             0,
             0,
             0);

  ldirfd = open_read(".");

  if(ldirfd == -1)
    xbailout(111, errno, "failed to open . for reading", 0, 0, 0);

  connect_auth(host, o_user, o_pass, o_acct, o_tries);

  sx2("TYPE I");

  if(o_interactive) {
    int retcode = 0;
    buffer io_stdin;
    char spc[BUFFER_INSIZE];
    buffer_init(&io_stdin, (buffer_op)read, 0, spc, sizeof(spc));

    for(;;) {
      int gotlf;
      char* p;

      if(-1 == getln(&io_stdin, &d1, &gotlf, '\n'))
        xbailout(111, errno, "failed to read from stdin", 0, 0, 0);

      if(d1.len == 0)
        break;

      if(!gotlf)
        xbailout(111,
                 errno,
                 "unterminated line                  ead from stdin",
                 0,
                 0,
                 0);
      d1.len--;

      if(!stralloc_0(&d1))
        oom();

      if(-1 == getln(&io_stdin, &d2, &gotlf, '\n'))
        xbailout(111, errno, "failed to read from stdin", 0, 0, 0);

      if(d2.len == 0)
        break;

      if(!gotlf)
        xbailout(111,
                 errno,
                 "unterminated line                  ead from stdin",
                 0,
                 0,
                 0);
      d2.len--;

      if(!stralloc_0(&d2))
        oom();

      if(d2.s[0] != '/') {
        stralloc x = STRALLOC_INIT;

        if(!stralloc_copys(&x, local_start_dir))
          oom();

        if(!stralloc_append(&x, "/"))
          oom();

        if(!stralloc_cat(&x, &d2))
          oom();

        if(!stralloc_copy(&d2, &x))
          oom();
        stralloc_free(&x);
      }

      /* back to local start dir. make
       * non-absolute directories work
       */

      if(-1 == fchdir(ldirfd))
        xbailout(111,
                 errno,
                 "failed to fchdir to                  tarting directory",
                 0,
                 0,
                 0);
      /* back to remote root */
      cmdwrite1("CWD /");
      p = ccread();

      if(!p)
        eof_or_error(111, errno, "failed to read CWD answer", 0, 0);

      if(*p != '2') {
        do_log1(p);

        do_log1("\n");
        retcode = 1;
      } else
        retcode = onedirpair(&d1, &d2);

      do_log1("END-OF-COPY\n");
    }
    return retcode;
  }

  if(*localdir == '/') {
    if(!stralloc_copys(&d2, localdir))
      oom();

    if(!stralloc_0(&d2))
      oom();
  } else {
    if(!stralloc_copys(&d2, local_start_dir))
      oom();

    if(!stralloc_append(&d2, "/"))
      oom();

    if(!stralloc_cats(&d2, localdir))
      oom();

    if(!stralloc_0(&d2))
      oom();
  }
  return onedirpair(&d1, &d2);
}
