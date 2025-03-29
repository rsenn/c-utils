#include "../cpp.h"
#include "../cpp_internal.h"

// __TIME__ is expanded to the current time, e.g. "13:34:03".
char*
cpp_format_time(struct tm* tm) {
  return cpp_format("\"%02d:%02d:%02d\"", tm->tm_hour, tm->tm_min, tm->tm_sec);
}
