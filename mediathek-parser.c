#include "strlist.h"
#include "stralloc.h"
#include "strlist.h"
#include "buffer.h"
#include "byte.h"
#include "open.h"
#include "array.h"
#include "str.h"
#include <errno.h>
#include <ctype.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stdlib.h>

static const char* file_path = "filme.json"; //"C:/Users/roman/.mediathek3/filme.json"; //"D:/Programs/MediathekView_11_2015.09.15/Einstellungen/.mediathek3/filme.json";
static const char delimiters[] = "\"";
static char  inbuf[16384];

/*
int
parse_predicate(const char* x, size_t len)
{

  while (isspace(*x) && len > 0) {
    ++x;
    --len;
  }
  if (len == 0)
    return 0;


  size_t pos = byte_chr(x, '\n', len);

  if (pos != len && pos > 0)
    return 1;


  buffer_puts(buffer_1, "Predicate: ");
  buffer_put(buffer_1, x, len);
  buffer_puts(buffer_1, " (");
  buffer_putulong(buffer_1, len);
  buffer_putsflush(buffer_1, ")\n");

  if (len > 0 && x[len - 1] == ' ')
    return 1;

  return 0;
}
*/

int
read_line(buffer* input, strlist* fields, array* x)
{
  int64 pos = 0;

  const char* end = input->x + input->n, *p = input->x + input->p;
  int ret = 0, quoted = 0, escaped = 0;
  size_t n, i = 0;
  char tokbuf[16384];

  array_trunc(x);
  //array_allocate(&x, sizeof(char*), pos);

  if ((n = byte_chr(p, end - p, '\n')) != end - p)
    end = p + n;

  while (p < end && *p != '"')
    ++p;

  for (; p < end; ++p, escaped = 0) {
    if (*p == '\\') {
      escaped = 1;
      ++p;
    }

    else if (*p == '"') {
      if (!quoted) {
        quoted = 1;
        i = 0;
        continue;
      } else {
        quoted = 0;
        char** a = array_allocate(x, sizeof(char*), pos++);
        *a++ = strndup(tokbuf, i);
        //        *a = NULL;

        //        strlist_pushb(fields, tokbuf, i);
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

  // *arr = array_start(&x);
  return p - &input->x[input->p];
}

void
strarray_dump(buffer* b, const array* a)
{
  char** av = array_start(a);
  ssize_t ac = array_length(a, sizeof(char*));
  buffer_puts(b, "[ \"");
  while (ac--) {
    buffer_puts(b, *av);
    if (ac > 0) buffer_puts(b, "\", \"");
    ++av;
  }
  buffer_putsflush(b, "\" ]\n");
}

void
dump_pair(buffer* b, const char* first, const char* second)
{
  buffer_puts(b, first);
  buffer_puts(b, ": ");
  buffer_puts(b, second);
  buffer_putnlflush(b);
}
void
dump_long(buffer* b, const char* name, long value)
{
  buffer_puts(b, name);
  buffer_puts(b, ": ");
  buffer_putlong(b, value);
  buffer_putnlflush(b);
}

void
get_domain(const char* url, stralloc* d)
{
  while (*url && *url != ':')
    ++url;
  ++url;
  while (*url && *url == '/')
    ++url;
  stralloc_copyb(d, url, str_chr(url, '/'));
}

void
process_entry(const array* a)
{

  char** av = array_start(a);
  size_t ac = array_length(a, sizeof(char*));

  if (ac >= 21 && !str_diff(av[0], "X")) {
    stralloc datetime;
    struct tm tm;
    time_t t;
    size_t d;

    char* sender = av[1], *thema = av[2], *title = av[3], *date = av[4], *time = av[5], *duration = av[6], *grcoee = av[7], *description = av[8], *url = av[9], *website = av[10], *untertitel = av[11], *urlrtmp = av[12], *url_klein = av[13], *urlrtmp_klein = av[14], *url_hd = av[15], *urlrtmp_hd = av[16], *datuml = av[17], *url_history = av[18], *geo = av[19], *neu = av[20];

    /*    char* title = av[8];
        char* date = av[4];
        char* time = av[5];
        char* duration = av[6];
        char* url = av[9];
      */
    stralloc url_lo;
    stralloc_init(&url_lo);

    if (str_len(url_klein)) {
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
    if (strptime(datetime.s, "%d.%m.%Y %H:%M:%S", &tm) == NULL) {
      t = 0;
    } else {
      t = mktime(&tm);
    }
    {
      int h = 0, m = 0, s = 0;
      if (sscanf(duration, "%d:%d:%d", &h, &m, &s) >= 2) {
        d = h * 3600 + m * 60 + s;
      } else {
        d = 0;
      }
    }

    if (str_len(thema) == 0)
      return;
    if (d < 20 * 60)
      return;

    for (int i = 0; i < str_len(thema); i++) {
      if (isspace(thema[i])  || thema[i] == ',')
        thema[i] = '_';
    }

    for (int i = 0; i < str_len(title); i++) {
      if (isspace(title[i])  || title[i] == ',')
        title[i] = '_';
    }

    stralloc s;
    stralloc_init(&s);

    if (str_len(sender) == 0) {
      get_domain(url_lo.s, &s);
    } else {
      stralloc_copys(&s, sender);
    }

    dump_pair(buffer_2, "thema", thema);
    dump_pair(buffer_2, "title", title);
    dump_long(buffer_2, "d", d);
    dump_pair(buffer_2, "duration", duration);
    dump_pair(buffer_2, "url", url);
    dump_pair(buffer_2, "url_lo.n", av[13]);
    dump_pair(buffer_2, "url_lo.s", url_lo.s);

    char timebuf[256];

    strftime(timebuf, sizeof(timebuf), "%Y%m%d %H:%M:%S", &tm);

    buffer_puts(buffer_1, "#EXTINF:");
    buffer_putulong(buffer_1, d);
    buffer_put(buffer_1, ",", 1);
    buffer_puts(buffer_1, timebuf);
    buffer_puts(buffer_1, "-");
    buffer_putsa(buffer_1, &s);
    buffer_puts(buffer_1, "-");
    buffer_puts(buffer_1, thema);
    buffer_puts(buffer_1, "-");
    buffer_puts(buffer_1, title);
    buffer_puts(buffer_1, "-");
    buffer_puts(buffer_1, description);
    buffer_put(buffer_1, "\r\n", 2);
    buffer_puts(buffer_1, "#EXTVLCOPT:network-caching=2500\r\n");
    buffer_puts(buffer_1, url_lo.s);
    buffer_put(buffer_1, "\r\n", 2);
    buffer_flush(buffer_1);
  } else {
    strarray_dump(buffer_2, a);

  }
}

int
read_file(const char* p)
{
  buffer input;
  int ret;
  size_t line = 0, index = 0;
  stralloc sa;
  static array arr;
  strlist fields;
  stralloc_init(&sa);
  strlist_init(&fields);

  buffer_puts(buffer_2, "Opening '");
  buffer_puts(buffer_2, file_path);
  buffer_puts(buffer_2, "' ...");
  buffer_putnlflush(buffer_2);
  /*  int fd = open_read(p);


    if (fd == -1) {

      buffer_puts(buffer_2, "Failed to open: ");
      buffer_puts(buffer_2, file_path);
      buffer_puts(buffer_2, " (");
      buffer_putulong(buffer_2, errno);
      buffer_putsflush(buffer_2, ")\n");
      return -1;
    }

    buffer_init(&input, read, fd, 	inbuf, sizeof(inbuf));*/
  buffer_mmapread(&input, file_path);

  buffer_puts(buffer_1, "#EXTM3U\r\n");

  for (; input.p < input.n;) {


    while (input.p < input.n && isspace(input.x[input.p])) {
      if (input.x[input.p] == '\n') {
        /*        if (strlist_count(&fields)) {
                  strlist_dump(buffer_1, &fields);
                }
        */
        ++line;
        index = 0;

      }
      ++input.p;
    }

    strlist_init(&fields);
    array_trunc(&arr);
    ret = read_line(&input, &fields, &arr);
    /*
        buffer_puts(buffer_1, "Line ");
        buffer_putulong(buffer_1, line);
        buffer_puts(buffer_1, ":");

    size_t index = 0;
        while (*arr) {

        buffer_puts(buffer_1, "\n    ");
        buffer_putulong(buffer_1, index++);
        buffer_puts(buffer_1, ": \"");
          buffer_puts(buffer_1, *arr++);
          buffer_puts(buffer_1, "\"");
        }

        buffer_flush(buffer_1);

        //strlist_dump(buffer_1, &fields);
        buffer_putnlflush(buffer_1);
        //  strlist_free(&fields);

        strlist_zero(&fields);
        */

    process_entry(&arr);

    if (ret > 0) {
      input.p += ret;
      /*
            strlist_push_sa(&fields, &sa);

            //if(sa.len > 0)  {
            buffer_puts(buffer_1, "Token (");
            buffer_putulong(buffer_1, line);
            buffer_puts(buffer_1, ":");
            buffer_putulong(buffer_1, index);
            buffer_puts(buffer_1, ",len=");
            buffer_putulong(buffer_1, ret);
            buffer_puts(buffer_1, "): ");
            buffer_putsa(buffer_1, &sa);
            buffer_putnlflush(buffer_1);
            stralloc_zero(&sa);
            //}
      */
    } else {
      ++input.p;

    }


  }
end:
  //  close(fd);
  buffer_close(&input);
  return ret;
}

int main(int argc, char* argv[])
{
  read_file(argc > 1 ? argv[1] : file_path);
}