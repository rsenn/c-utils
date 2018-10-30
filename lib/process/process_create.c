#include "../io.h"

int
process_create() {

  fd_t pipes[3][2];

  PROCESS_INFORMATION piProcessInfo;
  int status;

  STARTUPINFOA siStartInfo;

  SECURITY_ATTRIBUTES saAttr;
  BOOL retval = FALSE;

  SECURITY_ATTRIBUTES saAttr;

  ;

  saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
  saAttr.bInheritHandle = TRUE;
  /* TODO: should we set explicit security attributes? (#2046, comment 5) */
  saAttr.lpSecurityDescriptor = NULL;

  /* Assume failure to start process */
  status = PROCESS_STATUS_ERROR;

  io_pipe(&pipes[0]);
  io_pipe(&pipes[1]);
  io_pipe(&pipes[2]);

  ZeroMemory(&piProcessInfo, sizeof(PROCESS_INFORMATION));
  ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));

  siStartInfo.cb = sizeof(STARTUPINFO);
  siStartInfo.hStdError = _get_osfhandle(pipes[2][1]);
  siStartInfo.hStdOutput = _get_osfhandle(pipes[1][1]);
  siStartInfo.hStdInput = _get_osfhandle(pipes[2][0]);
  siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

  /* Create the child process */

  retval = CreateProcessA(filename,         // module name
                          joined_argv,      // command line
                                            /* TODO: should we set explicit security attributes? (#2046, comment 5) */
                          &saAttr,          // process security attributes
                          NULL,             // primary thread security attributes
                          TRUE,             // handles are inherited
                                            /*(TODO: set CREATE_NEW CONSOLE/PROCESS_GROUP to make GetExitCodeProcess()
                                             * work?) */
                          CREATE_NO_WINDOW, // creation flags
                          NULL,
                          NULL,            // use parent's current directory
                          &siStartInfo,    // STARTUPINFO pointer
                          &piProcessInfo); // receives PROCESS_INFORMATION
}