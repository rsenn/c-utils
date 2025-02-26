#include "../errmsg.h"

const char* errmsg_argv0;

const char*
errmsg_iam(const char* who) {
  const char* s = errmsg_argv0;

  if(who == NULL)
    return s;
  errmsg_argv0 = who;
  return s;
}
