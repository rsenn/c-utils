
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <signal.h>
#include "buffer.h"
#include "strlist.h"
#include "str.h"
#include "scan.h"
#include "fmt.h"

#define BUFSIZE 65535

extern char strlist_dumpx[5];

const char* argv0;

const char* const mediathek_url = "http://verteiler1.mediathekview.de/Filmliste-akt.xz";

static ssize_t
buffer_dummyread(int fd, char* b, size_t n) {
  return 0;
}

void
count_field_lengths(strlist* sl, int lengths[21]) {
  int i;
  for(i = 0; i < 21; i++) {
    const char* s = strlist_at(sl, i);
    if(s) {
      if(str_len(s) >= lengths[i])
        lengths[i] = str_len(s);
    }
  }
}

int
split_fields(strlist* sl, strlist* prev, char* buf, size_t n) {
  int ret = 0;
  char buf2[4096];
  char *p = buf;
  char *end = buf + n;

  strlist_zero(sl);

  for(;;) {
    size_t n = 0;
    while(p < end && *p != '"')
      ++p;

    if(p == end) break;

    ++p;

    while(p < end) {

      if(*p == '\\' && p[1] == '"')	{
        ++p;
      } else {
        if(*p == '"') {
          ++p;
          break;
        }
      }

      buf2[n++] = *p++;
    }

    if(ret >= 1 && ret <= 2 && n == 0) {
      strlist_push(sl, strlist_at(prev, ret));
    } else {
      strlist_pushb(sl, buf2, n);
    }
    ret++;
  }

  return ret;
}

void process_status(void) {
  /* display interesting process IDs  */
  fprintf(stderr, "process %s: pid=%d, ppid=%d, pgid=%d, fg pgid=%dn\n",
          argv0, (int)getpid(), (int)getppid(),
          (int)getpgrp(), (int)tcgetpgrp(STDIN_FILENO));
}

int
read_mediathek_list(const char* url) {
  /*  int status;
    int xzpid;
    int xzpipe[2];
    int clpipe[2];
    int clpid;

    process_status();

    if(pipe(xzpipe) != 0) return -1;
    if(pipe(clpipe) != 0) return -1;

    if((xzpid = fork()) == 0) {

      close(STDOUT_FILENO);
      dup(xzpipe[1]);
      close(STDIN_FILENO);
      dup(clpipe[0]);

      execlp("xzcat", "xzcat", NULL);
      exit(1);
    }

    if((clpid = fork()) == 0) {

      close(STDOUT_FILENO);
      dup(clpipe[1]);

      execlp("curl", "curl", "-s", url, NULL);
      exit(1);
    }

    return xzpipe[0];*/

  stralloc cmd;
  stralloc_init(&cmd);
  stralloc_copys(&cmd, "curl -s ");
  stralloc_cats(&cmd, url);
  stralloc_cats(&cmd, "| xzcat");
  stralloc_0(&cmd);

  static FILE *pfd;

  if((pfd  = popen(cmd.s, "r")) == NULL) return -1;

  return fileno(pfd);
}


unsigned long
parse_time(const char* s) {
  size_t r = 0;
  unsigned short n;
  size_t l;

  if(s == NULL) return 0;

  for(;;) {
    l = scan_ushort(s, &n);
    if(l == 0) break;
    r += n;
    s += l;
    if(*s == ':') {
      r *= 60;
      ++s;
      continue;
    }
    if(*s == '\0') break;
  }
  return r;
}

char*
format_num(time_t num) {
  static char buf[1024];
  memset(buf, 0,  sizeof(buf));
  buf[fmt_ulonglong(buf, num)] = '\0';
  return buf;
}

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

  if(h < 10) buf[i++] = '0';
  i += fmt_ulong(&buf[i], h);
  buf[i++] = ':';
  if(m < 10) buf[i++] = '0';
  i += fmt_ulong(&buf[i], m);
  buf[i++] = ':';
  if(s < 10) buf[i++] = '0';
  i += fmt_ulong(&buf[i], s);
  buf[i] = '\0';

  return buf;
}

time_t
parse_datetime(const char* s, const char* fmt) {
  struct tm tm_s;
  memset(&tm_s, 0, sizeof(struct tm));
  if(strptime(s, fmt, &tm_s) == s)
    return 0;
  return mktime(&tm_s);
}

char*
format_datetime(size_t t, const char* fmt) {
  static char buf[1024];
  time_t tm = t;
  size_t n = strftime(buf, sizeof(buf), fmt, localtime(&tm));

  return buf; //  buffer_put(b, buf, n);
}

char*
make_url(const char* base, const char* trail) {
  static stralloc url;

  size_t n = 0;
  size_t i;

  if(trail == NULL) return NULL;
  i = scan_uint(trail, &n);
  if(trail[i] != '|') return NULL;
  stralloc_copyb(&url, base, n);

  stralloc_cats(&url, &trail[++i]);
  stralloc_0(&url);
  return url.s;
}

int
parse_entry(buffer* b, strlist* sl) {

  const char* sep = ", ";
  time_t dt = parse_datetime(strlist_at(sl, 4), "%d.%m.%Y");

  time_t tm = parse_time(strlist_at(sl, 5));
  time_t dr = parse_time(strlist_at(sl, 6));  /* duration */

  size_t mbytes = 0;
  const char *mb = strlist_at(sl, 7);
  if(mb) scan_uint(mb, &mbytes);

  const char* desc = strlist_at(sl, 8);
  const char* url = strlist_at(sl, 9);
  const char* link = strlist_at(sl, 10);

  buffer_putm(b, "Kanal:\t", strlist_at(sl, 1), sep, NULL);
  buffer_putm(b, "Thema:\t", strlist_at(sl, 2), sep, NULL);
  buffer_putm(b, "Titel:\t", strlist_at(sl, 3), sep, NULL);

  buffer_putm(b, "Datum:\t", format_datetime(dt + tm, "%d.%m.%Y %H:%M:%S"), sep, NULL);
  buffer_putm(b, "Dauer:\t", format_time(dr), sep, NULL);
  buffer_putm(b, "Grösse:\t", format_num(mbytes), "MB", sep, NULL);

  buffer_putm(b, "URL:\t", url , sep, NULL);
  buffer_putm(b, "URL lo:\t", make_url(url, strlist_at(sl, 13)), sep, NULL);
  buffer_putm(b, "URL hi:\t", make_url(url, strlist_at(sl, 15)), sep, NULL);

  buffer_putnlflush(b);

  return 0;
}

void
output_entry(buffer* b, strlist* sl) {

  size_t i, n = strlist_count(sl);

  buffer_put(b, " ", 1);

  for(i = 0; i < n; ++i) {
    char c;
    const char* s = strlist_at(sl, i);
    buffer_put(b, " \"", 2);

    while((c = *s++)) {
      if(c == '"') {
        c = '\\';
        buffer_PUTC(b, c);
        c = '"';
      }
      buffer_PUTC(b, c);

    }

    buffer_puts(b, (i == 0 ? "\" : [" : ((i + 1 < n) ? "\"," : "\" ]")));
  }
}

int
parse_mediathek_list(int fd) {
  char buf[1024];
  static char buf2[BUFSIZE];
  ssize_t ret, ret2;
  strlist prev, sl;
  buffer b = BUFFER_INIT(read, fd, buf, sizeof(buf));

  strlist_init(&prev);

  buffer_put(buffer_1, "{\n", 2);

  while((ret = buffer_get_token(&b, buf2, sizeof(buf2), "]", 1)) > 0) {

    for(;;) {
      if(ret + 1 >= BUFSIZE) break;
      buf2[ret++] = ']';
      ret2 = buffer_get(&b, &buf2[ret], 1);
      if(ret2 > 0) {

        if(buf2[ret] == ',') break;

        ret += ret2;
        ret2 = buffer_get_token(&b, &buf2[ret], sizeof(buf2) - ret, "]", 1);
        if(ret2 > 0)
          ret += ret2;
      }
    }

    strlist_init(&sl);
    split_fields(&sl, &prev, buf2, ret);

//    strlist_dump(buffer_2, &sl);
    parse_entry(buffer_2, &sl);

    if(strlist_count(&prev)) buffer_put(buffer_1, ",\n", 2);
    output_entry(buffer_1, &sl);

    prev = sl;
  }

  buffer_flush(buffer_1);
}

int main(int argc, char *argv[]) {
  strlist_dumpx[1] = '\n';
  strlist_dumpx[2] = '\t';


  argv0 = basename(argv[0]);
  return parse_mediathek_list(read_mediathek_list(mediathek_url));
}
