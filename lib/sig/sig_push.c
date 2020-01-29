/* ISC license. */

/* MT-unsafe */


int sig_push (int sig, sighandler_t_ref f)
{
  struct sigaction ssa = { f, SKASA_MASKALL | SKASA_NOCLDSTOP } ;
  return sig_pusha(sig, &ssa) ;
}
