#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef _WIN32
#include <unistd.h>
#else
#include <io.h>
#endif
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef _WIN32
#include <libgen.h>
#endif

#include "stralloc.h"
#include "buffer.h"
#include "open.h"
#include "fmt.h"
#include "byte.h"
#include "str.h"
#include "scan.h"
#include "uint64.h"

#ifndef PATH_MAX
#define PATH_MAX _MAX_PATH
#endif

INLINE static char*
mybasename(const char* path) {
  char* r = strrchr(path, '/');
  return r ? r + 1:(char *)path;
}

static int  force = 0;


INLINE static char hexchar(char value) {
  static const char hchars[] = "0123456789abcdef";
  return hchars[(unsigned int)((unsigned char)value & 0xf)];
}

static INLINE char char_tolower(char ch) {
  if(ch >= 'A' && ch <= 'Z')
    ch += 0x20;
  return ch;
}

static int find_char(char ch, char* buffer, unsigned int n) {
  unsigned int i;
  int ret = 0;
  for(i = 0; i < n; i++) {
    if(buffer[i] == ch)
      ret++;
  }
  return ret;
}


static ssize_t collapse_unicode(char* buffer, size_t n) {
  size_t i = 0, o = 0;
  while(buffer[i] == '\0')
    i++;

  for(; i < n; o++, i += 2) {
    buffer[o] = buffer[i];
  }
  buffer[o] = '\0';
  return o;
}

typedef enum {
  REGISTRY_NONE = 0,
  REGISTRY_SZ,
  REGISTRY_MULTI_SZ,
  REGISTRY_EXPAND_SZ,
  REGISTRY_DWORD,
  REGISTRY_QWORD,
  REGISTRY_BINARY,
} regtype_t;

const char* regtype_strings[] = {
  "REG_NONE", "REG_SZ", "REG_MULTI_SZ", "REG_EXPAND_SZ", "REG_DWORD", "REG_QWORD", "REG_BINARY",
};

int reg2cmd()
{
  char buffer[PATH_MAX];
  char key[PATH_MAX];
  unsigned int lineno = 0;
  int unicode = 0;
  ssize_t pos, len;
  stralloc line;
  stralloc_init(&line);

  for(;;)
  {
    buffer[0] = '\0';
    buffer[1] = '\0';
    lineno++;
    len = buffer_getline(buffer_0, buffer, sizeof(buffer));

    if(lineno == 1) {
      if(((unsigned char)buffer[0] == 0xff && (unsigned char)buffer[1] == 0xfe) ||
          (buffer[0] == 0x00 || buffer[1] == 0x00)) {
        unicode = 1;
      }
      /*
          buffer_puts(buffer_2, "BOM: ");
          buffer_putulong(buffer_2, (unsigned char)buffer[0]);
          buffer_puts(buffer_2, " ");
          buffer_putulong(buffer_2, (unsigned char)buffer[1]);
          buffer_puts(buffer_2, " ");
          buffer_putulong(buffer_2, (unsigned char)unicode);
          buffer_puts(buffer_2, "\n");
          buffer_flush(buffer_2);
       */
      continue;
    }

    if(len < 0 || (len == 0 && buffer[0] == '\0'))
      break;


    if(unicode) {
      len = collapse_unicode(buffer, len);
    }

    while(len >= 1) {
      if(buffer[len - 1] == '\r' || buffer[len - 1] == '\n')
        len--;
      else
        break;
    }

    if(len == 0) {
      key[0] = '\0';
      continue;
    }

    pos = 0;
    if(line.len) {
      for(; pos < len; pos++) {
        if(buffer[pos] != ' ')
          break;
      }
    }
    stralloc_catb(&line, &buffer[pos], len - pos);

    if(line.len >= 1) {
      if(line.s[line.len - 1] == '\\') {
        line.len--;
        continue;
      }
    }

    stralloc_catb(&line, "\0", 1);
    line.len -= 1;

    if(line.s[0] == '[') {
      const char* end = strrchr(&line.s[1], ']');

      if(end) {
        size_t keylen = end - &line.s[1];
        memcpy(key, &line.s[1], keylen);
        key[keylen] = '\0';
        stralloc_zero(&line);
        continue;
      }
    }

    if(key[0])
    {
      int has_newline = 0, has_expansion = 0;
      const char* type;
			int keystart, keyend, valuestart = 0, valueend;
      regtype_t rt = 0;
      uint64 word = 0;
      int inquote;

      keystart = (line.s[0] == '"' ? 1 : 0);
      inquote = keystart;

			for(keyend = keystart; (unsigned)keyend < line.len; keyend++) {
        if(line.s[keyend] == '\\') {
          keyend++;
          continue;
        }
        if(!inquote && line.s[keyend] == '=') {
          valuestart = keyend + 1;
          if(keystart > 0) {
            keyend--;
          }
//          line.s[keyend] = '\0';
          break;
        }
        if(line.s[keyend] == '"')
          inquote =  !inquote;
      }


      inquote = 0;
      valueend = line.len;

      for(pos = valuestart; pos < valueend; pos++) {
        if(line.s[pos] == '\\') {
          pos++;
          continue;
        }
        if(line.s[pos] == '"')
          inquote = !inquote;
      }

      if(inquote) {
        stralloc_catb(&line, "\r\n", 2);
        continue;
      }

      if(line.s[valuestart] == '"') {
        rt = REGISTRY_SZ;

        if(line.s[valuestart] == '"')
          valuestart++;

        if(line.s[valueend - 1] == '"')
          valueend--;

      } else if(!strncmp(&line.s[valuestart], "hex", 3)) {
        rt = REGISTRY_BINARY;

        while(line.s[valuestart] != ':' && valuestart < valueend)
          valuestart++;

        valuestart++;

      } else if(!strncmp(&line.s[valuestart], "dword:", 6)) {
        unsigned long ul;
        scan_xlong(&line.s[valuestart + 6], &ul);
        word = ul;
        rt = REGISTRY_DWORD;
      } else if(!strncmp(&line.s[valuestart], "qword:", 6)) {
        unsigned long long ull;
        scan_xlonglong(&line.s[valuestart + 6], &ull);
        word = ull;
        rt = REGISTRY_QWORD;
      } else {
        buffer_puts(buffer_2, "No such type: ");
        buffer_put(buffer_2, &line.s[valuestart], line.len - valuestart);
        buffer_puts(buffer_2, "\n");
        buffer_flush(buffer_2);
        exit(2);
      }

      buffer_puts(buffer_1, "REG ADD \"");
      buffer_puts(buffer_1, key);
      buffer_puts(buffer_1, "\" ");

      if(force)
        buffer_puts(buffer_1, "/f ");

      if(line.s[keystart] == '@' && (keyend - keystart) == 1) {
        buffer_puts(buffer_1, "/ve ");
      } else {
        buffer_puts(buffer_1, "/v \"");
        buffer_put(buffer_1, &line.s[keystart], keyend - keystart);
        buffer_puts(buffer_1, "\" ");
      }

      has_newline =
        (find_char('\n', &line.s[valuestart], valueend - valuestart) ||
         find_char('\r', &line.s[valuestart], valueend - valuestart));


      has_expansion =
        (find_char('%', &line.s[valuestart], valueend - valuestart) >= 2);

      if(has_expansion && rt == REGISTRY_SZ)
        rt = REGISTRY_EXPAND_SZ;

      if(valueend == valuestart && rt == REGISTRY_BINARY)
        type = "REG_SZ";
      if(has_newline && rt == REGISTRY_SZ)
        type = "REG_BINARY";
      else
        type = regtype_strings[rt];

      buffer_puts(buffer_1, "/t ");
      buffer_puts(buffer_1, type);

      buffer_puts(buffer_1, " /d ");

      switch(rt) {
      //case REGISTRY_BINARY:
      case REGISTRY_EXPAND_SZ: {
        buffer_putc(buffer_1, '"');
        for(pos = valuestart; pos < valueend; pos++) {
          if(line.s[pos] == '%')
            buffer_putc(buffer_1, '^');
          buffer_putc(buffer_1, line.s[pos]);
        }
        buffer_putc(buffer_1, '"');
        break;
      }
      case REGISTRY_SZ:  {
        buffer_putc(buffer_1, '"');
        if(has_newline) {
          for(pos = valuestart; pos < valueend; pos++) {
            buffer_putc(buffer_1, hexchar((unsigned char)line.s[pos] >> 4));
            buffer_putc(buffer_1, hexchar((unsigned char)line.s[pos] & 0x0f));
          }
        } else {
          buffer_put(buffer_1, &line.s[valuestart], valueend - valuestart);
        }
        buffer_putc(buffer_1, '"');
        break;
      }
      case REGISTRY_DWORD:  {
        buffer_putuint64(buffer_1, word);
        break;
      }
      case REGISTRY_QWORD:  {
        buffer_putuint64(buffer_1, word);
        break;
      }
      case REGISTRY_BINARY:  {
        buffer_putc(buffer_1, '"');
        for(pos = valuestart; pos < valueend; pos++) {
          if(scan_fromhex(line.s[pos]) != -1)
            buffer_putc(buffer_1, char_tolower(line.s[pos]));
        }
        buffer_putc(buffer_1, '"');
        break;
      }
      default: {
        buffer_puts(buffer_2, "Unhandled type: ");
        buffer_puts(buffer_2, regtype_strings[rt]);
        buffer_puts(buffer_2, "\n");
        buffer_flush(buffer_2);
        exit(2);
        break;
      }
      }


      buffer_puts(buffer_1, "\r\n");
      buffer_flush(buffer_1);
    }
    /*
    buffer_puts(buffer_2, "Line ");
    buffer_putulong(buffer_2, lineno);
    buffer_puts(buffer_2, ": ");
    buffer_put(buffer_2, line.s, line.len);
    buffer_puts(buffer_2, "\n");
    buffer_flush(buffer_2);


    buffer_puts(buffer_2, "Length: ");
    buffer_putulong(buffer_2, len);
    buffer_puts(buffer_2, "\n");
    buffer_flush(buffer_2);
    */
    /*  buffer_put(buffer_1, "\r\n", 2);
      buffer_flush(buffer_1);
      */
    stralloc_zero(&line);
  }
  return 0;
}

void usage(char* arg0)
{
  buffer_puts(buffer_2, "Usage: ");
  buffer_puts(buffer_2, mybasename(arg0));
  buffer_puts(buffer_2, " [-f] [input - file] [output - file]\n");
  buffer_flush(buffer_2);
  exit(1);
}
int main(int argc, char* argv[])
{
  int argi;

  for(argi = 1; argi < argc; argi++)
  {
    char* arg = argv[argi];
    if(arg[0] == '-')
    {
      switch(arg[1])
      {
      case 'f':
        force++;
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
    buffer_puts(buffer_2, "Opening file for reading '");
    buffer_puts(buffer_2, argv[argi]);
    buffer_puts(buffer_2, "' ...\n");
    buffer_flush(buffer_2);

    if((buffer_0->fd = open(argv[argi], O_RDONLY)) < 0)
      usage(argv[0]);

    argi++;
  }
  if(argi < argc)
  {
    buffer_puts(buffer_2, "Opening file for writing '");
    buffer_puts(buffer_2, argv[argi]);
    buffer_puts(buffer_2, "' ...\n");
    buffer_flush(buffer_2);
    if((buffer_1->fd = open(argv[argi], O_CREAT|O_TRUNC|O_WRONLY, 0644)) < 0)
      usage(argv[0]);

    argi++;
  }
  reg2cmd();
  return 0;
}

