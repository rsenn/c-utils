#include "../byte.h"
#include "../dns.h"
#include <errno.h>
#include <stdlib.h>

int
dns_domain_fromdot(char** out, const char* buf, size_t n) {
  char label[63], name[255];
  unsigned int labellen = 0, namelen = 0;
  char ch, *x;

  errno = EINVAL;

  for(;;) {
    if(!n)
      break;
    ch = *buf++;
    --n;

    if(ch == '.') {
      if(labellen) {
        if(namelen + labellen + 1 > sizeof name)
          return 0;
        name[namelen++] = labellen;
        byte_copy(name + namelen, labellen, label);
        namelen += labellen;
        labellen = 0;
      }
      continue;
    }

    if(ch == '\\') {
      if(!n)
        break;
      ch = *buf++;
      --n;

      if((ch >= '0') && (ch <= '7')) {
        ch -= '0';

        if(n && (*buf >= '0') && (*buf <= '7')) {
          ch <<= 3;
          ch += *buf - '0';
          ++buf;
          --n;

          if(n && (*buf >= '0') && (*buf <= '7')) {
            ch <<= 3;
            ch += *buf - '0';
            ++buf;
            --n;
          }
        }
      }
    }

    if(labellen >= sizeof label)
      return 0;
    label[labellen++] = ch;
  }

  if(labellen) {
    if(namelen + labellen + 1 > sizeof name)
      return 0;
    name[namelen++] = labellen;
    byte_copy(name + namelen, labellen, label);
    namelen += labellen;
  }

  if(namelen + 1 > sizeof name)
    return 0;
  name[namelen++] = 0;

  if(!(x = malloc(namelen)))
    return 0;
  byte_copy(x, namelen, name);

  if(*out)
    free(*out);
  *out = x;
  return 1;
}
