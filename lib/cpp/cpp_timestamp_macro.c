#include "../cpp.h"
#include "../cpp_internal.h"
#include <sys/stat.h>

/* "Fri Jul 24 01:32:50 2020" */
cpp_token*
cpp_timestamp_macro(cpp_token* tmpl) {
  struct stat st;

  if(stat(tmpl->file->name, &st) != 0)
    return cpp_new_str_token("??? ??? ?? ??:??:?? ????", tmpl);

  char buf[30];
  ctime_r(&st.st_mtime, buf);
  buf[24] = '\0';
  return cpp_new_str_token(buf, tmpl);
}
