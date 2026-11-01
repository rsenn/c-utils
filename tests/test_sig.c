#include "unit_test.h"
#include "../lib/sig.h"
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

static volatile sig_atomic_t got_usr1, got_usr2, got_alrm, got_segv, got_fpe;

static void
on_usr1(int s) {
  (void)s;
  ++got_usr1;
}
static void
on_usr2(int s) {
  (void)s;
  ++got_usr2;
}
static void
on_alrm(int s) {
  (void)s;
  ++got_alrm;
}
static void
on_segv(int s) {
  (void)s;
  ++got_segv;
}
static void
on_fpe(int s) {
  (void)s;
  ++got_fpe;
}

/*
 * const char* sig_name(int); int sig_number(const char*);
 */
TEST(test_sig_name_number) {
  ASSERT_STR_EQUAL("USR1", sig_name(SIGUSR1));
  ASSERT_STR_EQUAL("TERM", sig_name(SIGTERM));
  ASSERT_STR_EQUAL("???", sig_name(9999));
  ASSERT_EQ(SIGINT, sig_number("INT"));
  ASSERT_EQ(SIGINT, sig_number("int")); /* case insensitive */
  ASSERT_EQ(0, sig_number("NOSUCHSIG"));
}

/*
 * int sig_catch(int, handler); int sig_restore via sig_action; sig_dfl/sig_ign
 */
TEST(test_sig_catch_restore) {
  got_usr1 = 0;
  ASSERT_EQ(0, sig_catch(SIGUSR1, on_usr1));
  raise(SIGUSR1);
  ASSERT_EQ(1, got_usr1);
  raise(SIGUSR1);
  ASSERT_EQ(2, got_usr1);
  ASSERT_EQ(0, sig_restore(SIGUSR1));
  sig_ignore(SIGUSR1);
  raise(SIGUSR1); /* ignored, the process survives */
  ASSERT_EQ(2, got_usr1);
  sig_restore(SIGUSR1);
}

/*
 * int sig_action(int, const struct sigaction*, struct sigaction* old);
 */
TEST(test_sig_action) {
  struct sigaction sa, old;

  memset(&sa, 0, sizeof sa);
  sa.sa_handler = on_usr2;
  sigemptyset(&sa.sa_mask);
  ASSERT_EQ(0, sig_action(SIGUSR2, &sa, 0));
  ASSERT_EQ(0, sig_action(SIGUSR2, &sig_dfl, &old)); /* old receives the previous action */
  ASSERT_EQ(1, old.sa_handler == on_usr2);
  ASSERT_EQ(1, sig_action(-5, &sa, 0) == -1);
}

/*
 * int sig_block(int); int sig_unblock(int); int sig_blocknone(void);
 * int sig_blockset(const void*): a blocked signal stays pending until unblocked
 */
TEST(test_sig_block) {
  sigset_t set;

  got_usr1 = 0;
  sig_catch(SIGUSR1, on_usr1);
  ASSERT_EQ(0, sig_block(SIGUSR1));
  raise(SIGUSR1);
  ASSERT_EQ(0, got_usr1); /* pending */
  sigpending(&set);
  ASSERT_EQ(1, sigismember(&set, SIGUSR1));
  ASSERT_EQ(0, sig_unblock(SIGUSR1));
  ASSERT_EQ(1, got_usr1); /* delivered on unblock */

  sig_block(SIGUSR1);
  ASSERT_EQ(0, sig_blocknone()); /* clears the whole mask */
  raise(SIGUSR1);
  ASSERT_EQ(2, got_usr1);

  sigemptyset(&set);
  sigaddset(&set, SIGUSR1);
  ASSERT_EQ(0, sig_blockset(&set)); /* the mask is exactly this set */
  raise(SIGUSR1);
  ASSERT_EQ(2, got_usr1);
  sig_blocknone();
  ASSERT_EQ(3, got_usr1);
  sig_restore(SIGUSR1);
}

/*
 * int sig_shield(void); int sig_unshield(void): block/unblock the terminating signals
 */
TEST(test_sig_shield) {
  sigset_t cur;

  ASSERT_EQ(0, sig_shield());
  sigprocmask(SIG_BLOCK, NULL, &cur);
  ASSERT_EQ(1, sigismember(&cur, SIGTERM));
  ASSERT_EQ(1, sigismember(&cur, SIGINT));
  ASSERT_EQ(0, sig_unshield());
  sigprocmask(SIG_BLOCK, NULL, &cur);
  ASSERT_EQ(0, sigismember(&cur, SIGTERM));
  ASSERT_EQ(0, sigismember(&cur, SIGINT));
}

/*
 * int sig_push(int, handler); int sig_pop(int); int sig_pusha(int, const struct sigaction*)
 */
TEST(test_sig_push_pop) {
  got_usr1 = got_usr2 = 0;
  ASSERT_EQ(0, sig_catch(SIGUSR1, on_usr1));
  ASSERT_EQ(1, sig_push(SIGUSR1, on_usr2)); /* the new depth */
  raise(SIGUSR1);
  ASSERT_EQ(1, got_usr2);
  ASSERT_EQ(0, got_usr1);
  ASSERT_EQ(0, sig_pop(SIGUSR1));           /* back to on_usr1 */
  raise(SIGUSR1);
  ASSERT_EQ(1, got_usr1);
  ASSERT_EQ(-1, sig_pop(SIGUSR1));          /* nothing left on the stack */
  ASSERT_EQ(EFAULT, errno);
  ASSERT_EQ(-1, sig_push(-1, on_usr1));
  ASSERT_EQ(EINVAL, errno);
  sig_restore(SIGUSR1);
}

/*
 * void sig_pause(void): returns after a signal was handled
 */
TEST(test_sig_pause) {
  struct itimerval it;

  got_alrm = 0;
  sig_catch(SIGALRM, on_alrm);
  memset(&it, 0, sizeof it);
  it.it_value.tv_usec = 20000;
  setitimer(ITIMER_REAL, &it, 0);
  sig_pause();
  ASSERT_EQ(1, got_alrm);
  sig_restore(SIGALRM);
}

/*
 * void sig_restoreto(const sigset_t*, unsigned int n): restore the default for all in the set
 */
TEST(test_sig_restoreto) {
  sigset_t set;
  struct sigaction cur;

  sigemptyset(&set);
  sigaddset(&set, SIGUSR1);
  sig_catch(SIGUSR1, on_usr1);
  sig_restoreto(&set, SIGUSR2);
  sigaction(SIGUSR1, NULL, &cur);
  ASSERT_EQ(1, cur.sa_handler == SIG_DFL);
}

/*
 * int sigsegv(void); int sigfpe(void): raise the signal
 */
TEST(test_sig_sigsegv_sigfpe) {
  got_segv = got_fpe = 0;
  sig_catch(SIGSEGV, on_segv);
  sig_catch(SIGFPE, on_fpe);
  ASSERT_EQ(1, sigsegv());
  ASSERT_EQ(1, got_segv);
  ASSERT_EQ(1, sigfpe());
  ASSERT_EQ(1, got_fpe);
  sig_restore(SIGSEGV);
  sig_restore(SIGFPE);
}

#define RUN_SIG_TESTS() \
  RUN(test_sig_name_number); \
  RUN(test_sig_catch_restore); \
  RUN(test_sig_action); \
  RUN(test_sig_block); \
  RUN(test_sig_shield); \
  RUN(test_sig_push_pop); \
  RUN(test_sig_pause); \
  RUN(test_sig_restoreto); \
  RUN(test_sig_sigsegv_sigfpe)

TESTS(sig) { RUN_SIG_TESTS(); }
