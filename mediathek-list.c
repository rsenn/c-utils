#include <unistd.h>
#include <stdlib.h>
#include "buffer.h"

const char* const mediathek_url = "http://download10.onlinetvrecorder.com/mediathekview/Filmliste-akt.xz";

int
get_mediathek_list(const char* url)
{
  int pid;
  int pipe1[2];

  if(pipe(pipe1) != 0) return -1;

  if((pid = fork()) == 0) {
    int pipe2[2];
    int pid2;


    if(pipe(pipe2) != 0) return -1;

    close(STDOUT_FILENO);
    dup(pipe1[1]);
    close(STDIN_FILENO);
    dup(pipe2[0]);

    if((pid2 = fork()) == 0) {

      close(STDOUT_FILENO);
      dup(pipe2[1]);

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
    buffer b = BUFFER_INIT(read, pipe1[0], buf, sizeof(buf));

    while((ret = buffer_get_token(&b, buf2, sizeof(buf2), "]", 1)) > 0) {
			if(buf2[ret - 1] == ']') 
        buffer_put(buffer_1, "\n", 1);
      buffer_put(buffer_1, buf2, ret);
    }
    buffer_flush(buffer_1);

  }


}

int main()
{
  get_mediathek_list(mediathek_url);
}
