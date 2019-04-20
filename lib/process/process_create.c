#include "../windoze.h"
#include "../io.h"
#include "../strlist.h"
#include "../uint64.h"
#include "../env.h"
#include "../errmsg.h"
#include "../wait.h"

#if WINDOWS_NATIVE
#include <windows.h>
#include <process.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#define PROCESS_STATUS_ERROR     -1       // process has entered an erroneous state


int
process_create(const char* filename, const char* argv[], fd_t std[3], const char* cwd) {
  fd_t fds[3];
  int64 pid;
  int status = 0;
  
  if(std) {
    fds[0] = std[0];
    fds[1] = std[1];
    fds[2] = std[2];
  } else {
    fds[0] = 0;
    fds[1] = 1;
    fds[2] = 2;
  }

#if WINDOWS_NATIVE
  {
    size_t i;
    stralloc joined_argv;

    PROCESS_INFORMATION piProcessInfo;
    int status;
    STARTUPINFOA siStartInfo;

    SECURITY_ATTRIBUTES saAttr;
    BOOL retval = FALSE;

    stralloc_init(&joined_argv);

    for(i = 0; argv[i]; ++i) {
      const char* arg = argv[i];
      int quote = !!arg[str_chr(argv[i], ' ')];

      if(joined_argv.len)
        stralloc_catc(&joined_argv, ' ');

      if(quote) stralloc_catc(&joined_argv, '"');
      stralloc_cats(&joined_argv, arg);
      if(quote) stralloc_catc(&joined_argv, '"');
    }
    stralloc_nul(&joined_argv);

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    /* TODO: should we set explicit security attributes? (#2046, comment 5) */
    saAttr.lpSecurityDescriptor = NULL;

    /* Assume failure to start process */
    status = PROCESS_STATUS_ERROR;

    ZeroMemory(&piProcessInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));

    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = (HANDLE)_get_osfhandle(fds[2]);
    siStartInfo.hStdOutput = (HANDLE)_get_osfhandle(fds[1]);
    siStartInfo.hStdInput = (HANDLE)_get_osfhandle(fds[0]);
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    /* Create the child process */

    retval = CreateProcessA(filename,
                            joined_argv.s, 
                            &saAttr,          // process security attributes
                            NULL,             // primary thread security attributes
                            TRUE,             // handles are inherited
                        /*(TODO: set CREATE_NEW CONSOLE/PROCESS_GROUP to make GetExitCodeProcess() work?) */
                            CREATE_NO_WINDOW, // creation flags
                            NULL,
                            cwd,             // use parent's current directory
                            &siStartInfo,    // STARTUPINFO pointer
                            &piProcessInfo); // receives PROCESS_INFORMATION

    if(retval != FALSE) {
      pid = piProcessInfo.dwProcessId;
    }
  }
#else
  {
    int status;


    if((pid = fork()) == 0) {
      fd_t i;
      for(i = 0; i <= 2; ++i) {
        if(fds[i] != i) {
          dup2(fds[i], i);
          close(fds[i]);
        }
      }

      if(execve(filename, argv, environ) == -1)
        errmsg_warn("execve failed: ", 0);
      exit(127);
    }


  }
#endif
  return wait_pid_nohang(pid, &status) == 0 ? pid : -1;
}
