#include "../windoze.h"
#include "../unix.h"
#include "../sig.h"
#include "../errmsg.h"

#if 0 // WINDOWS_NATIVE
#include <windows.h>

static HANDLE timerHandle = INVALID_HANDLE_VALUE;

static VOID CALLBACK
timer_completion(LPVOID arg, DWORD timeLow, DWORD timeHigh) {
}

/*
 * Limitations of this implementation:
 *
 * - Does not support setting ovalue
 * - Does not support interval timer (value->it_interval)
 * - Only supports ITIMER_REAL
 */
int
setitimer(int which, const struct itimerval* value, struct itimerval* ovalue) {
  LARGE_INTEGER dueTime;

 assert(which == ITIMER_REAL);

  if(timerHandle == INVALID_HANDLE_VALUE) {
    /* First call in this backend, create new timer object */
    timerHandle = CreateWaitableTimer(NULL, TRUE, NULL);

    if(timerHandle == NULL)
      ereport(FATAL, (errmsg_internal("failed to create waitable timer: %i", GetLastError())));
  }

  if(value == NULL) {
    errno = EFAULT;
    return -1;
  }

  if(value->it_value.tv_sec == 0 && value->it_value.tv_usec == 0) {
    /* Turn timer off */
    CancelWaitableTimer(timerHandle);
    return 0;
  }

  /* Negative time to SetWaitableTimer means relative time */
  dueTime.QuadPart = -(value->it_value.tv_usec * 10 + value->it_value.tv_sec * 10000000L);

  /* Turn timer on, or change timer */

  if(!SetWaitableTimer(timerHandle, &dueTime, 0, timer_completion, NULL, FALSE))
    errmsg_warnsys("failed to set waitable timer: ",0);

  return 0;
}
#endif
