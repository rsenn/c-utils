#define _XOPEN_SOURCE 1
#define __POSIX_VISIBLE 199209
#define __MISC_VISIBLE 1
#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/fmt.h"
#include "lib/unix.h"
#include "lib/http.h"
#include "lib/io.h"
#include "lib/iarray.h"
#include "lib/scan.h"
#include "lib/slist.h"
#include "lib/str.h"
#include "lib/stralloc.h"
#include "lib/strarray.h"
#include "lib/strlist.h"
#include "lib/windoze.h"
#include "lib/errmsg.h"
#include "lib/unix.h"
#include "lib/http.h"
#include "lib/io.h"
#include "lib/case.h"
#include "lib/hmap.h"

#if !defined(_WIN32) && !(defined(__MSYS__) && __MSYS__ == 1)
#include <libgen.h>
#endif

#include <string.h>
#include <time.h>

#if WINDOWS_NATIVE
#include <io.h>
#include <stdio.h>
#else
#include <unistd.h>
#endif
#include <errno.h>

#define BUFSIZE 65535

#if __BORLANDC__
#undef popen
void* __declspec(dllimport) popen(const char*, const char*);

#define popen __popen_tmp
#include <stdio.h>
#undef popen
#define popen _popen
#else
#include <stdio.h>
#undef _popen
#endif

static const char* dt_fmt = "%Y%m%d %H:%M";
static http h;

#include "lib/http.h"

const char* const mediathek_urls[] = {
    "http://"
    "download10.onlinetvrecorder.com/"
    "mediathekview/Filmliste-akt.xz",
    "http://mediathekview.jankal.me/"
    "Filmliste-akt.xz",
    "http://"
    "verteiler1.mediathekview.de/"
    "Filmliste-akt.xz",
    "http://"
    "verteiler2.mediathekview.de/"
    "Filmliste-akt.xz",
    "http://"
    "verteiler3.mediathekview.de/"
    "Filmliste-akt.xz",
    "http://"
    "verteiler4.mediathekview.de/"
    "Filmliste-akt.xz",
    "http://"
    "verteiler5.mediathekview.de/"
    "Filmliste-akt.xz",
    "http://"
    "verteiler6.mediathekview.de/"
    "Filmliste-akt.xz",
};

const char* mediathek_url = "https://"
                            "verteiler1.mediathekview.de/"
                            "Filmliste-akt.xz";
//"http://127.0.0.1/Filmliste-akt.xz";

static unsigned long min_length;
static int debug;
static strlist include, exclude;
static buffer output, *console;

/**
 * @brief count_field_lengths
 * @param sl
 * @param lengths
 */
void
count_field_lengths(strlist* sl, int lengths[21]) {
  int i;
  for(i = 0; i < 21; i++) {
    const char* s = strlist_at(sl, i);
    if(s) {
      if(str_len(s) >= (unsigned)lengths[i])
        lengths[i] = str_len(s);
    }
  }
}

/**
 * @brief split_fields
 * @param sl
 * @param prev
 * @param buf
 * @param n
 * @return
 */
int
split_fields(strlist* sl, strlist* prev, char* buf, size_t n) {

  size_t i;
  size_t offs = byte_finds(buf, n, "[\"");

  if(offs == n)
    return 0;

  buf += offs + 2;
  n -= offs + 2;

  strlist_zero(sl);

  for(i = 0; n; ++i) {
    offs = byte_finds(buf, n, "\",\"");

    if(offs == n)
      offs = byte_finds(buf, n, "\"],");

    if(offs == 0) {
      const char* p = strlist_at(prev, i);
      strlist_push(sl, p ? p : "");
    } else {
      strlist_pushb(sl, buf, offs);
    }

    if(offs == n)
      break;

    buf += offs + 3;
    n -= offs + 3;
  }

  return i;
}

/**
 * @brief process_status
 */
void
process_status(void) {
  /* display interesting process IDs  */
#if !(defined(_WIN32) || defined(_WIN64))
  buffer_putm_internal(console, "process ", errmsg_argv0, ": pid=", NULL);
  buffer_putlong(console, getpid());
  buffer_puts(console, ", ppid=");
  buffer_putlong(console, getppid());
  buffer_puts(console, ", pgrp=");
  buffer_putlong(console, getpgrp());
  buffer_puts(console, ", tcpgrp=");
  buffer_putlong(console, tcgetpgrp(STDIN_FILENO));
  buffer_putnlflush(console);
#endif
}

/**
 * @brief read_mediathek_list  Reads
 * Mediathek list from HTTP server
 * @param url
 * @return
 */
int
read_mediathek_list(const char* url, buffer* b) {
  static buffer in;

  http_init(&h, 0, 0);
  // h.keepalive = 1;

  http_get(&h, url);

  io_onlywantwrite(h.sock);

  buffer_init(&in, (buffer_op_sys*)(void*)&http_read, (fd_t)(size_t)(void*)&h, malloc(8192), 8192);
  in.cookie = &h;
  in.deinit = &buffer_free;
  buffer_lzma(b, &in, 0);

  for(;;) {
    fd_t fd;
    int doread = 0;

    io_wait();

    while((fd = io_canwrite()) != -1) {
      if(h.sock != fd)
        continue;

      if(http_canwrite(&h, &io_onlywantread) == -1) {
        if(errno == EWOULDBLOCK)
          continue;
        errmsg_warnsys("send error: ", 0);
        return 2;
      }
    }

    while((fd = io_canread()) != -1) {
      if(h.sock == fd) {

        if(http_canread(&h, &io_onlywantwrite) == -1) {
          if(errno == EAGAIN)
            continue;
          errmsg_warnsys("send error: ", 0);
          return 2;
        }

        doread = 1;
        return 0;
      }
    }
  }
}

/**
 * @brief parse_time  Parses a time in
 * HH:MM:SS format and returns seconds
 * @param s
 * @return
 */
unsigned long
parse_time(const char* s) {
  size_t r = 0;
  if(s) {
    unsigned short n;
    size_t l;

    if(s == 0)
      return 0;

    for(;;) {
      l = scan_ushort(s, &n);
      if(l == 0)
        break;
      r += n;
      s += l;
      if(*s == ':') {
        r *= 60;
        ++s;
        continue;
      }
      if(*s == '\0')
        break;
    }
  }
  return r;
}

/**
 * @brief format_num
 * @param num
 * @return
 */
char*
format_num(time_t num) {
  static char buf[FMT_LONG];
  byte_zero(buf, sizeof(buf));
  buf[fmt_ulonglong(buf, num)] = '\0';
  return buf;
}

/**
 * @brief format_time
 * @param ti
 * @return
 */
char*
format_time(time_t ti) {
  static char buf[1024];
  unsigned short h, m, s;
  size_t i = 0;

  s = ti % 60;
  ti /= 60;
  m = ti % 60;
  ti /= 60;
  h = ti;

  if(h < 10)
    buf[i++] = '0';
  i += fmt_ulong(&buf[i], h);
  buf[i++] = ':';
  if(m < 10)
    buf[i++] = '0';
  i += fmt_ulong(&buf[i], m);
  buf[i++] = ':';
  if(s < 10)
    buf[i++] = '0';
  i += fmt_ulong(&buf[i], s);
  buf[i] = '\0';

  return buf;
}

void
mktime_r(struct tm* const t, time_t* ret) {
  time_t day;
  time_t i;
  time_t years = t->tm_year - 70;
  if(t->tm_sec > 60) {
    t->tm_min += t->tm_sec / 60;
    t->tm_sec %= 60;
  }
  if(t->tm_min > 60) {
    t->tm_hour += t->tm_min / 60;
    t->tm_min %= 60;
  }
  if(t->tm_hour > 60) {
    t->tm_mday += t->tm_hour / 60;
    t->tm_hour %= 60;
  }
  if(t->tm_mon > 12) {
    t->tm_year += t->tm_mon / 12;
    t->tm_mon %= 12;
  }
  if(t->tm_mon < 0)
    t->tm_mon = 0;
  while(t->tm_mday > __spm[1 + t->tm_mon]) {
    if(t->tm_mon == 1 && isleap(t->tm_year + 1900)) {
      if(t->tm_mon == 31 + 29)
        break;
      --t->tm_mday;
    }
    t->tm_mday -= __spm[t->tm_mon];
    ++t->tm_mon;
    if(t->tm_mon > 11) {
      t->tm_mon = 0;
      ++t->tm_year;
    }
  }
  if(t->tm_year < 70) {
    *ret = -1;
    return;
  }
  day = years * 365 + (years + 1) / 4;
  if((years -= 131) >= 0) {
    years /= 100;
    day -= (years >> 2) * 3 + 1;
    if((years &= 3) == 3)
      years--;
    day -= years;
  }
  day += t->tm_yday = __spm[t->tm_mon] + t->tm_mday - 1 + (isleap(t->tm_year + 1900) & (t->tm_mon > 1));
  i = 7;
  t->tm_wday = (day + 4) % i;
  i = 24;
  day *= i;
  i = 60;
  *ret = ((day + t->tm_hour) * i + t->tm_min) * i + t->tm_sec;
}

/**
 * @brief parse_datetime
 * @param s
 * @param fmt
 * @return
 */
time_t
parse_datetime(const char* s, const char* fmt) {
  struct tm tm_s;
  time_t t;
  byte_zero(&tm_s, sizeof(struct tm));
  if(str_ptime(s, fmt, &tm_s) == s)
    return 0;
  mktime_r(&tm_s, &t);
  return t;
}

/**
 * @brief parse_anydate
 * @param s
 * @return
 */
time_t
parse_anydate(const char* s) {
  const char* fmt;
  if(s) {
    size_t len = str_len(s);
    if(len != 8) /* len - str_rchr(s,
                    '.') == 4) */
      fmt = "%d.%m.%Y";
    else
      fmt = "%Y%m%d";
    return parse_datetime(s, fmt);
  }
  return 0;
}

/**
 * @brief format_datetime
 * @param t
 * @param fmt
 * @return
 */
char*
format_datetime(size_t t, const char* fmt) {
  static char buf[1024];
  time_t tim = t;
  struct tm* tms;

  tms = localtime(&tim);
  if(tms == 0)
    return "(invalid time)";

  /*size_t n =*/strftime(buf, sizeof(buf), fmt, tms);

  return buf; /*  buffer_put(b, buf, n);
               */
}

/**
 * @brief make_url
 * @param base
 * @param trail
 * @return
 */
char*
make_url(const char* base, const char* trail) {
  static stralloc url;

  unsigned int n = 0;
  size_t i;

  if(trail == 0)
    return 0;
  i = scan_uint(trail, &n);
  if(trail[i] != '|')
    return 0;
  stralloc_copyb(&url, base, n);

  stralloc_cats(&url, &trail[++i]);
  stralloc_0(&url);
  return url.s;
}

typedef struct mediathek_entry {
  stralloc channel;
  stralloc topic;
  stralloc title;
  time_t tm;
  time_t dr;
  unsigned int mbytes;
  stralloc desc;
  stralloc url;
  stralloc link;
} mediathek_entry_t;

/**
 * @brief new_mediathek_entry
 * @return
 */
mediathek_entry_t*
new_mediathek_entry() {
  mediathek_entry_t* e = alloc_zero(sizeof(mediathek_entry_t));
  /*  if(e == 0)
      return 0;
    byte_zero(e,
    sizeof(mediathek_entry_t));*/
  return e;
}

/**
 * @brief delete_mediathek_entry
 * @param e
 */
void
delete_mediathek_entry(mediathek_entry_t* e) {
  if(e->channel.s)
    stralloc_free(&e->channel);
  if(e->topic.s)
    stralloc_free(&e->topic);
  if(e->title.s)
    stralloc_free(&e->title);

  stralloc_free(&e->desc);
  stralloc_free(&e->url);
  stralloc_free(&e->link);
  alloc_free(e);
}

/**
 * @brief match_tokens
 * @param toks
 * @param str
 * @return 1 if all tokens match
 */
int
match_tokens(char* toks, const char* x, size_t n) {
  size_t tlen = str_len(toks), i;
  int ret = 1;
  const char* s;

  str_foreach_skip(toks, s, i) {
    i = scan_noncharsetnskip(s, "/+,|*;", tlen);

    if(i > 0 && case_findb(x, n, s, i) == n) {
      ret = 0;
      break;
    }

    i += scan_charsetnskip(&s[i], "/+,|*;", tlen - i);
    tlen -= i;
  }

  if(ret && debug > 1) {
    buffer_putm_internal(console, "token list '", toks, "' matched '", NULL);
    buffer_put(console, x, n);
    buffer_puts(console, "'.");
    buffer_putnlflush(console);
  }

  return ret;
}

/**
 * @brief match_toklists  returns 1 if
 * any of the token lists match
 * @param sl
 * @return
 */
int
match_toklists(strlist* sl) {
  char* s;
  int ret = 0;

  if(include.sa.s == 0)
    ret = 1;

  strlist_foreach_s(&include, s) {
    if(match_tokens(s, sl->sa.s, sl->sa.len)) {
      ret = 1;
      break;
    }
  }

  if(ret == 0)
    return 0;

  strlist_foreach_s(&exclude, s) {
    if(match_tokens(s, sl->sa.s, sl->sa.len)) {
      ret = 0;
      break;
    }
  }

  return ret;
}

/**
 * @brief parse_entry
 * @param sl
 * @return
 */
mediathek_entry_t*
parse_entry(strlist* sl) {

  mediathek_entry_t* e = 0;
  time_t dt = parse_anydate(strlist_at(sl, 3));

  time_t tm = parse_time(strlist_at(sl, 4));
  time_t dr = parse_time(strlist_at(sl, 5)); /* duration */

  if((unsigned)dr < min_length)
    return 0;
  {
    unsigned int mbytes = 0;
    const char* mb = strlist_at(sl, 6);
    if(mb)
      scan_uint(mb, &mbytes);

    {
      const char* desc = strlist_at(sl, 7);
      const char* url = strlist_at(sl, 8);
      const char* link = strlist_at(sl, 9);

      if(!desc || !url)
        return 0;

      if(!e)
        e = new_mediathek_entry();

      if(e) {
        stralloc_copys(&e->channel, strlist_at(sl, 0));
        stralloc_0(&e->channel);
        stralloc_copys(&e->topic, strlist_at(sl, 1));
        stralloc_0(&e->topic);
        stralloc_copys(&e->title, strlist_at(sl, 2));
        stralloc_0(&e->title);

        stralloc_copys(&e->desc, desc ? desc : "");
        stralloc_0(&e->desc);
        stralloc_copys(&e->url, url ? url : "");
        stralloc_0(&e->url);
        stralloc_copys(&e->link, link ? link : "");
        stralloc_0(&e->link);

        e->tm = dt + tm;
        e->dr = dr;
        e->mbytes = mbytes;
      }
      return e;
    }
  }
  return 0;
}

/**
 * @brief print_entry
 * @param b
 * @param e
 */
void
print_entry(buffer* b, const mediathek_entry_t* e) {

  const char* sep = ", ";

  buffer_putm_internal(b,
                       "Kanal:\t",
                       e->channel.s ? e->channel.s : "<null>" /*strlist_at(sl,
                                                                 1)*/
                       ,
                       sep,
                       NULL);
  buffer_putm_internal(b,
                       "Thema:\t",
                       e->topic.s ? e->topic.s : "<null>" /*strlist_at(sl,
                                                             2)*/
                       ,
                       sep,
                       NULL);
  buffer_putm_internal(b, "Titel:\t", e->title.s /*strlist_at(sl, 3)*/, sep, NULL);

  buffer_putm_internal(b, "Datum:\t", format_datetime(e->tm, dt_fmt), sep, NULL);
  buffer_putm_internal(b, "Dauer:\t", format_time(e->dr), sep, NULL);
  buffer_putm_internal(b, "Grösse:\t", format_num(e->mbytes), "MB", sep, NULL);

  /* buffer_putm_internal(b, "URL:\t",
   url , sep, NULL);
   buffer_putm_internal(b, "URL lo:\t",
   make_url(url, strlist_at(sl, 13)),
   sep, NULL);
   buffer_putm_internal(b, "URL hi:\t",
   make_url(url, strlist_at(sl, 15)),
   sep, NULL);*/

  buffer_putnlflush(b);
}

/**
 * @brief output_entry
 * @param b
 * @param sl
 */
void
output_entry(buffer* b, strlist* sl) {

  size_t i, n = strlist_count(sl);

  buffer_puts(b, "\"X\":[");

  for(i = 0; i < n; ++i) {
    char c;
    const char* s = strlist_at(sl, i);
    buffer_put(b, " \"", 2);

    if(s == NULL)
      s = "";

    while((c = *s++)) {
      if(c == '"')
        buffer_putc(b, '\\');
      buffer_PUTC(b, c);
    }

    buffer_puts(b, ((i + 1 < n) ? "\"," : "\" ]"));
  }
}

/**
 * @brief parse_mediathek_list
 * @param inbuf
 * @return
 */
int
parse_mediathek_list(buffer* inbuf, buffer* outbuf) {
  int count = 0;
  char buf2[BUFSIZE];
  static strlist prev, prevout, sl;
  size_t matched = 0, total = 0;
  ssize_t ret, ret2, read_bytes = 0;
  mediathek_entry_t* e = 0;

  /*  strlist_init(&sl, '\0');
    strlist_init(&prev, '\0');
    strlist_init(&prevout, '\0');
  */

  if((ret = buffer_get_token(inbuf, buf2, sizeof(buf2), "]", 1)) > 0) {
    read_bytes += ret;

    for(;;) {
      ret2 = 0;
      if(ret + 1 >= BUFSIZE)
        break;
      // buf2[ret++] = ']';
      //  ++ret;
      ret2 = buffer_get(inbuf, &buf2[ret], 1);
      if(ret2 > 0) {

        if(ret > 1 && buf2[ret - 2] == '"' && buf2[ret] == ',')
          break;

        ret += ret2;
        ret2 = buffer_get_token(inbuf, &buf2[ret], sizeof(buf2) - ret, "]", 1);
        if(ret2 > 0)
          ret += ret2;
      }
    }

    strlist_zero(&sl);
    split_fields(&sl, &prev, buf2, ret + ret2);

    if((e = parse_entry(&sl))) {
      total++;
      if(debug > 2)
        print_entry(console, e);

      if(match_toklists(&sl)) {
        matched++;

        if(strlist_count(&prevout)) {
          buffer_put(outbuf, ",\n", 2);
          buffer_flush(outbuf);
        }

        output_entry(outbuf, &sl);
        strlist_copy(&prevout, &sl);
        ++count;
      }
      delete_mediathek_entry(e);
      e = NULL;
    }

    strlist_copy(&prev, &sl);
  }

#ifdef DEBUG_OUTPUT_
  buffer_puts(console, "Read ");
  buffer_putlong(console, read_bytes);
  buffer_putsflush(console, " bytes.\n");
#endif

  if(h.response->err) {
    if(h.response->err != EAGAIN) {
      buffer_puts(console, "Return value: ");
      buffer_putlong(console, ret);
      buffer_puts(console, " ");
      buffer_flush(console);
      errno = h.response->err;
      errmsg_warnsys("Read error", 0);
    }
  }

  if(ret == 0 && h.response->err != EAGAIN) {
    char status[FMT_ULONG + 1], error[1024];
    status[fmt_ulong(status, h.response->status)] = '\0';
    http_strerror(&h, ret);
    errmsg_warn("STATUS: ", status, " error: ", error, " connected: ", h.connected ? "1" : "0", 0);
  }

  if(ret == 0) {

    if(debug) {
      buffer_puts(console, "\nprocessed ");
      buffer_putulong(console, matched);
      buffer_puts(console, "/");
      buffer_putulong(console, total);
      buffer_puts(console, " entries.");
      buffer_putnlflush(console);
    }

    strlist_free(&sl);
    strlist_free(&prev);
    strlist_free(&prevout);
  }

  return count;
}

/**
 * @brief usage
 * @param errmsg_argv0
 */
void
usage(char* errmsg_argv0) {
  buffer_putm_internal(buffer_1,
                       "Usage: ",
                       str_basename(errmsg_argv0),
                       "[OPTIONS] [KEYWORDS...]\n",
                       "\n",
                       "Options\n",
                       "  -h, --help                "
                       "show this help\n",
                       "  -u, --url=URL             set "
                       "URL\n",
                       "  -F                        "
                       "date/time format\n",
                       "  -t HH:MM:SS               "
                       "minimum length\n",
                       "  -i KEYWORD                "
                       "include entries matching\n",
                       "  -x KEYWORD                "
                       "exclude entries matching\n",
                       "  -o FILE                   "
                       "output file\n",
                       "\n",
                       0);
  buffer_putnlflush(buffer_1);
}

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int
main(int argc, char* argv[]) {

  int opt, ret;
  static buffer in;
  static const char* outfile;

  min_length = 0;

  errmsg_iam(argv[0]);

  while((opt = unix_getopt(argc, argv, "u:F:dt:i:x:ho:")) != -1) {
    switch(opt) {
      case 'h': usage(argv[0]); return 0;
      case 'F': dt_fmt = unix_optarg; break;
      case 'u': mediathek_url = unix_optarg; break;
      case 'd': debug++; break;
      case 't': min_length = parse_time(unix_optarg); break;
      case 'i': strlist_push(&include, unix_optarg); break;
      case 'x': strlist_push(&exclude, unix_optarg); break;
      case 'o': outfile = unix_optarg; break;
      default: usage(argv[0]); return EXIT_FAILURE;
    }
  }

  while(unix_optind < argc) {

    strlist_push(&include, argv[unix_optind++]);
  }

  if(outfile) {
    buffer_truncfile(&output, outfile);
    console = buffer_1;
  } else {
    buffer_write_fd(&output, STDOUT_FILENO);
    console = buffer_2;
  }

  /* if(strlist_count(&include) == 0)
     strlist_push(&include, "");*/

  strlist_dump(console, &include);
  strlist_dump(console, &exclude);

  /*  stralloc sa;
    stralloc_init(&sa);

    strlist_join(&include, &sa, ',');

    buffer_putsa(console, &sa);
    buffer_putnlflush(console);*/
  /*
    fprintf(stderr, "%p\n",
    str_istr("blah", ""));
    fprintf(stderr, "%p\n",
    str_istr("[", "blah"));
    fflush(stderr);
    */

  /*   if(unix_optind >= argc) {
         fprintf(stderr,
                 "Nach den Optionen
     wurde ein Argument erwartet\n");
         exit(EXIT_FAILURE);
     }
  */

  errmsg_argv0 = argv[0];

  if(errmsg_argv0[str_rchr(errmsg_argv0, '/')] != '\0')
    errmsg_argv0 += str_rchr(errmsg_argv0, '/') + 1;

  ret = read_mediathek_list(mediathek_url, &in);
  if(!ret) {
    int n = 0;
    fd_t fd;

    buffer_puts(&output, "{\n");

    for(;;) {
      io_waituntil2(1000);

      while((fd = io_canwrite()) != -1) {
        if(fd == h.sock)
          http_canwrite(&h, &io_onlywantread);
      }

      while((fd = io_canread()) != -1) {
        if(fd == h.sock) {
          if(!h.sent)
            http_canread(&h, &io_onlywantwrite);
          else
            n += parse_mediathek_list(&in, &output);
        }
      }
    }

    buffer_puts(&output, "\n}");
    buffer_flush(&output);

    buffer_puts(console, "Processed ");
    buffer_putlong(console, n);
    buffer_putsflush(console, " entries.\n");
  } else {
    errmsg_warnsys("Read failed", 0);
    buffer_puts(console, "Read = ");
    buffer_putlong(console, ret);
    buffer_putnlflush(console);
  }
  return 1;
}
