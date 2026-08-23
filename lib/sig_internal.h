#ifndef SIG_INTERNAL_H
#define SIG_INTERNAL_H

#include "sig.h"

/* lib/sig deliberately covers only the standard POSIX signals (1..NSIG-1),
 * not the SIGRTMIN..SIGRTMAX real-time range: sig_push/sig_pop size their
 * per-signal handler stacks off NSIG, and following SIGRTMAX here (which
 * varies by platform/kernel, commonly ~64 signals wide on Linux) would blow
 * that up to roughly double the storage for a range this module doesn't
 * otherwise support. If real-time signals are ever needed, sig_pusha's
 * range check (`sig >= NSIG`) is the first thing that would need revisiting. */
#ifndef NSIG
#define NSIG 65
#endif

typedef struct sigtable_s sigtable_t, *sigtable_t_ref;
struct sigtable_s {
  int number;
  char const* name;
};

extern sigtable_t const sigtable[];

#endif
