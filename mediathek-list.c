#include <unistd.h>
#include <stdlib.h>
#include "buffer.h"
#include "strlist.h"

const char* const mediathek_url = "http://download10.onlinetvrecorder.com/mediathekview/Filmliste-akt.xz";

static ssize_t
buffer_dummyread(int fd, char* b, size_t n) { return 0; }

int
split_fields(strlist* sl, char* buf, size_t n)
{
  char buf2[4096];
  buffer b = BUFFER_INIT(buffer_dummyread, 0, buf, n);
  b.n = b.a;

  strlist_zero(sl);
  int ret;


  while((ret = buffer_get_token(&b, buf2, sizeof(buf2), "\"", 1)) > 0) {

    ret = buffer_get_token(&b, buf2, sizeof(buf2), "\"", 1);

    if(ret > 0) {

      strlist_pushb(sl, buf2, ret);
      /*buffer_put(buffer_2, "\n", 1);
      buffer_put(buffer_2, buf2, ret);
      buffer_put(buffer_2, "\n", 1);
      buffer_flush(buffer_2);*/
    }
  }

  strlist_dump(buffer_2, sl);
}

int
get_mediathek_list(const char* url)
{
  int xzpid;
  int xzpipe[2];

  if(pipe(xzpipe) != 0) return -1;

  if((xzpid = fork()) == 0) {
    int clpipe[2];
    int clpid;


    if(pipe(clpipe) != 0) return -1;

    close(STDOUT_FILENO);
    dup(xzpipe[1]);
    close(STDIN_FILENO);
    dup(clpipe[0]);

    if((clpid = fork()) == 0) {

      close(STDOUT_FILENO);
      dup(clpipe[1]);

      execlp("curl", "curl", "-s", url, NULL);
      exit(1);
    }

    execlp("xzcat", "xzcat", NULL);
    exit(1);
  }

  {
    char buf[1024];
    char buf2[16384];
    ssize_t ret;
    strlist sl;
    buffer b = BUFFER_INIT(read, xzpipe[0], buf, sizeof(buf));

    strlist_init(&sl);

    while((ret = buffer_get_token(&b, buf2, sizeof(buf2), "]", 1)) > 0) {
      buf2[ret++] = ']';

      split_fields(&sl, buf2, ret);

      /*buffer_put(buffer_1, buf2, ret);
      buffer_put(buffer_1, "\n", 1);*/
    }
    buffer_flush(buffer_1);

  }


}

int main()
{
  get_mediathek_list(mediathek_url);
}
