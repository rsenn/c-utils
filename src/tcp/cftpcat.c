#include "../../lib/alloc.h"
#include "../../lib/buffer.h"
#include "../../lib/byte.h"
#include "../../lib/case.h"
#include "../../lib/errmsg.h"
#include "../../lib/scan.h"
#include "../../lib/str.h"
#include "../../lib/stralloc.h"
#include "../../lib/uri.h"
#include "../../lib/unix.h"
#include "../dnsutils/address.h"

#include <unistd.h>

typedef struct {
  int fd;
  buffer in, out;
  char inbuf[4096], outbuf[1024];
  stralloc host;
  stralloc reply;
  char type;
} ftp;

static const char *user = "anonymous", *pass = "anonymous@", *output_file;
static int verbose;
static buffer file_out, *out;

static void
usage(const char* argv0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(argv0),
                       " [OPTIONS] ftp://[USER[:PASS]@]HOST[:PORT]/PATH\n"
                       "       ",
                       str_basename(argv0),
                       " [OPTIONS] HOST COMMAND [ARGS...]\n"
                       "\n"
                       "Writes a file or a directory listing of an FTP server to standard output.\n"
                       "A URL ending in a slash lists the directory, any other path is downloaded.\n"
                       "\n"
                       "Commands, several are separated by a semicolon:\n"
                       "  ls [ARGS]            directory listing, ARGS go to the server like -lR /pub\n"
                       "  nlst [ARGS]          names only\n"
                       "  cat FILE             download FILE (also get, retr)\n"
                       "  cd DIR               change the directory\n"
                       "  pwd, size FILE       print the server's answer\n"
                       "  quote LINE           send LINE as it is and print the answer\n"
                       "\n"
                       "  -u, --user NAME      login name, default anonymous\n"
                       "  -p, --pass PASSWORD  password, default anonymous@\n"
                       "  -P, --port PORT      control port, default 21\n"
                       "  -o, --output FILE    write to FILE instead of standard output\n"
                       "  -v, --verbose        show the dialog on standard error\n"
                       "  -h, --help           show this help\n"
                       "\n"
                       "Data connections are passive. The exit status is 1 if a command failed.\n",
                       NULL);
  buffer_flush(buffer_1);
}

static ssize_t
ftp_read(fd_type fd, void* x, size_t n, buffer* b) {
  (void)b;
  return read(fd, x, n);
}

static ssize_t
ftp_write(fd_type fd, void* x, size_t n, buffer* b) {
  (void)b;
  return write(fd, x, n);
}

/* reads a reply, a continued one ends with the same code and a space.
 * Returns the code, in f->reply the text of the last line, 0 on error. */
static unsigned
ftp_reply(ftp* f) {
  stralloc line;
  unsigned code = 0, last = 0;
  int multi = 0;

  stralloc_init(&line);

  for(;;) {
    stralloc_zero(&line);

    if(buffer_getline_sa(&f->in, &line) <= 0) {
      errmsg_warn("connection closed by the server", 0);
      return 0;
    }
    stralloc_trimr(&line, "\r\n", 2);
    stralloc_nul(&line);

    if(verbose) {
      buffer_putm_internal(buffer_2, "< ", line.s, "\n", NULL);
      buffer_flush(buffer_2);
    }

    if(line.len >= 4 && scan_uint(line.s, &code) == 3 && (line.s[3] == ' ' || line.s[3] == '-')) {
      if(!multi) {
        multi = line.s[3] == '-';
        last = code;
      }
      if(line.s[3] == ' ' && code == last)
        break;
    }
  }
  stralloc_copyb(&f->reply, line.s + 4, line.len - 4);
  stralloc_nul(&f->reply);
  stralloc_free(&line);
  return code;
}

static unsigned
ftp_command(ftp* f, const char* cmd, const char* arg) {
  if(verbose) {
    buffer_putm_internal(buffer_2, "> ", cmd, arg && *arg ? " " : "", arg && *arg ? arg : "", "\n", NULL);
    buffer_flush(buffer_2);
  }

  buffer_puts(&f->out, cmd);
  if(arg && *arg) {
    buffer_putc(&f->out, ' ');
    buffer_puts(&f->out, arg);
  }
  buffer_puts(&f->out, "\r\n");
  buffer_flush(&f->out);

  return ftp_reply(f);
}

static void
fail(const ftp* f, const char* what) {
  errmsg_warn(what, ": ", f->reply.s ? f->reply.s : "no reply", 0);
}

/* opens the data connection, tries EPSV and falls back to PASV */
static int
ftp_data(ftp* f) {
  unsigned code;
  unsigned long port = 0, n[6];
  const char* x;
  size_t i;

  if((code = ftp_command(f, "EPSV", 0)) == 229) {
    x = f->reply.s + str_chr(f->reply.s, '|');
    if(x[0] == '|' && x[1] == '|' && x[2] == '|' && scan_ulong(x + 3, &port))
      return address_connect(f->host.s, (uint16)port);
  }

  if((code = ftp_command(f, "PASV", 0)) != 227) {
    fail(f, "PASV");
    return -1;
  }

  x = f->reply.s + str_chr(f->reply.s, '(');
  if(*x)
    x++;

  for(i = 0; i < 6; i++) {
    size_t l = scan_ulong(x, &n[i]);

    if(!l)
      break;
    x += l;
    if(*x == ',')
      x++;
  }
  if(i < 6) {
    errmsg_warn("cannot read the address of the data connection: ", f->reply.s, 0);
    return -1;
  }

  /* servers behind a NAT report their private address, the one we reached is good */
  port = (n[4] << 8) | n[5];
  return address_connect(f->host.s, (uint16)port);
}

static int
ftp_type(ftp* f, char t) {
  char arg[2] = {t, 0};

  if(f->type == t)
    return 0;

  if(ftp_command(f, "TYPE", arg) / 100 != 2) {
    fail(f, "TYPE");
    return -1;
  }
  f->type = t;
  return 0;
}

/* runs a transfer command (LIST, NLST, RETR) and copies the data to out */
static int
ftp_transfer(ftp* f, const char* cmd, const char* arg, char type) {
  buffer data;
  char dbuf[8192], tmp[4096];
  int d;
  ssize_t r;
  unsigned code;
  int ok = 0;

  if(ftp_type(f, type) < 0 || (d = ftp_data(f)) < 0)
    return -1;

  if((code = ftp_command(f, cmd, arg)) / 100 != 1) {
    fail(f, cmd);
    close(d);
    return -1;
  }

  buffer_init(&data, ftp_read, d, dbuf, sizeof(dbuf));

  while((r = buffer_get(&data, tmp, sizeof(tmp))) > 0) {
    if(type == 'A') {
      /* a text transfer ends its lines with CR LF */
      ssize_t i;

      for(i = 0; i < r; i++)
        if(tmp[i] != '\r')
          buffer_putc(out, tmp[i]);
    } else {
      buffer_put(out, tmp, (size_t)r);
    }
  }
  ok = r == 0;
  buffer_flush(out);
  close(d);

  if(ftp_reply(f) / 100 != 2) {
    fail(f, cmd);
    return -1;
  }
  return ok ? 0 : -1;
}

/* runs one command word and its arguments */
static int
ftp_run(ftp* f, char* cmd) {
  char* arg;
  unsigned code;

  while(*cmd == ' ')
    cmd++;
  arg = cmd + str_chr(cmd, ' ');
  if(*arg) {
    *arg++ = '\0';
    while(*arg == ' ')
      arg++;
  }

  if(str_equal(cmd, "ls"))
    return ftp_transfer(f, "LIST", arg, 'A');
  if(str_equal(cmd, "nlst"))
    return ftp_transfer(f, "NLST", arg, 'A');
  if(str_equal(cmd, "cat") || str_equal(cmd, "get") || str_equal(cmd, "retr"))
    return ftp_transfer(f, "RETR", arg, 'I');

  if(str_equal(cmd, "cd"))
    code = ftp_command(f, "CWD", arg);
  else if(str_equal(cmd, "pwd"))
    code = ftp_command(f, "PWD", 0);
  else if(str_equal(cmd, "size"))
    code = ftp_command(f, "SIZE", arg);
  else if(str_equal(cmd, "quote")) {
    char* a2 = arg + str_chr(arg, ' ');

    if(*a2)
      *a2++ = '\0';
    code = ftp_command(f, arg, a2);
  } else {
    errmsg_warn("unknown command ", cmd, 0);
    return -1;
  }

  if(code / 100 != 2 && code / 100 != 3) {
    fail(f, cmd);
    return -1;
  }

  if(!str_equal(cmd, "cd")) {
    buffer_putsa(out, &f->reply);
    buffer_putc(out, '\n');
    buffer_flush(out);
  }
  return 0;
}

static int
ftp_login(ftp* f, const char* host, uint16 port) {
  unsigned code;

  if((f->fd = address_connect(host, port)) < 0)
    return -1;

  buffer_init(&f->in, ftp_read, f->fd, f->inbuf, sizeof(f->inbuf));
  buffer_init(&f->out, ftp_write, f->fd, f->outbuf, sizeof(f->outbuf));
  stralloc_copys(&f->host, host);
  stralloc_nul(&f->host);
  f->type = 'A';

  if(ftp_reply(f) / 100 != 2) {
    fail(f, "greeting");
    return -1;
  }

  code = ftp_command(f, "USER", user);
  if(code / 100 == 3)
    code = ftp_command(f, "PASS", pass);

  if(code / 100 != 2) {
    fail(f, "login");
    return -1;
  }
  return 0;
}

int
main(int argc, char* argv[]) {
  static const struct unix_longopt opts[] = {{"user", 1, NULL, 'u'},
                                             {"pass", 1, NULL, 'p'},
                                             {"port", 1, NULL, 'P'},
                                             {"output", 1, NULL, 'o'},
                                             {"verbose", 0, NULL, 'v'},
                                             {"help", 0, NULL, 'h'},
                                             {0, 0, 0, 0}};
  int c, index = 0, rc = 0;
  unsigned long port = 21;
  ftp f;
  uri_t u;
  stralloc cmds;
  char* x;

  errmsg_iam(argv[0]);

  while((c = unix_getopt_long(argc, argv, "u:p:P:o:vh", opts, &index)) != -1) {
    switch(c) {
      case 'u': user = unix_optarg; break;
      case 'p': pass = unix_optarg; break;
      case 'P':
        if(!scan_ulong(unix_optarg, &port) || port > 65535) {
          errmsg_warn("invalid port ", unix_optarg, 0);
          return 2;
        }
        break;
      case 'o': output_file = unix_optarg; break;
      case 'v': verbose = 1; break;
      case 'h': usage(argv[0]); return 0;
      default: return 2;
    }
  }

  if(unix_optind >= argc) {
    usage(argv[0]);
    return 2;
  }

  byte_zero(&f, sizeof(f));
  stralloc_init(&cmds);
  uri_init(&u);

  if(case_starts(argv[unix_optind], "ftp://")) {
    uri_scan(&u, argv[unix_optind], str_len(argv[unix_optind]));

    if(!u.host) {
      errmsg_warn("no host in ", argv[unix_optind], 0);
      return 2;
    }
    if(u.username)
      user = u.username;
    if(u.password)
      pass = u.password;
    if(u.port)
      port = u.port;

    /* a path ending in a slash, or none, is a directory */
    if(!u.location || u.location[0] == '\0' || u.location[str_len(u.location) - 1] == '/') {
      stralloc_copys(&cmds, "ls ");
      stralloc_cats(&cmds, u.location ? u.location : "/");
    } else {
      stralloc_copys(&cmds, "cat ");
      stralloc_cats(&cmds, u.location);
    }
  } else {
    int i;

    if(argc - unix_optind < 2) {
      usage(argv[0]);
      return 2;
    }
    u.host = argv[unix_optind];

    for(i = unix_optind + 1; i < argc; i++) {
      if(i > unix_optind + 1)
        stralloc_cats(&cmds, " ");
      stralloc_cats(&cmds, argv[i]);
    }
  }
  stralloc_nul(&cmds);

  if(output_file && !str_equal(output_file, "-")) {
    if(buffer_truncfile(&file_out, output_file) < 0) {
      errmsg_warnsys("cannot write ", output_file, 0);
      return 1;
    }
    out = &file_out;
  } else {
    out = buffer_1;
  }

  if(ftp_login(&f, u.host, (uint16)port) < 0)
    return 1;

  for(x = cmds.s; *x;) {
    char* e = x + str_chr(x, ';');
    char saved = *e;

    *e = '\0';
    if(ftp_run(&f, x) < 0) {
      rc = 1;
      break;
    }
    *e = saved;
    x = saved ? e + 1 : e;
  }

  ftp_command(&f, "QUIT", 0);
  buffer_flush(out);
  close(f.fd);
  return rc;
}
