// decode-ls-lR.c

#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#include "buffer.h"
#include "open.h"
#include "fmt.h"
#include "byte.h"
#include "stralloc.h"

static int skip_fields = 8;
static char *delimiters = " \t\r";
static unsigned long delimiters_len;
static char buffer_0_in[BUFFER_INSIZE];
static buffer buffer_0 = BUFFER_INIT((void*)read, 0, buffer_0_in, BUFFER_INSIZE);

static char buffer_1_out[BUFFER_OUTSIZE];
static buffer buffer_1 = BUFFER_INIT((void*)write, 1, buffer_1_out, BUFFER_OUTSIZE);


static char buffer_2_out[BUFFER_OUTSIZE];
static buffer buffer_2 = BUFFER_INIT((void*)write, 2, buffer_2_out, BUFFER_OUTSIZE);

static stralloc pfx;

int is_delimiter(char c)
{
  return !(byte_chr(delimiters, delimiters_len, c) == delimiters_len);
}
unsigned long skip_field(int n, char *s, unsigned long len)
{
  unsigned long ret=0;
  while(n-- && ret < len)
  {
    if(ret == len) return ret;


    while(ret < len && !is_delimiter(s[ret]))
      ret++;

    if(ret == len) return ret;

    while(ret < len && is_delimiter(s[ret]))
      ret++;

    if(ret == len) return ret;
  }
  return ret;
}

int decode_ls_lR()
{
  char buffer[PATH_MAX];
  unsigned long pos;
  char num[FMT_ULONG];
  unsigned long len, i, c;
  char last;
  stralloc dir;
  stralloc_init(&dir);

  for(;;)
  {
    buffer[0] = '\0';
    len = buffer_getline(&buffer_0, buffer, sizeof(buffer));

    if(len < 0) // || buffer[0] == '\0')
      break;

    if(len == 0) continue;

    last = buffer[len - 1];

    if(last == '/' || last == ':')
    {
      if(last == ':') len--;

      stralloc_copyb(&dir,buffer,len);
      continue;
    }

    pos = skip_field(skip_fields, buffer, len);

    if(pfx.len) buffer_putsa(&buffer_1, &pfx);

    if(dir.len) buffer_putsa(&buffer_1, &dir);

    buffer_put(&buffer_1, &buffer[pos], len-pos);
    buffer_put(&buffer_1, "\n", 1);
  }
  buffer_flush(&buffer_1);
}
void usage(const char *arg0)
{
  buffer_puts(&buffer_2, "Usage: ");
  buffer_puts(&buffer_2, (char *)basename(arg0));
  buffer_puts(&buffer_2, " [Options]\n");
  buffer_puts(&buffer_2, " -s num   Skip <num> Number of fields\n");
  buffer_puts(&buffer_2, " -t chars Delimiter chars\n");
  buffer_puts(&buffer_2, " -p str   prefix to prepend\n");
  buffer_flush(&buffer_2);
  exit(1);
}
int main(int argc, char *argv[])
{
  int argi;

  for(argi = 1; argi < argc; argi++)
  {
    char *arg = argv[argi];
    if(arg[0] == '-')
    {
      switch(arg[1])
      {
      case 's':
        argi++;
        if(argi<argc)
          skip_fields = atoi(argv[argi]);
        break;
      case 'p':
        argi++;
        if(argi<argc)
        {
          stralloc_copys(&pfx, argv[argi]);
          if(pfx.len > 0)
          {
            if(pfx.s[pfx.len - 1] != '/')
              stralloc_catc(&pfx, '/');
          }
        }
        break;
      case 't':
        argi++;
        if(argi<argc) {
          delimiters = argv[argi];
          delimiters_len = strlen(delimiters);
        }
        break;
      default:
        usage(argv[0]);
        break;
      }
    }
    else break;
  }
  if(argi < argc)
  {
    buffer_puts(&buffer_2, "Opening file ");
    buffer_puts(&buffer_2, argv[argi]);
    buffer_puts(&buffer_2, "...\n");
    buffer_flush(&buffer_2);
    if((buffer_0.fd = open(argv[argi], O_RDONLY)) < 0)
      usage(argv[0]);
  }
  delimiters_len = strlen(delimiters);
  decode_ls_lR();
}

