
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <sys/wait.h>
#include <signal.h>
#include "buffer.h"
#include "strlist.h"
#include "str.h"

const char* argv0;

const char* const mediathek_url = "http://verteiler1.mediathekview.de/Filmliste-akt.xz";

static ssize_t
buffer_dummyread(int fd, char* b, size_t n) { return 0; }

void
count_field_lengths(strlist* sl, int lengths[21])
{
  int i;
  for(i = 0; i < 21; i++) {
    const char* s = strlist_at(sl, i);
    if(s) {
      if(str_len(s) >= lengths[i])
        lengths[i] = str_len(s);
    }
  }
}

int
split_fields(strlist* sl, char* buf, size_t n)
{
  int ret = 0;
  char buf2[4096];
  char *p = buf;
  char *end = buf + n;

  strlist_zero(sl);

  for(;;) {
    size_t n = 0;
    while(p < end && *p != '"')
      ++p;

    if(p == end) break;

    ++p;

    while(p < end) {

      if(*p == '\\' && p[1] == '"')	{ ++p; } else {
        if(*p == '"') { ++p; break; }
      }

      buf2[n++] = *p++;
    }

    strlist_pushb(sl, buf2, n);
    ret++;
  }

  strlist_dump(buffer_2, sl);
  return ret;
}

void process_status(void)
{
  /* display interesting process IDs  */
  fprintf(stderr, "process %s: pid=%d, ppid=%d, pgid=%d, fg pgid=%dn\n",
         argv0, (int)getpid(), (int)getppid(),
         (int)getpgrp(), (int)tcgetpgrp(STDIN_FILENO));
}

int
read_mediathek_list(const char* url)
{
/*  int status;
  int xzpid;
  int xzpipe[2];
  int clpipe[2];
  int clpid;

  process_status();

  if(pipe(xzpipe) != 0) return -1;
  if(pipe(clpipe) != 0) return -1;

  if((xzpid = fork()) == 0) {

    close(STDOUT_FILENO);
    dup(xzpipe[1]);
    close(STDIN_FILENO);
    dup(clpipe[0]);

    execlp("xzcat", "xzcat", NULL);
    exit(1);
  }

  if((clpid = fork()) == 0) {

    close(STDOUT_FILENO);
    dup(clpipe[1]);

    execlp("curl", "curl", "-s", url, NULL);
    exit(1);
  }

  return xzpipe[0];*/
  
  stralloc cmd;
  stralloc_init(&cmd);
  stralloc_copys(&cmd, "curl -s ");
  stralloc_cats(&cmd, url);
  stralloc_cats(&cmd, "| xzcat");
  stralloc_0(&cmd);
  
  static FILE *pfd;
  
  if((pfd  = popen(cmd.s, "r")) == NULL) return -1;
  
  return fileno(pfd);
}


int
parse_mediathek_list(int fd)
{
  char buf[1024];
  char buf2[16384];
  ssize_t ret, ret2;
  strlist sl;
  buffer b = BUFFER_INIT(read, fd, buf, sizeof(buf));

  strlist_init(&sl);

  while((ret = buffer_get_token(&b, buf2, sizeof(buf2), "]", 1)) > 0) {

    for(;;) {
      buf2[ret++] = ']';
      ret2 = buffer_get(&b, &buf2[ret], 1);
      if(ret2 > 0) {

        if(buf2[ret] == ',') break;

        ret += ret2;
        ret2 = buffer_get_token(&b, &buf2[ret], sizeof(buf2) - ret, "]", 1);
        if(ret2 > 0)
          ret += ret2;
      }
    }

    split_fields(&sl, buf2, ret);
  }

  buffer_flush(buffer_1);
}

int main(int argc, char *argv[])
{
  argv0 = basename(argv[0]);
  return parse_mediathek_list(read_mediathek_list(mediathek_url));
}

