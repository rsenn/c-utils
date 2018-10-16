#ifndef WAIT_H
#define WAIT_H

#ifdef __cplusplus
extern "C" {
#endif

int          waitn_reap(int* pids, unsigned int len);
int          waitn(int* pids, unsigned int n);
int          waitpid_nointr(int pid, int* wstat, int flags);
int          wait_nohang(int* wstat);
int          wait_nointr(int* wstat);
int          wait_pids_nohang(int const* pids, unsigned int len, int* wstat);
int          wait_pid_nohang(int pid, int* wstat);
unsigned int wait_reap(void);

#ifdef __cplusplus
}
#endif

#endif // WAIT_H

