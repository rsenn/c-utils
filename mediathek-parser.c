#include "strlist.h"
#include "stralloc.h"
#include "strlist.h"
#include "buffer.h"
#include "byte.h"
#include "open.h"
#include "array.h"
#include <errno.h>
#include <ctype.h>

#include <stdbool.h>

static const char* file_path = "D:/Programs/MediathekView_11_2015.09.15/Einstellungen/.mediathek3/filme.json";
static const char delimiters[] = "\"";
static char  inbuf[16384];


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


int
read_line(buffer* input, strlist* fields, char** * arr)
{
  static array x;
  int64 pos = 0;

  const char* end = input->x + input->n, *p = input->x + input->p;
  int ret = 0, quoted = 0, escaped = 0;
  size_t n, i = 0;
  char tokbuf[16384];

  *arr = (char**)array_allocate(&x, sizeof(char*), pos);



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
        char** a = array_allocate(&x, sizeof(char*), pos++);
        *a++ = strndup(tokbuf, i);
        *a = NULL;

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
  /*
      while (p < end && isspace(*p))
        ++p;

      while (p < end && *p != '"')
        ++p;

      if (p >= end || *p == '\n') goto end;


      if (*p == '"') {
        const char* x = ++p;
        bool escaped = false;
        i = 0;


        while (x < end && *x != '"') {
          if (*x == '\\') {
            escaped = true;
            if (++x == end)
              break;
          }
          if (*x == '"' && !escaped) {
            if (++x == end || *x == '\n')
              return x - input->x;

            if(*x == ',' && x < end)
              ++x;

            while (x < end && (isspace(*x) && *x != '\n'))
              ++x;


            strlist_pushb(fields, tokbuf, i);

            ret = x - p;

            i=0;
            break;

          }
          tokbuf[i++] = *x;

          escaped = false;
          ++x;
        }
        if(x < end && *x == ',')
          ++x;

        while(x < end && *x != '"' && *x != '\n')
              ++x;

        p = x;
      }

    }*/
  *arr = array_start(&x);
  return p - &input->x[input->p];
}

int
read_file(const char* p)
{
  buffer input;
  int ret;
  size_t line = 0, index = 0;
  stralloc sa;
  char** arr;
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
    ret = read_line(&input, &fields, &arr);

    buffer_puts(buffer_1, "Line ");
    buffer_putulong(buffer_1, line);
    buffer_puts(buffer_1, ": \"");

    while (*arr) {

      buffer_puts(buffer_1, *arr++);
      buffer_puts(buffer_1, "\"\n   \"");
    }
    buffer_flush(buffer_1);

    //strlist_dump(buffer_1, &fields);
    buffer_putnlflush(buffer_1);
    //  strlist_free(&fields);

    /*    char c;

        do {
          if ((ret = buffer_get(&input, &c, 1)) != 1)
    			goto end;


        } while (isspace(c));*/

    //ret = buffer_getline(&input, tokbuf, sizeof(tokbuf));
    strlist_zero(&fields);
    //    ret = 0;
    //ret = buffer_get_token(&input, tokbuf, sizeof(tokbuf), "\"\r\n", 4);
    //ret = buffer_get_token_sa(&input, &sa, delimiters, sizeof(delimiters));
    //    ret = buffer_get_token_pred(&input, tokbuf, sizeof(tokbuf), parse_predicate);

    /*
    buffer_puts(buffer_1, "Buffer: ");
    buffer_dump(buffer_1, &input);
    buffer_putnlflush(buffer_1);
    */

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

int main()
{
  read_file(file_path);
}