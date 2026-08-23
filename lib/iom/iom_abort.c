#include "../io_internal.h"
#include "../iom.h"

int
iom_abort(iomux_t* c) {
  c->working = -2;
#if WINDOWS_NATIVE
  return ReleaseSemaphore(c->sem, 1, 0) ? 0 : -1;
#elif defined(__dietlibc__)
  return cnd_broadcast(&c->sem);
#else
  return sem_post(&c->sem);
#endif
}
