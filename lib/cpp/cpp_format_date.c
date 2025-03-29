#include "../cpp.h"
#include "../cpp_internal.h"

/* __DATE__ is expanded to the current date, e.g. "May 17 2020". */
char*
cpp_format_date(struct tm* tm) {
  static char mon[][4] = {
      "Jan",
      "Feb",
      "Mar",
      "Apr",
      "May",
      "Jun",
      "Jul",
      "Aug",
      "Sep",
      "Oct",
      "Nov",
      "Dec",
  };

  return cpp_format("\"%s %2d %d\"", mon[tm->tm_mon], tm->tm_mday, tm->tm_year + 1900);
}
