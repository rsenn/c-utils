#include "lib/unix.h"
#include "lib/array.h"
#include "lib/buffer.h"
#include "lib/stralloc.h"
#include "lib/byte.h"
#include "lib/io.h"
#include "lib/iarray.h"
#include "lib/open.h"
#include "lib/str.h"
#include "lib/strarray.h"
#include "lib/strlist.h"
#include "lib/fmt.h"
#include "lib/mmap.h"
#include "lib/ucs.h"
#include "lib/case.h"
#include "lib/bool.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if WINDOWS_NATIVE
#include <io.h>
#else
#include <unistd.h>
#endif

#define isdelim(c) (c == ' ' || c == '\t' || c == '\n' || c == '-' || c == ';' || c == ',')
typedef enum format { M3U = 0, WGET, CURL } format_t;
static int lowq = 0, debug = 0;
static const char* datetime_format = "%d.%m.%Y %H:%M:%S";
static int csv = 0;
static format_t output_format = M3U;
static buffer output_buf;
static const char* prefix_cmd;

char* str_ptime(const char* s, const char* format, struct tm* tm);

typedef void output_fn(const char* sender, const char* thema, const char* title, unsigned duration, const char* datetime, const char* url, const char* description);
typedef output_fn* output_fn_ptr;

output_fn output_m3u_entry, output_wget_entry, output_curl_entry;

const output_fn_ptr output_handlers[3] = {&output_m3u_entry, &output_wget_entry, &output_curl_entry};

/**
 * @brief read_line
 * @param s
 * @param len
 * @param fields
 * @param x
 * @return
 */
int
read_line(char* s, size_t len, strlist* fields) {
  char *end = s + len, *p = s;
  int64 pos = 0;
  int quoted = 0;
  size_t n, i = 0;
  char tokbuf[65536];
  bool comma_space;

  (void)fields;

  if((n = byte_finds(p + 1, end - p - 1, "\"X\":[")) != (unsigned)(end - p))
    end = p + 1 + n;

  comma_space = byte_finds(p, n, "\", \"") < n;

  while(p < end && *p != '"')
    ++p;

  if(str_start(p, "\"X\":[")) {
    p[2] = ':';
    p[3] = '"';
    p[4] = ',';
  }

  strlist_zero(fields);
  strlist_fromb(fields, p, end - p, comma_space ? "\", \"" : "\",\"");

  for(; p < end; ++p) {
    if(*p == '\\') {
      ++p;
    } else if(*p == '"') {
      if(!quoted) {
        quoted = 1;
        i = 0;
        continue;
      } else {
        quoted = 0;
        tokbuf[i] = '\0';
        i = 0;
        continue;
      }
    }
    tokbuf[i++] = *p;
  }
  return p - s;
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

/**
 * @brief get_domain
 * @param url
 * @param d
 */
void
get_domain(const char* url, stralloc* d) {
  while(*url && *url != ':')
    ++url;
  ++url;

  while(*url && *url == '/')
    ++url;
  stralloc_copyb(d, url, str_chr(url, '/'));
}

/**
 * @brief cleanup_text
 * @param t
 */
void
cleanup_text(char** t) {
  size_t i;
  size_t len = str_len(*t);
  char c, prev = 'x';
  stralloc out;
  stralloc_init(&out);

  for(i = 0; i < len; ++i) {
    c = (*t)[i];

    if(isdelim(c) && isdelim(prev))
      continue;

    if(isdelim(c))
      c = ' ';
    stralloc_append(&out, &c);
    prev = c;
  }
  stralloc_replaces(&out, "/", " - ");
  stralloc_replaces(&out, ": ", " - ");
  stralloc_replaces(&out, " - ...", "...");

  while(out.len > 0 && !isalnum(out.s[out.len - 1]))
    out.len--;

  stralloc_nul(&out);

  free(*t);
  *t = out.s;

  /*  byte_copy(t, len < out.len ? len :
    out.len, out.s);

    if(out.len < len)
      t[out.len] = '\0';
  */  // str_utf8_latin1(t);
  //  stralloc_free(&out);
}

/**
 * @brief cleanup_domain
 * @param d
 * @return
 */
char*
cleanup_domain(stralloc* d) {
  size_t i;
  const char* remove_parts[] = {"ondemand", "storage", "files", "stream", "mvideos", "online", 0};
  d->len = byte_rchr(d->s, d->len, '.');

  for(i = 0; i < d->len; ++i)
    d->s[i] = toupper(d->s[i]);
  stralloc_nul(d);

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

/**
 * @brief process_entry
 * @param av
 * @param ac
 * @return
 */
int
process_entry(char** av, int ac) {

  if(!str_start(av[0], "\"X") && !str_start(av[0], "["))
    return 0;

  while(ac > 6 && !(av[4] && str_len(av[4]) == 10 && av[6] && str_len(av[6]) == 8)) {
    av++;
    ac--;
  }

  if(ac >= 21) {
    char timebuf[256];
    stralloc datetime;
    struct tm tm;
    time_t t;
    stralloc url_lo;
    unsigned d;
    char *sender, *thema, *title, *duration, *description, *url, *url_klein;

    cleanup_text(&av[2]);
    cleanup_text(&av[3]);
    cleanup_text(&av[8]);

    sender = av[1];
    thema = av[2];
    title = av[3];
    duration = av[6];
    description = av[8];
    url = av[9];
    url_klein = av[13];

    stralloc_init(&url_lo);

    if(str_len(url_klein)) {
      char* endptr = url_klein;
      unsigned long pos_lo = strtoul(url_klein, &endptr, 10);

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

    t = str_ptime(datetime.s, datetime_format, &tm) ? mktime(&tm) : 0;

    {
      int h = 0, m = 0, s = 0;

      if(sscanf(duration, "%d:%d:%d", &h, &m, &s) >= 2) {
        d = h * 3600 + m * 60 + s;
      } else {
        d = 0;
      }
    }

    if(d < 20 * 60)
      return 1;

    // dump_pair(buffer_2, "title",
    // title);

    if(str_len(sender) == 0) {
      stralloc s;
      stralloc_init(&s);
      get_domain(url_lo.s, &s);
      sender = cleanup_domain(&s);
    }

    /*dump_pair(buffer_2, "sender",
    sender); dump_pair(buffer_2,
    "thema", thema);*/

    // dump_pair(buffer_2,
    // "description", description);
    /*
      dump_long(buffer_2, "d", d);
      dump_pair(buffer_2, "duration",
      duration); dump_pair(buffer_2,
      "url", url); dump_pair(buffer_2,
      "url_lo.n", av[13]);
      dump_pair(buffer_2, "url_lo.s",
      url_lo.s);
    */

    strftime(timebuf, sizeof(timebuf), "%Y%m%d %H:%M", &tm);

    output_handlers[output_format](sender, thema, title, d, timebuf, lowq > 0 ? url_lo.s : url, description);

    (void)t;
  } else {
    return 0;
  }

  return 1;
}

/**
 * @brief put_quoted_string
 * @param str
 */
void
put_quoted_string(const char* str) {
  buffer_putc(&output_buf, '"');

  while(*str) {
    char c = *str++;

    if(c == '"' || c == '\\') {
      buffer_puts(&output_buf, c == '\\' ? "\\\\" : "\"\"");
    } else {
      buffer_PUTC(&output_buf, c);
    }
  }
  buffer_putc(&output_buf, '"');
}

/**
 * @brief output_entry
 * @param sender
 * @param thema
 * @param title
 * @param duration
 * @param datetime
 * @param url
 * @param description
 */
void
output_m3u_entry(const char* sender, const char* thema, const char* title, unsigned duration, const char* datetime, const char* url, const char* description) {

  if(csv == 0) {
    buffer_puts(&output_buf, "#EXTINF:");
    buffer_putulong(&output_buf, duration);
    buffer_put(&output_buf, ",|", 2);
    buffer_put(&output_buf, datetime, str_len(datetime));
    buffer_puts(&output_buf, "|");
    buffer_puts(&output_buf, sender);
    buffer_puts(&output_buf, "|");
    buffer_puts(&output_buf, thema);
    buffer_puts(&output_buf, "|");
    buffer_puts(&output_buf, title);
    buffer_puts(&output_buf, "|");
    buffer_puts(&output_buf, description);
    buffer_put(&output_buf, "\r\n", 2);
    buffer_puts(&output_buf,
                "#EXTVLCOPT:network-"
                "caching=2500\r\n");
    buffer_puts(&output_buf, url);
  } else {
    put_quoted_string(sender);
    buffer_puts(&output_buf, ",");
    put_quoted_string(thema);
    buffer_puts(&output_buf, ",");
    put_quoted_string(title);
    buffer_puts(&output_buf, ",");
    buffer_put(&output_buf, datetime, str_len(datetime));
    buffer_puts(&output_buf, ",");
    buffer_putulong(&output_buf, duration);
    buffer_puts(&output_buf, ",");
    put_quoted_string(description);
    buffer_puts(&output_buf, ",");
    put_quoted_string(url);
  }
  buffer_put(&output_buf, "\r\n", 2);
  buffer_flush(&output_buf);
}
void
output_wget_entry(const char* sender, const char* thema, const char* title, unsigned duration, const char* datetime, const char* url, const char* description) {
  int skipSender = str_start(thema, sender);
  int multiline = 0;
  buffer_putm_internal(&output_buf, prefix_cmd ? prefix_cmd : "", prefix_cmd ? " " : "", multiline ? "wget \\\n  -c " : "wget -c ", url, NULL);
  buffer_putm_internal(&output_buf, multiline ? " \\\n  -O '" : " -O '", NULL);

  if(!skipSender) {
    buffer_puts_escaped(&output_buf, sender, &fmt_escapecharnonprintable);
    buffer_puts(&output_buf, " - ");
  }

  buffer_puts_escaped(&output_buf, thema, &fmt_escapecharnonprintable);
  buffer_puts(&output_buf, " - ");
  buffer_puts_escaped(&output_buf, title, &fmt_escapecharnonprintable);

  buffer_putm_internal(&output_buf, ".mp4'", "\ntouch -c -d '", datetime, "' '", NULL);

  if(!skipSender) {
    buffer_puts_escaped(&output_buf, sender, &fmt_escapecharnonprintable);
    buffer_puts(&output_buf, " - ");
  }

  buffer_puts_escaped(&output_buf, thema, &fmt_escapecharnonprintable);
  buffer_puts(&output_buf, " - ");
  buffer_puts_escaped(&output_buf, title, &fmt_escapecharnonprintable);
  buffer_putm_internal(&output_buf, ".mp4'", NULL);
  buffer_putnlflush(&output_buf);
}

void
output_curl_entry(const char* sender, const char* thema, const char* title, unsigned duration, const char* datetime, const char* url, const char* description) {

  buffer_putm_internal(&output_buf, "curl -L -k ", url, NULL);
  buffer_putm_internal(&output_buf, " -o '", sender, " - ", thema, " - ", title, ".mp4'", NULL);
  /*    buffer_puts(&output_buf, "|");
      buffer_puts(&output_buf,
     description);*/

  buffer_putnlflush(&output_buf);
}

ssize_t
get_line(buffer* input, stralloc* sa) {
  bool done = false;
  stralloc_zero(sa);

  while(!done) {
    if(buffer_get_token_sa(input, sa, "]", 1) <= 0)
      break;

    if(buffer_prefetch(input, 6) < 6)
      break;

    done = byte_equal(&input->x[input->p], 5, ",\"X\":");

    if(!done)

      done = byte_equal(&input->x[input->p], 6, ",\n\"X\":");

    stralloc_append(sa, &input->x[input->p]);
    buffer_skipc(input);

    if(input->x[input->p] == '\n')
      buffer_skipc(input);
  }

  return sa->len;
}

/**
 * @brief process_input
 * @param input
 * @return
 */
int
process_input(buffer* input) {
  int ret = -1;
  size_t line = 0 /*, index = 0*/;
  stralloc sa;
  strlist fields;
  stralloc_init(&sa);
  strlist_init(&fields, '\0');

  if(csv == 0 && output_format == M3U)
    buffer_puts(&output_buf, "#EXTM3U\r\n");

  for(stralloc_init(&sa); get_line(input, &sa); stralloc_zero(&sa)) {
    ++line;

    strlist_init(&fields, '\0');

    ret = read_line(sa.s, sa.len, &fields);

    /*        buffer_puts(buffer_2,
       "Line ");
            buffer_putulong(buffer_2,
       line); buffer_puts(buffer_2, ":
       "); buffer_putsa(buffer_2, &sa);
            buffer_putnlflush(buffer_2);
    */

    if(fields.sa.s) {
      strarray arr;

      char** v = strlist_to_argv(&fields);
      int c = strlist_count(&fields);
      strarray_init(&arr);
      strarray_from_argv(c > 21 ? 21 : c, (const char**)v, &arr);

      if(!process_entry(strarray_BEGIN(&arr), strarray_size(&arr)))
        strlist_dump(buffer_2, &fields);

      strarray_free(&arr);

      free(v);
    }
  }

  buffer_close(input);
  return ret;
}

void
usage(const char* argv0) {
  buffer_putm_internal(buffer_2,
                       "Usage: ",
                       argv0,
                       "[OPTIONS] <file>\n",
                       "\n",
                       "  -h, --help                  Show this help\n",
                       "  -c, --csv                   Output as CSV\n",
                       "  -d, --debug                 Debug mode\n",
                       "  -l, --low                   Low quality\n",
                       "  -o, --output FILE           Output file\n",
                       "  -F, --format FMT            Output format\n",
                       "\n",
                       "Valid formats:\n",
                       "  wget, curl, m3u\n",
                       NULL);

  buffer_putnlflush(buffer_2);
}

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int
main(int argc, char* argv[]) {
  int opt, index = 0;
  char inbuf[8192];
  buffer b;
  const char* output_file = 0;

  struct unix_longopt opts[] = {
      {"help", 0, NULL, 'h'},
      {"csv", 0, NULL, 'c'},
      {"debug", 0, NULL, 'd'},
      {"low", 0, NULL, 'l'},
      {"format", 1, NULL, 'F'},
      {"output", 1, NULL, 'o'},
      {"prefix-cmd", 1, NULL, 'P'},
      {0, 0, 0, 0},
  };

  while((opt = unix_getopt_long(argc, argv, "hcdf:t:i:x:lF:o:P:", opts, &index)) != -1) {
    if(opt == 0)
      continue;

    switch(opt) {
      case 'h':
        usage(argv[0]);
        return 0;
        break;
      case 'c': csv = 1; break;
      case 'd': debug++; break;
      case 'l': lowq++; break;
      case 'o': output_file = unix_optarg; break;
      case 'P': prefix_cmd = unix_optarg; break;
      case 'F': {
        if(case_equals(unix_optarg, "wget"))
          output_format = WGET;
        else if(case_equals(unix_optarg, "curl"))
          output_format = CURL;
        else
          output_format = M3U;

        break;
      }
      case 'f': datetime_format = unix_optarg; break;
      default: /* '?' */ usage(argv[0]); exit(EXIT_FAILURE);
    }
  }

  if(output_file)
    buffer_truncfile(&output_buf, output_file);
  else
    buffer_write_fd(&output_buf, STDOUT_FILENO);

  if(unix_optind == argc) {
    ++argc;
    argv[unix_optind] = "-";
  }

  while(unix_optind < argc) {

    if(str_diff(argv[unix_optind], "-")) {
      buffer_puts(buffer_2, "Processing '");
      buffer_puts(buffer_2, argv[unix_optind]);
      buffer_puts(buffer_2, "' ... ");

      if(buffer_mmapread(&b, argv[unix_optind])) {
        buffer_puts(buffer_2, "failed");
        buffer_putnlflush(buffer_2);
        return 1;
      } else {
        buffer_putnlflush(buffer_2);
      }
    } else {
      buffer_init(&b, (buffer_op_proto*)(void*)&read, STDIN_FILENO, inbuf, sizeof(inbuf));
    }
    process_input(&b);
    ++unix_optind;
  }

  return 0;
}
