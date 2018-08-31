#include "lib/io_internal.h"
#include "lib/array.h"
#include "lib/buffer.h"
#include "lib/byte.h"
#include "lib/open.h"
#include "lib/str.h"
#include "lib/stralloc.h"
#include "lib/strlist.h"
#include "lib/getopt.h"

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#if (defined(_WIN32) || defined(_WIN64)) && !defined(__MSYS__)
#include <io.h>
#else
#include <unistd.h>
#endif

static int lowq = 0, debug = 0;
static const char* datetime_format = "%d.%m.%Y %H:%M:%S";
static bool csv = false;

char* str_ptime(const char* s, const char* format, struct tm* tm);

void output_entry(const char* sender, const char* thema, const char* title, unsigned duration, const char* datetime, const char* url, const char* description);
/*
int
parse_predicate(const char* x, size_t len)
{

  while(isspace(*x) && len > 0) {
    ++x;
    --len;
  }>
#include <stdlib.h>
  if(len == 0)
    return 0;

  size_t pos = byte_chr(x, '\n', len);

  if(pos != len && pos > 0)
    return 1;

  buffer_puts(buffer_1, "Predicate: ");
  buffer_put(buffer_1, x, len);

  buffer_puts(buffer_1, " (");
  buffer_putulong(buffer_1, len);
  buffer_putsflush(buffer_1, ")\n");

  if(len > 0 && x[len - 1] == ' ')
    return 1;

  return 0;
}
*/

int
read_line(const char* s, size_t len, strlist* fields, array* x) {
  int64 pos = 0;

  (void)fields;

  const char *end = s + len, *p = s;
  int quoted = 0 /*, escaped = 0*/;
  size_t n, i = 0;
  char tokbuf[65536];

  array_trunc(x);

  if((n = byte_chr(p, end - p, '\n')) != (unsigned)(end - p)) end = p + n;

  while(p < end && *p != '"') ++p;

  for(; p < end; ++p /*, escaped = 0*/) {
    if(*p == '\\') {
      /* escaped = 1;*/
      ++p;
    }

    else if(*p == '"') {
      if(!quoted) {
        quoted = 1;
        i = 0;
        continue;
      } else {
        quoted = 0;
        char** a = array_allocate(x, sizeof(char*), pos++);
        tokbuf[i] = '\0';
        *a++ = str_dup(tokbuf);

        i = 0;
        continue;
        /*while(++p < end && *p != '"')
          ;
        if(*++p == ',')
          ++p;*/
      }
    }
    tokbuf[i++] = *p;
  }

  return p - s;
}

void
strarray_dump(buffer* b, const array* a) {
  char** av = array_start(a);
  ssize_t ac = array_length(a, sizeof(char*));
  buffer_puts(b, "[ \"");
  while(ac--) {
    buffer_puts(b, *av);
    if(ac > 0) buffer_puts(b, "\", \"");
    ++av;
  }
  buffer_putsflush(b, "\" ]\n");
}

void
dump_pair(buffer* b, const char* first, const char* second) {
  buffer_puts(b, first);
  buffer_puts(b, ": ");
  buffer_puts(b, second);
  buffer_putnlflush(b);
}
void
dump_long(buffer* b, const char* name, long value) {
  buffer_puts(b, name);
  buffer_puts(b, ": ");
  buffer_putlong(b, value);
  buffer_putnlflush(b);
}

void
get_domain(const char* url, stralloc* d) {
  while(*url && *url != ':') ++url;
  ++url;
  while(*url && *url == '/') ++url;
  stralloc_copyb(d, url, str_chr(url, '/'));
}

#define isdelim(c) (c == ' ' || c == '\t' || c == '\n' || c == '-' || c == ';' || c == ',')

void
cleanup_text(char* t) {
  int i;
  char c;
  char prev = 'x';
  stralloc out;
  stralloc_init(&out);

  for(i = 0; (c = t[i]); ++i) {

    if(isdelim(c) && isdelim(prev)) continue;

    if(isdelim(c)) c = ' ';
    stralloc_append(&out, &c);
    prev = c;
  }
  byte_copy(t, out.len, out.s);
  t[out.len] = '\0';
  stralloc_free(&out);
}

char*
cleanup_domain(stralloc* d) {
  size_t i;
  d->len = byte_rchr(d->s, d->len, '.');
  for(i = 0; i < d->len; ++i) d->s[i] = toupper(d->s[i]);
  stralloc_nul(d);
  const char* remove_parts[] = {"ondemand", "storage", "files", "stream", "mvideos", "online", 0};

  for(i = 0; remove_parts[i]; ++i) {
    char *s2, *s = strtok(d->s, remove_parts[i]);
    if(s && (s2 = strchr(s, '.')) && s2 > s) {
      ++s2;
      s = str_dup(s2);
      free(d->s);
      d->s = s;
    }
  }
  return d->s;
}

void
process_entry(const array* a) {

  char** av = array_start(a);
  size_t ac = array_length(a, sizeof(char*));

  if(ac >= 21 && !str_diff(av[0], "X")) {
    stralloc datetime;
    struct tm tm;
    time_t t;
    unsigned d;

    char *sender = av[1], *thema = av[2], *title = av[3] /*, *date = av[4], *time = av[5]*/, *duration = av[6], /**grcoee = av[7],*/ *description = av[8],
         *url = av[9] /*, *website = av[10], *untertitel = av[11], *urlrtmp = av[12]*/,
         *url_klein =
           av[13] /*, *urlrtmp_klein = av[14], *url_hd = av[15], *urlrtmp_hd = av[16], *datuml = av[17], *url_history = av[18], *geo = av[19], *neu = av[20]*/;

    /*    char* title = av[8];
        char* date = av[4];
        char* time = av[5];
        char* duration = av[6];
        char* url = av[9];
      */
    stralloc url_lo;
    stralloc_init(&url_lo);

    if(str_len(url_klein)) {
      char* endptr = url_klein;
      unsigned long pos_lo = strtoul(url_klein, &endptr, 10);
      //*endptr++ = '\0';
      endptr++;
      stralloc_copyb(&url_lo, url, pos_lo);
      stralloc_catb(&url_lo, endptr, str_len(endptr) + 1);
    } else {
      stralloc_copys(&url_lo, url);
    }

    stralloc_init(&datetime);
    stralloc_copys(&datetime, av[4]);
    stralloc_catc(&datetime, ' ');
    stralloc_cats(&datetime, av[5]);
    stralloc_nul(&datetime);

    if(str_ptime(datetime.s, datetime_format, &tm) == NULL) {
      t = 0;
    } else {
      t = mktime(&tm);
    }

    {
      int h = 0, m = 0, s = 0;
      if(sscanf(duration, "%d:%d:%d", &h, &m, &s) >= 2) {
        d = h * 3600 + m * 60 + s;
      } else {
        d = 0;
      }
    }

    if(str_len(thema) == 0) return;
    if(d < 20 * 60) return;

    cleanup_text(thema);
    cleanup_text(title);
    cleanup_text(description);

    if(str_len(sender) == 0) {

      stralloc s;
      stralloc_init(&s);
      get_domain(url_lo.s, &s);
      free(sender);
      sender = cleanup_domain(&s);
    }

    /*dump_pair(buffer_2, "sender", sender);
    dump_pair(buffer_2, "thema", thema);
    dump_pair(buffer_2, "title", title);*/

    /*    dump_long(buffer_2, "d", d);
        dump_pair(buffer_2, "duration", duration);
        dump_pair(buffer_2, "url", url);
        dump_pair(buffer_2, "url_lo.n", av[13]);
        dump_pair(buffer_2, "url_lo.s", url_lo.s);
    */
    char timebuf[256];

    strftime(timebuf, sizeof(timebuf), "%Y%m%d %H:%M", &tm);

    output_entry(sender, thema, title, d, timebuf, lowq > 0 ? url_lo.s : url, description);

    (void)t;
  } else {
    strarray_dump(buffer_2, a);
  }

  while(ac > 0) {
    --ac;
    if(av[ac]) free(av[ac]);
  }
}

static void
put_quoted_string(const char* str) {
  buffer_putc(buffer_1, '"');
  while(*str) {
    char c = *str++;
    if(c == '"' || c == '\\') {
      buffer_puts(buffer_1, c == '\\' ? "\\\\" : "\"\"");
    } else {
      buffer_putc(buffer_1, c);
    }
  }
  buffer_putc(buffer_1, '"');
}

void
output_entry(const char* sender, const char* thema, const char* title, unsigned duration, const char* datetime, const char* url, const char* description) {

  if(csv == false) {
    buffer_puts(buffer_1, "#EXTINF:");
    buffer_putulong(buffer_1, duration);
    buffer_put(buffer_1, ",|", 2);
    buffer_put(buffer_1, datetime, str_len(datetime));
    buffer_puts(buffer_1, "|");
    buffer_puts(buffer_1, sender);
    buffer_puts(buffer_1, "|");
    buffer_puts(buffer_1, thema);
    buffer_puts(buffer_1, "|");
    buffer_puts(buffer_1, title);
    buffer_puts(buffer_1, "|");
    buffer_puts(buffer_1, description);
    buffer_put(buffer_1, "\r\n", 2);
    buffer_puts(buffer_1, "#EXTVLCOPT:network-caching=2500\r\n");
    buffer_puts(buffer_1, url);
  } else {
    put_quoted_string(sender);
    buffer_puts(buffer_1, ",");
    put_quoted_string(thema);
    buffer_puts(buffer_1, ",");
    put_quoted_string(title);
    buffer_puts(buffer_1, ",");
    buffer_put(buffer_1, datetime, str_len(datetime));
    buffer_puts(buffer_1, ",");
    buffer_putulong(buffer_1, duration);
    buffer_puts(buffer_1, ",");
    put_quoted_string(description);
    buffer_puts(buffer_1, ",");
    put_quoted_string(url);
  }
  buffer_put(buffer_1, "\r\n", 2);
  buffer_flush(buffer_1);
}

int
process_input(buffer* input) {
  int ret = -1;
  size_t line = 0 /*, index = 0*/;
  stralloc sa;
  static array arr;
  strlist fields;
  stralloc_init(&sa);
  strlist_init(&fields);

  if(csv == false) buffer_puts(buffer_1, "#EXTM3U\r\n");

  for(stralloc_init(&sa); buffer_getline_sa(input, &sa); stralloc_zero(&sa)) {
    ++line;

    strlist_init(&fields);
    array_trunc(&arr);

    ret = read_line(sa.s, sa.len, &fields, &arr);

    /*        buffer_puts(buffer_2, "Line ");
            buffer_putulong(buffer_2, line);
            buffer_puts(buffer_2, ": ");
            buffer_putsa(buffer_2, &sa);
            buffer_putnlflush(buffer_2);
    */

    process_entry(&arr);
  }

  buffer_close(input);
  return ret;
}

int
main(int argc, char* argv[]) {

  int opt, index = 0;

  char inbuf[8192];
  buffer b;

  struct option opts[] = {
    {"csv", 0, NULL, 'c'},
    {"debug", 0, NULL, 'd'},
    {"low", 0, NULL, 'l'},
    {"format", 1, NULL, 'F'},
  };

  while((opt = getopt_long(argc, argv, "cdf:t:i:x:l", opts, &index)) != -1) {
    switch(opt) {
      case 'c': csv = true; break;
      case 'd': debug++; break;
      case 'l': lowq++; break;
      case 'f': datetime_format = optarg; break;
      default: /* '?' */ buffer_putm(buffer_2, "Usage: ", argv[0], "[-d] [-l] <file>\n"); exit(EXIT_FAILURE);
    }
  }

  if(optind == argc) {
    ++argc;
    argv[optind] = "-";
  }

  while(optind < argc) {

    if(str_diff(argv[optind], "-")) {
      buffer_puts(buffer_2, "Processing '");
      buffer_puts(buffer_2, argv[optind]);
      buffer_puts(buffer_2, "' ... ");

      if(buffer_mmapread(&b, argv[optind])) {
        buffer_puts(buffer_2, "failed");
        buffer_putnlflush(buffer_2);
        return 1;
      } else {
        buffer_putnlflush(buffer_2);
      }
    } else {
      buffer_init(&b, (buffer_op_sys*)&read, STDIN_FILENO, inbuf, sizeof(inbuf));
    }
    process_input(&b);
    ++optind;
  }

  return 0;
}
