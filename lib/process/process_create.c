#include "../windoze.h"
#include "../io.h"
#include "../strlist.h"

#if WINDOWS_NATIVE
#include <windows.h>
#include <process.h>
#include <io.h>
#endif

#define PROCESS_STATUS_ERROR     -1       // process has entered an erroneous state


int
process_create(const char* filename, const char* argv[]) {
#if WINDOWS_NATIVE
  size_t i;
  strlist joined_argv;
  fd_t pipes[3][2];

  PROCESS_INFORMATION piProcessInfo;
  int status;

  STARTUPINFOA siStartInfo;

  SECURITY_ATTRIBUTES saAttr;
  BOOL retval = FALSE;

  strlist_init(&joined_argv, ' ');

  for(i = 0; argv[i]; ++i) {
    strlist_push(&joined_argv, argv[i]);
  }
  stralloc_nul(&joined_argv.sa);

  saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
  saAttr.bInheritHandle = TRUE;
  /* TODO: should we set explicit security attributes? (#2046, comment 5) */
  saAttr.lpSecurityDescriptor = NULL;

  /* Assume failure to start process */
  status = PROCESS_STATUS_ERROR;

  io_pipe(pipes[0]);
  io_pipe(pipes[1]);
  io_pipe(pipes[2]);

  ZeroMemory(&piProcessInfo, sizeof(PROCESS_INFORMATION));
  ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));

  siStartInfo.cb = sizeof(STARTUPINFO);
  siStartInfo.hStdError = _get_osfhandle(pipes[2][1]);
  siStartInfo.hStdOutput = _get_osfhandle(pipes[1][1]);
  siStartInfo.hStdInput = _get_osfhandle(pipes[0][0]);
  siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

  /* Create the child process */

  retval = CreateProcessA(filename,
                          joined_argv.sa.s, 
                          &saAttr,          // process security attributes
                          NULL,             // primary thread security attributes
                          TRUE,             // handles are inherited
                      /*(TODO: set CREATE_NEW CONSOLE/PROCESS_GROUP to make GetExitCodeProcess() work?) */
                          CREATE_NO_WINDOW, // creation flags
                          NULL,
                          NULL,            // use parent's current directory
                          &siStartInfo,    // STARTUPINFO pointer
                          &piProcessInfo); // receives PROCESS_INFORMATION
#else


#endif
}
