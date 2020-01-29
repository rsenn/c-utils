#ifndef _SIG_H
#define _SIG_H

#include <sys/types.h>
#include <signal.h>

typedef void sighandler_t_fn(int);
typedef sighandler_t_fn* sighandler_t_ref;

#if !defined(_POSIX_SOURCE) && !defined(__linux__) && !defined(__unix__)
struct sigaction {
  sighandler_t_ref sa_handler;
  unsigned int sa_flags : 2;
};

extern struct sigaction const SIG_DFL;
extern struct sigaction const SIG_IGN;
#endif

#ifndef SA_MASKALL
#define SA_MASKALL 0x01
#endif
#ifndef SA_NOCLDSTOP
#define SA_NOCLDSTOP 0x02
#endif

#define SIGSTACKSIZE 16

int sigaction(int sig, struct sigaction const* new, struct sigaction* old);
void sig_blocknone(void);
void sig_blockset(sigset_t const* set);
void sig_block(void);
int sig_catch(int sig, sighandler_t_ref f);
int sigfpe(void);
char const* sig_name(int sig);
int sig_number(char const* name);
void sig_pause(void);
int sig_pop(int sig);
int sig_pusha(int sig, struct sigaction const* ssa);
int sig_push(int sig, sighandler_t_ref f);
void sig_restoreto(sigset_t const* set, unsigned int n);
int sigsegv(void);
void sig_shield(void);
void sig_unshield(void);

#endif
