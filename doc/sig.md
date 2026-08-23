# sig

`lib/sig.h` + `lib/sig/*.c` -- signal handling helpers modeled on djb-style
signal action/block primitives (see `lib/sig.h` in `README.txt`). One
function per file under `lib/sig/`, following the module convention
described in `CLAUDE.md`.

Everything degrades to a no-op on `WINDOWS_NATIVE` builds unless noted
otherwise; `sig.h` supplies `sigset_t`/`sigaction`/`sigemptyset`-family
shims for that case.

## Installing and removing handlers

```c
int sig_action(int sig, struct sigaction const* new, struct sigaction* old);
int sig_catch(int sig, sighandler_t_ref f);
int sig_ignore(int sig);
```

`sig_action` is the common core: it installs `sigaction(2)`-style behavior
for `sig` from a simplified `struct sigaction` (just `sa_handler` and a
2-bit `sa_flags` of `SA_MASKALL`/`SA_NOCLDSTOP`), always adding
`SA_RESTART` where available, and optionally returns the previous
disposition in `old`.

`sig_catch(sig, f)` is the common case: install handler `f` for `sig` with
`SA_MASKALL | SA_NOCLDSTOP`. `sig_ignore(sig)` sets the disposition to
`SIG_IGN`. Both are MT-unsafe.

Two macros built on `sig_action`:

```c
#define sig_catcha(sig, ac) sig_action(sig, (ac), 0)
#define sig_restore(sig)    sig_action((sig), &sig_dfl, 0)
```

`sig_catcha` installs a full `struct sigaction const*` directly.
`sig_restore(sig)` puts `sig` back to its default disposition, using the
predefined constants:

```c
extern struct sigaction const sig_dfl; /* SIG_DFL */
extern struct sigaction const sig_ign; /* SIG_IGN */
```

## Handler stack

```c
int sig_push(int sig, sighandler_t_ref f);
int sig_pusha(int sig, struct sigaction const* ssa);
int sig_pop(int sig);
```

`sig_push`/`sig_pusha` install a new handler for `sig` while saving the
previous disposition on a fixed-depth per-signal stack (`SIGSTACKSIZE` =
16 entries, `sig.h`). `sig_pop` undoes the most recent push, restoring the
disposition that was active before it. Both directions are MT-unsafe.
Returns the resulting stack depth on success, -1 on failure (`errno` is
`EINVAL` for an out-of-range `sig`, `EFAULT` from `sig_pop` on an empty
stack, `ENOBUFS` from `sig_push`/`sig_pusha` when the stack is full).

```c
void sig_restoreto(const sigset_t* ss, unsigned int n);
```

Bulk helper: for every signal number `1..n` present in `ss`, calls
`sig_restore` on it (restores default disposition). Typically used with a
mask captured earlier (e.g. from `sigprocmask`) to reset everything that
was touched.

## Blocking

```c
void sig_block(int sig);
void sig_unblock(int sig);
void sig_blocknone(void);
void sig_blockset(const void* set);
```

`sig_block`/`sig_unblock` add/remove a single signal from the process
signal mask via `sigprocmask(SIG_BLOCK/SIG_UNBLOCK, ...)`.
`sig_blocknone` clears the mask entirely (`SIG_SETMASK` to the empty set).
`sig_blockset` replaces the mask outright with the given `sigset_t*`.

```c
void sig_shield(void);
void sig_unshield(void);
```

Convenience pair that block/unblock a fixed set of signals in one call --
`SIGTERM`, `SIGQUIT`, `SIGABRT`, `SIGINT`, `SIGPIPE`, `SIGHUP` -- typically
used to protect a critical section from being interrupted.

```c
void sig_pause(void);
```

Blocks until a signal is delivered (`sigsuspend` with an empty mask).

## Name/number lookup

```c
char const* sig_name(int sig);
int sig_number(char const* name);
```

Translate between a signal number and its short name (`"INT"`, `"SEGV"`,
`"USR1"`, ...) via the `sigtable[]` array built in `lib/sig/sig_table.c`
from whichever `SIG*` macros the platform defines. `sig_name` returns
`"???"` for an unknown number; `sig_number` returns 0 for an unknown name
(name lookup is case-insensitive, via `case_diffs`).

## Test helpers

```c
int sigfpe(void);
int sigsegv(void);
```

Raise `SIGFPE`/`SIGSEGV` against the current process (`raise(2)`); return
nonzero on success. Useful for exercising a handler installed with
`sig_catch`/`sig_push`.

## See also

Man pages for each function live alongside the source in `lib/sig/*.3`
(e.g. `lib/sig/sig_push.3`).
