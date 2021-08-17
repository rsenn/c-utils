#include "../windoze.h"

#if WINDOWS_NATIVE
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#endif

#include "../taia.h"
#include "../tai.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

void
taia_now(struct taia* t) {
#if WINDOWS_NATIVE
  LARGE_INTEGER ticks, freq;
  if(QueryPerformanceFrequency(&freq)) {
    if(QueryPerformanceCounter(&ticks)) {
      const int microseconds = 1000000;
      const int64 nanoseconds = microseconds * 1000;
      t->sec.x = ticks.QuadPart / freq.QuadPart;
      ticks.QuadPart %= freq.QuadPart;
      t->nano = ticks.QuadPart * nanoseconds / freq.QuadPart;
      t->atto = 0;
    }
  }
#elif 0
  GetSystemTimeAsFileTime(&fnord.f);
  /* 64-bit value representing the number of 100-nanosecond intervals
   * since January 1, 1601 (UTC) */
  fnord.l -= ((int64)(1970 - 1601)) * 365 * 24 * 60 * 60;
  t->sec.x = fnord.l / 10000000;
  t->nano = ((fnord.l + 5) / 10) % 1000000;
  t->atto = 0;
#elif defined(HAVE_CLOCK_GETTIME)
  struct timespec now = {0, 0};
  clock_gettime(CLOCK_REALTIME, &now);
  tai_unix(&t->sec, now.tv_sec);
  t->nano = now.tv_nsec;
  t->atto = 0;
#else
  struct timeval now;
  gettimeofday(&now, (struct timezone*)0);
  tai_unix(&t->sec, now.tv_sec);
  t->nano = (uint32)(1000 * now.tv_usec + 500);
  t->atto = 0;
#endif
}
