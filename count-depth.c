#include <unistd.h>
#include <limits.h>
#include "buffer.h"
#include "fmt.h"

static char buffer_0_in[BUFFER_INSIZE];
static buffer buffer_0 = BUFFER_INIT((void*)read, 0, buffer_0_in, BUFFER_INSIZE);

static char buffer_1_out[BUFFER_OUTSIZE];
static buffer buffer_1 = BUFFER_INIT((void*)write, 1, buffer_1_out, BUFFER_OUTSIZE);

int count_depth()
{
  char buffer[PATH_MAX];
  char num[FMT_ULONG];
  unsigned long len, i, c;
  for(;;)
  {
    buffer[0] = '\0';
    len = buffer_getline(&buffer_0, buffer, sizeof(buffer));

    if(len == 0 || buffer[0] == '\0')
      break;

    if(buffer[len - 1 ] == '/')
      len--;

    c=0;
    for(i = 0; i < len; i++)
    {
      if(buffer[i] == '/')
        c++;
    }
    buffer_putulong(&buffer_1, c);
    buffer_put(&buffer_1, " ", 1);
    buffer_put(&buffer_1, buffer, len);
    buffer_put(&buffer_1, "\n", 1);
  }
  buffer_flush(&buffer_1);
}

int main(int argc, char *argv[])
{

  count_depth();
}

