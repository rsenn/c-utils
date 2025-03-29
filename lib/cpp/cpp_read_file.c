#include "../cpp_internal.h"
#include "../str.h"
#include <stdio.h>

/* Returns the contents of a given file. */
char*
cpp_read_file(char* path) {
  FILE* fp;

  if(str_equal(path, "-")) {
    /* By convention, read from stdin if a given filename is "-". */
    fp = stdin;
  } else {
    if(!(fp = fopen(path, "r")))
      return NULL;
  }

  {
    char* buf;
    size_t buflen;
    FILE* out = open_memstream(&buf, &buflen);

    /* Read the entire file. */
    for(;;) {
      char buf2[4096];
      int n;

      if((n = fread(buf2, 1, sizeof(buf2), fp)) == 0)
        break;

      fwrite(buf2, 1, n, out);
    }

    if(fp != stdin)
      fclose(fp);

    /* Make sure that the last line is properly terminated with '\n'. */
    fflush(out);

    if(buflen == 0 || buf[buflen - 1] != '\n')
      fputc('\n', out);

    fputc('\0', out);
    fclose(out);
    return buf;
  }
}
