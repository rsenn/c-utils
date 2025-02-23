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
#include <time.h> /* ftpparse needs it */
//#include "attributes.h"
#include "lib/stralloc.h"
//#include "ftpparse.h"
#include "lib/buffer.h"
//#include "ftplib.h"
#include "lib/alloc.h"
#include "lib/ip4.h"
//#include "uogetopt.h"
//#include "uotime.h"
//#include "urlparse.h"
#include "lib/iopause.h"
//#include "getln.h"
#include "lib/fmt.h"
#include "lib/str.h"
//#include "error.h"
//#include "ftpcopy.h"
//#include "bailout.h"
//#include "close.h"
#include "lib/str.h"
//#include "readwrite.h"
//#include "sort_it.h"
//#include "strhash.h"
#include "lib/socket.h"
#include "lib/env.h"
//#include "fs64.h"

static strhash hash;

#define TAI2UNIX(t) ((t)->x - 4611686018427387914ULL)

buffer io_i;
buffer io_o;
int data_sock = -1;
stralloc pasv_response_ips;
static const char* o_user;
static const char* o_pass;
static const char* o_acct;
static const char* o_title;
static const char* o_urlprefix;
static int may_mlsx = 1;
static int o_interactive;
static int o_recursive;
unsigned long o_loglevel = 1;
static int o_html = 0;
unsigned long o_timeout = 30;
unsigned long o_login_sleep = 5;
static unsigned long o_max_depth = 4294967295UL;
static int o_machine = 0;
extern unsigned long sortmode;
static const char* o_list_options = 0;
static stralloc urlprefix;
static unsigned long o_tries = 1;
static unsigned int dirprintoffset = 0;
static int o_raw = 0;
static int o_print_dir = 0;
static int o_ascii_listings;
stralloc remoteip;
static int o_v4_only;
static int o_v6_only;

static void usage(void) attribute_noreturn;

#define KB (1024)
#define MB (1024 * 1024)
static void
ulong_pad(stralloc* out, unsigned long l, unsigned int ml) {
  char buf[FMT_ULONG];
  l = fmt_ulong(buf, l);
  if(l < ml)
    if(!stralloc_catb(out, "    ", ml - l))
      oom();
  if(!stralloc_catb(out, buf, l))
    oom();
}

static void
mksize(stralloc* out, uint64 l) {
  if(l < 10000) {
    ulong_pad(out, l, 4);
    if(!stralloc_cats(out, "    B"))
      oom();
  } else if(l < 9 * MB) {
    ulong_pad(out, l / KB, 4);
    if(!stralloc_cats(out, "."))
      oom();

    ulong_pad(out, (l - (l / KB) * KB) / 102, 1);
    if(!stralloc_cats(out, " KB"))
      oom();
  } else {
    ulong_pad(out, l / MB, 4);
    if(!stralloc_cats(out, "."))
      oom();
    ulong_pad(out, (l - (l / MB) * MB) / (102 * KB), 1);
    if(!stralloc_cats(out, " MB"))
      oom();
  }
}

static void
entity(stralloc* d, struct ftpparse* x) {
  char nb[FMT_ULONG];
  char db[20];
  static stralloc sizebuf = STRALLOC_INIT;
  uo_sec70_t s70;
  uo_datetime_t dt;
  s70 = TAI2UNIX(&x->mtime);
  uo_sec702dt(&dt, &s70);
  yyyy_mm_dd_hh_mm_ss(db, &dt, '-');
  if(o_machine) {
    do_log1("N");
    if(d->s[dirprintoffset])
      do_log2(d->s + dirprintoffset, "/");
    do_logmem(x->name, x->namelen);
    do_log1("\nK");
    if(x->symlink)
      do_log1("link"); /* guess */
    else if(x->flagtrycwd)
      do_log1("dir");
    else
      do_log1("file");
    do_log1("\nS");
    nb[fmt_uint64(nb, x->size)] = 0;
    do_log4(nb, "\nT", db, "\n");
  } else if(o_html) {
    do_log1("<dt><a href=\"");
    do_logmem(urlprefix.s, urlprefix.len);
    if(d->s[dirprintoffset])
      do_log2(d->s + dirprintoffset, "/");
    do_logmem(x->name, x->namelen);

    /* this is certainly not the right
     * way to do it, but what is the
     * right way? */
    if(x->flagtrycwd && !x->flagtryretr && !x->symlink)
      do_logmem("/", 1);

    do_log1("\">");
    if(d->s[dirprintoffset])
      do_log2(d->s + dirprintoffset, "/");
    do_logmem(x->name, x->namelen);
    do_log1("</a><br>\n");
    do_log3("<dd> last modified ", db, ", ");
    sizebuf.len = 0;
    mksize(&sizebuf, x->size);
    do_logmem(sizebuf.s, sizebuf.len);
    do_log1("\n");
  } else {
    if(x->flagtrycwd && x->flagtryretr)
      do_log1("link "); /* guess */
    else if(x->flagtrycwd)
      do_log1("dir  ");
    else
      do_log1("file ");
    do_log2(db, "  ");
    sizebuf.len = 0;
    mksize(&sizebuf, x->size);
    do_logmem(sizebuf.s, sizebuf.len);
    do_log1(" ");
    if(d->s[dirprintoffset])
      do_log2(d->s + dirprintoffset, "/");
    do_logmem(x->name, x->namelen);
    do_log1("\n");
  }
}

static int
get_listing(stralloc* r_dir, stralloc* dirdata, int* dirlines) {
  char* p;
  int got_it;
  int listno;

  if(data_sock == -1)
    data_sock = do_pasv();

  cmdwrite2(r_dir->s[0] == '/' ? "CWD " : "CWD /", r_dir->s);

  p = ccread();
  if(!p)
    eof_or_error(111, errno, "failed to read `CWD /", r_dir->s, " answer");
  if(*p != '2') {
    warning(0, "unwanted answer to CWD ", r_dir->s, ": ", p);
    return 0;
  }
  got_it = 0;
  if(may_mlsx)
    listno = 0;
  else
    listno = 1;
retry_listing:
  if(listno == 0)
    cmdwrite1("MLSD");
  else if(o_list_options)
    cmdwrite2("LIST ", o_list_options);
  else
    cmdwrite1("LIST");

  p = ccread();
  if(!p) {
    const char* x;
    if(!listno)
      x = "MLSD";
    else
      x = "LIST";
    eof_or_error(111, errno, "failed to read ", x, " answer");
  }
  if(listno == 0 && *p == '5') {
    listno = 1;
    if(!str_start(p, "501"))
      may_mlsx = 0;
    goto retry_listing;
  }

  if(*p != '1') {
    warning(0, "unwanted answer to LIST/MLSD ", r_dir->s, ": ", p);
    return 0;
  }
  *dirlines = ftp_read_list(data_sock, dirdata);
  if(-1 == *dirlines)
    xbailout(111,
             errno,
             "failed to read directory "
             "listing",
             0,
             0,
             0);
  close(data_sock);
  data_sock = -1;
  x2("LIST/MLSD");

  return 1;
}

static int
parseit(stralloc* r_dir, stralloc* dirdata, int dirlines) {
  char *p, *e;
  int ind;
  struct ftpparse* parsed;
  int i;
  p = dirdata->s;
  e = dirdata->s + dirdata->len;
  parsed = (struct ftpparse*)alloc(sizeof(*parsed) * dirlines);
  if(!parsed)
    oom();
  ind = 0;

  while(p != e) {
    unsigned int l = str_len(p);
    int ok;
    if(o_raw) {
      do_logmem(p, l);
      do_logmem("\n", 1);
    }
    if(may_mlsx) {
      ok = ftpparse_mlsx(&parsed[ind], p, l, 0);
      if(ok && parsed[ind].flagbrokenmlsx)
        warn_broken_mlsx();
    } else {
      ok = ftpparse(&parsed[ind], p, l, o_eat_leading_spaces);
    }
    if(!ok) {
      if(!str_start(p, "total") && !str_start(p, "Total"))
        do_log3("cannot parse LIST line: ", p, "\r\n");
    } else if(parsed[ind].name[0] == '.' && (parsed[ind].namelen == 1 || (parsed[ind].namelen == 2 && parsed[ind].name[1] == '.'))) {
      if(o_loglevel > 1) {
        do_log2(r_dir->s, "/");
        do_logmem(parsed[ind].name, parsed[ind].namelen);
        do_log1(": ignored\n");
      }
    } else {
      ind++;
    }
    p += l + 1;
  }
  if(!o_raw)
    sort_it(parsed, ind);
  /* output */
  if(!o_raw)
    for(i = 0; i < ind; i++)
      entity(r_dir, &parsed[i]);
  if(o_recursive)
    for(i = 0; i < ind; i++) {
      struct ftpparse* x;
      stralloc t = STRALLOC_INIT;
      char* id;
      unsigned int idlen;
      x = &parsed[i];
      if(x->flagtryretr || !x->flagtrycwd)
        continue;
      if(x->idlen) {
        idlen = x->idlen;
        id = x->id;
      } else {
        if(!stralloc_copy(&t, r_dir))
          oom();
        if(!stralloc_append(&t, "/"))
          oom();
        if(!stralloc_catb(&t, x->name, x->namelen))
          oom();
        id = t.s;
        idlen = t.len;
      }
      if(strhash_lookup(&hash, id, idlen, 0, 0)) {
      } else if(o_max_depth) {
        unsigned int pos;
        stralloc dd = STRALLOC_INIT;
        int dl = 0;
        r_dir->len--;
        pos = r_dir->len;

        if(1 != strhash_enter(&hash, 1, id, idlen, 0, "", 0))
          oom();

        if(r_dir->s[r_dir->len - 1] != '/')
          if(!stralloc_append(r_dir, "/"))
            oom();
        if(!stralloc_catb(r_dir, x->name, x->namelen))
          oom();
        if(!stralloc_0(r_dir))
          oom();
        if(get_listing(r_dir, &dd, &dl)) {
          if(o_html) {
            do_log2("<dt>Listing of:<dd>", r_dir->s);
            do_log1("<dl>\n");
          }
          if(!o_html && o_print_dir) {
            do_logmem(r_dir->s, r_dir->len - 1);
            do_logmem(":\n", 2);
          }
          o_max_depth--;
          parseit(r_dir, &dd, dl);
          o_max_depth++;
          if(o_html)
            do_log1("</dl>\n");
        }
        stralloc_free(&dd);
        r_dir->len = pos;
        if(!stralloc_0(r_dir))
          oom();
      }
      stralloc_free(&t);
    }
  return 1;
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
               "cannot parse IP "
               "address at `",
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
     "Connect / login / username / "
     "password options:",
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

    {0, "", uogo_label, 0, 0, 0, "Operational options:", 0, 0},
    {0,
     "interactive",
     uogo_flag,
     UOGO_NOARG,
     &o_interactive,
     1,
     "Read directories from stdin.",
     "This option tells ftpls to "
     "ignore any directories given on "
     "the "
     "command line, and to read the "
     "directories to list from the "
     "standard "
     "input, reading one directory per "
     "line."
     "ftpls will print an END-OF-COPY "
     "line after each operation. Do "
     "not "
     "use this option together with "
     "--html.\n"
     "This option was added in version "
     "0.3.6 and will be removed in "
     "future versions, unless someone "
     "objects.",
     0},
    COMMON_OPT_timeout,
    {'R',
     "recursive",
     uogo_flag,
     UOGO_NOARG,
     &o_recursive,
     1,
     "Do recursive listing.",
     "This option makes ftpls descend "
     "through the directory hierarchy "
     "and list "
     "all the directories and files it "
     "files. A top level directories "
     "content "
     "will be printed completely "
     "before the sub directories will "
     "be traversed.",
     0},
    {0,
     "raw",
     uogo_flag,
     UOGO_NOARG,
     &o_raw,
     1,
     "Raw output in original format.",
     "This will preserve the original "
     "format and sort order.",
     0},
    {0,
     "max-depth",
     uogo_ulong,
     0,
     &o_max_depth,
     0,
     "Descend at most LEVEL "
     "directories.",
     "  0 means `do not enter sub "
     "directories at all',\n"
     "  1 means `enter "
     "sub-directories, but not their "
     "sub-directories'.\n"
     "The default is 2^32-1 meaning "
     "`enter all'.\n"
     "This option enables the "
     "--recursive option.\n",
     0},
    {0, "", uogo_label, 0, 0, 0, "Sorting options:", 0, 0},
    {'a', "alpha", uogo_flag, UOGO_NOARG, &sortmode, SM_ALPHA, "sort alphabetical.", 0, 0},
    {'z', "omega", uogo_flag, UOGO_NOARG, &sortmode, SM_OMEGA, "sort reverse alphabetical.", 0, 0},
    {'n', "newest", uogo_flag, UOGO_NOARG, &sortmode, SM_NEW, "sort youngest first.", 0, 0},
    {'o', "oldest", uogo_flag, UOGO_NOARG, &sortmode, SM_OLD, "sort oldest first.", 0, 0},
    {'s', "shortest", uogo_flag, UOGO_NOARG, &sortmode, SM_SHORT, "sort shortest first.", 0, 0},
    {'l', "longest", uogo_flag, UOGO_NOARG, &sortmode, SM_LONG, "sort longest first.", 0, 0},
    {0, "", uogo_label, 0, 0, 0, "Output options:", 0, 0},
    {'h',
     "html",
     uogo_flag,
     UOGO_NOARG,
     &o_html,
     1,
     "Create HTML index.",
     "The index will contain all files "
     "together with modification "
     "time and size.\n"
     "Note: The links in this index "
     "will contain username and "
     "password if you "
     "include them into an URL given "
     "to ftpls on the command line "
     "(either as "
     "argument or through the "
     "--urlprefix option). They will "
     "not be included "
     "otherwise, for security and "
     "privacy reasons.",
     0},
    {'m', "machine", uogo_flag, UOGO_NOARG, &o_machine, 1, "Create machine parsable output.", 0, 0},
    {'t',
     "title",
     uogo_string,
     0,
     &o_title,
     0,
     "Title text to use on the HTML "
     "output.",
     0,
     "TEXT"},
    {'U',
     "urlprefix",
     uogo_string,
     0,
     &o_urlprefix,
     0,
     "URL-Prefix to use in listings.",
     "If this option is not given then "
     "an URL given on the command line "
     "will "
     "be used instead, and if this  "
     "has not been given an URL will "
     "be created "
     "from host, port and directory "
     "given on the command line.",
     "URL"},
    {0,
     "print-dir",
     uogo_flag,
     UOGO_NOARG,
     &o_print_dir,
     1,
     "Print sub directory names.",
     "This option makes ftpls print "
     "the name of a directory before "
     "it lists "
     "it's content. Note that the name "
     "of the top level directory of "
     "each "
     "listing will not be printed. "
     "This option is meant to be used "
     "together "
     "with --raw, and will not create "
     "useful output together with "
     "--html.",
     0},

    {0, "", uogo_label, 0, 0, 0, "Workaround options:", 0, 0},
    COMMON_OPT_ascii_listings,
    COMMON_OPT_list_options,
    COMMON_OPT_pasv_ip,
    COMMON_OPT_force_select,
    COMMON_OPT_eat_leading_spaces,
    {0, "", uogo_label, 0, 0, 0, "Help options:", 0, 0},
    {0,
     "examples",
     uogo_print_help,
     UOGO_NOARG | UOGO_EXIT | UOGO_NOLHD,
     0,
     1,
     "Show usage examples.",
     /* 12345678901234567890123456789012345678901234567890
      */

     "Generate a HTML index page:\n"
     "  ftpls \\\n"
     "  --html --title 'CVS tree index "
     "by date' \\\n"
     "  --urlprefix "
     "ftp://ftp.ohse.de/uwe/cvs-trees/ "
     " \\\n"
     "  --newest \\\n"
     "  "
     "ftp://serak.ohse.de/uwe/"
     "cvs-trees \\\n"
     "  > "
     "SOMEWHERE/cvs-trees/"
     "index-by-date.html\n",
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
     "The software comes with NO "
     "WARRANTY, to the extent "
     "permitted by law.\n\n"
     "This package is published unter "
     "the terms of the GNU General "
     "Public License "
     "version 2. Later versions of the "
     "GPL may or may not apply, see "
     "http://www.ohse.de/uwe/licenses/"
     "\n",
     0},
    {0,
     "see-also",
     uogo_print_help,
     UOGO_NOARG | UOGO_EXIT | UOGO_NOLHD,
     0,
     0,
     "Where to find related "
     "information.",
     "ftpcopy(1) creates and maintains "
     "ftp "
     "mirrors.\n" COMMON_RELATED_INFO,
     0},

    {0, 0, 0, 0, 0, 0, 0, 0, 0} /* --help and --version */
};

static void
usage(void) {
  bailout(0,
          "usage: ftpls [options] "
          "host[:port] [remotedir]\n"
          "   or: ftpls [options] URL\n"
          "  use the --help option to get "
          "a description of the options",
          0,
          0,
          0);
}

static int
onelisting(stralloc* d) {
  static stralloc dirdata = STRALLOC_INIT;
  static int dirlines = 0;
  dirprintoffset = d->len - 1; /* \0 */
  strhash_destroy(&hash);
  if(-1 == strhash_create(&hash, 16, 32, strhash_hash))
    oom();

  if(!get_listing(d, &dirdata, &dirlines))
    return 1;
  if(o_html) {
    do_log1("<html><head>\n");
    if(o_title)
      do_log3("<title>", o_title, "</title>\n");
    do_log1("</head><body>\n");
    if(o_title) {
      do_log3("<h1>", o_title, "</h1>\n");
    }
    do_log1("<dl>\n");
  }
  parseit(d, &dirdata, dirlines);
  if(o_html) {
    do_log1("</dl></body></html>\n");
  }
  if(o_interactive)
    if(!o_html)
      do_log1("END-OF-LISTING\n");
  return 0;
}

static uogetopt_env optenv = {0,
                              PACKAGE,
                              VERSION,
                              "usage: ftpls [options] "
                              "host[:port] [remotedir]\n"
                              "   or: ftpls [options] URL",
                              "generate a ftp directory listing.",
                              "ftpls generates a listing of "
                              "files and directories in a FTP "
                              "directory. "
                              "By default it prints a human "
                              "readable ascii output, but may "
                              "also create "
                              "HTML and machine parsable output.",
                              COMMON_BUGREPORT_INFO,
                              0,
                              0,
                              0,
                              0,
                              uogetopt_out,
                              myopts};

int
main(int argc, char** argv) {
  const char *host, *remotedir;
  stralloc d1 = STRALLOC_INIT;
  stralloc proto = STRALLOC_INIT;
  stralloc user = STRALLOC_INIT;
  stralloc pass = STRALLOC_INIT;
  stralloc hostport = STRALLOC_INIT;
  stralloc rest = STRALLOC_INIT;
  int retcode;

  bailout_progname(argv[0]);

  optenv.program = flag_bailout_log_name;
  uogetopt_parse(&optenv, &argc, argv);

  if(o_v4_only && o_v6_only)
    xbailout(2,
             0,
             "the --v4 and --v6 options are "
             "mutally exclusive",
             0,
             0,
             0);
  if(o_v4_only)
    socket_flag_noipv6 = 1;
  if(o_v6_only)
    socket_flag_noipv4 = 1;

  if(argc < 2 || argc > 3)
    usage();
  callback_ip(0, 0, env_get("FTPCOPY_ALLOW_PASV_IP"));
  if(o_max_depth != 4294967295UL)
    o_recursive = 1;

  if(urlparse(argv[1], &proto, &user, &pass, &hostport, &rest)) {
    if(!stralloc_0(&proto))
      oom();
    if(!str_equal(proto.s, "ftp"))
      xbailout(100, 0, "URL type `", proto.s, "' is not supported", 0);
    if(!hostport.len)
      xbailout(100, 0, "empty host in url", 0, 0, 0);
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
      o_user = pass.s;
    }
    if(argv[2])
      usage();

    if(!stralloc_copys(&urlprefix, argv[1]))
      oom();
    if(urlprefix.s[urlprefix.len - 1] != '/')
      if(!stralloc_append(&urlprefix, "/"))
        oom();
  } else {
    host = argv[1];
    remotedir = argv[2];
    if(!remotedir || !*remotedir)
      remotedir = "/";
    if(o_html && !o_interactive) {
      if(!stralloc_copys(&urlprefix, "ftp://"))
        oom();
      if(!stralloc_cats(&urlprefix, host))
        oom();
      if(!stralloc_cats(&urlprefix, remotedir))
        oom();
      if(urlprefix.s[urlprefix.len - 1] != '/')
        if(!stralloc_append(&urlprefix, "/"))
          oom();
    } else if(!stralloc_0(&urlprefix))
      oom();
  }
  if(!o_user)
    o_user = "anonymous";
  if(!o_pass)
    o_pass = "anonymous@example.invalid";
  if(o_urlprefix)
    if(!stralloc_copys(&urlprefix, o_urlprefix))
      oom();

  connect_auth(host, o_user, o_pass, o_acct, o_tries);
  if(o_ascii_listings)
    sx2("TYPE A");
  else
    sx2("TYPE I");
  retcode = 0;
  if(o_interactive) {
    buffer io_stdin;
    char spc[BUFFER_INSIZE];
    buffer_init(&io_stdin, (buffer_op)read, 0, spc, sizeof(spc));
    for(;;) {
      int gotlf;
      if(-1 == getln(&io_stdin, &d1, &gotlf, '\n'))
        xbailout(111, errno, "failed to read from stdin", 0, 0, 0);
      if(d1.len == 0)
        break;
      d1.len--;
      if(!stralloc_copys(&urlprefix, "ftp://"))
        oom();
      if(!stralloc_cats(&urlprefix, host))
        oom();
      if(!stralloc_cat(&urlprefix, &d1))
        oom();
      if(urlprefix.s[urlprefix.len - 1] != '/')
        if(!stralloc_append(&urlprefix, "/"))
          oom();
      if(!stralloc_0(&d1))
        oom();
      retcode = onelisting(&d1);
    }
  } else {
    if(!stralloc_copys(&d1, remotedir) || !stralloc_0(&d1))
      oom();
    retcode = onelisting(&d1);
  }
  return (0);
}
