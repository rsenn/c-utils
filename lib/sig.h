/** @defgroup   sig @brief      SIG module.  @{ */
#ifndef _SIG_H
#define _SIG_H

#include <sys/types.h>
#include <signal.h>
#include "windoze.h"

#if WINDOWS_NATIVE
typedef long sigset_t;

#ifndef SIG_BLOCK
#define SIG_BLOCK 1
#endif /* defined(SIG_BLOCK) */
#ifndef SIG_UNBLOCK
#define SIG_UNBLOCK 2
#endif /* defined(SIG_UNBLOCK) */

#ifndef SIGALL
#define SIGALL (~(sigset_t)0L) /* All signals.    */
#endif

#ifndef sigbit
#define sigbit(n) (1L << ((n) - 1))
#endif
#ifndef sigemptyset
#define sigemptyset(s) *(s) = ~SIGALL
#endif
#ifndef sigfillset
#define sigfillset(s) *(s) = SIGALL
#endif

#ifndef sigaddset
#define sigaddset(s, n) *(s) |= sigbit(n)
#endif
#ifndef sigdelset
#define sigdelset(s, n) *(s) &= ~sigbit(n)
#endif
#ifndef sigismember
#define sigismember(s, n) (*(s) & sigbit(n))
#endif
#include <errno.h>
#ifndef ENOBUFS
#define ENOBUFS 1039
#endif

#else

#include "byte.h"
#include <sys/signal.h>

#define __sigmask(sig) (((unsigned long)1) << (((sig) - 1) % (8 * sizeof(unsigned long))))
#define __sigword(sig) (((sig) - 1) / (8 * sizeof(unsigned long)))

#ifndef sigemptyset
#define sigemptyset(s) byte_zero((s), sizeof(*(s)))
#endif
#ifndef sigfillset
#define sigfillset(s) byte_fill((s), sizeof(*(s)), 0xff)
#endif
#ifndef sigaddset
#define sigaddset(s, n) (((unsigned long*)(s))[__sigword((n))] |= __sigmask((n)))
#endif
#ifndef sigdelset
#define sigdelset(s, n) (((unsigned long*)(s))[__sigword((n))] &= ~__sigmask((n)))
#endif
#ifndef sigismember
#define sigismember(s, n) ((((unsigned long*)(s))[__sigword((n))] & __sigmask((n))) ? 1 : 0)
#endif

#endif

typedef void sighandler_t_fn(int);
typedef sighandler_t_fn* sighandler_t_ref;

#if WINDOWS_NATIVE
struct sigaction {
  sighandler_t_ref sa_handler;
  unsigned int sa_flags : 2;
};
#endif

extern struct sigaction const sig_dfl;
extern struct sigaction const sig_ign;

#define SIGSTACKSIZE 16

#define sig_catcha(sig, ac) sig_action(sig, (ac), 0)
#define sig_restore(sig) sig_action((sig), &sig_dfl, 0)

/* sig_action is a thin, honest wrapper around sigaction(2): `new` and `old`
 * are real `struct sigaction` (on POSIX), passed through untouched -- every
 * field (sa_mask, sa_flags including SA_SIGINFO/SA_ONSTACK/..., sa_sigaction)
 * is under the caller's control, nothing is second-guessed or forced. */
int sig_action(int sig, struct sigaction const* new, struct sigaction* old);
int sig_block(int);
int sig_blocknone(void);
int sig_blockset(const void*);
int sig_catch(int, sighandler_t_ref);
int sigfpe(void);
int sig_ignore(int);
char const* sig_name(int);
int sig_number(char const*);
void sig_pause(void);
int sig_pop(int sig);
int sig_push(int, sighandler_t_ref);
int sig_pusha(int sig, struct sigaction const* ssa);
void sig_restoreto(const sigset_t*, unsigned int);
int sig_unblock(int);
int sigsegv(void);
int sig_shield(void);
int sig_unshield(void);

#endif
/** @} */
