#include "../windoze.h"
#include "../byte.h"
#include "../io.h"
#include "../strlist.h"
#include "../uint64.h"
#include "../env.h"
#include "../errmsg.h"
#include "../wait.h"
#include "../str.h"
#include "../uint32.h"

#if WINDOWS_NATIVE
#include <windows.h>
#include <process.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#if POSIX_SPAWN
#include <spawn.h>
#endif

#if WINDOWS_NATIVE
#define PROCESS_STATUS_ERROR -1 // process has entered an erroneous state

static const char*
last_error_str() {
  DWORD errCode = GetLastError();
  static char tmpbuf[1024];
  DWORD len;
  char* err = 0;
  tmpbuf[0] = '\0';

  if(errCode == 0)
    return tmpbuf;

  SetLastError(0);

  if(!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                    NULL,
                    errCode,
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), /* default language */
                    (LPTSTR)&err,
                    0,
                    NULL))
    return 0;

  byte_copy(tmpbuf, 7, "ERROR: ");

  len = strlen(err);

  if(len > sizeof(tmpbuf) - 8)
    len = sizeof(tmpbuf) - 8;

  byte_copy(tmpbuf + 7, len, err);

  LocalFree(err);
  return tmpbuf;
}
#endif

int
process_create(const char* filename, char* const argv[], fd_type std[3], const char* cwd) {
  fd_type fds[3];
  int32 pid;
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
#if POSIX_SPAWN
  {
    posix_spawn_file_actions_t actions;
    posix_spawnattr_t attr;

    posix_spawnattr_setflags(&attr, 0);

    if(std) {
      posix_spawn_file_actions_init(&actions);
      posix_spawn_file_actions_adddup2(&actions, std[0], 0);
      posix_spawn_file_actions_adddup2(&actions, std[1], 1);
      posix_spawn_file_actions_adddup2(&actions, std[2], 2);
    }

    if(posix_spawnp(&pid, filename, std ? &actions : 0, &attr, argv, NULL)) {
      errmsg_warnsys("execvpe error: ", 0);
      exit(1);
    }
  }
#ifdef DEBUG_OUTPUT_
  buffer_putm_internal(buffer_2, "Spawned '", filename, "' PID = ", NULL);
  buffer_putlong(buffer_2, pid);
  buffer_putsflush(buffer_2, "\n");
#endif

#elif WINDOWS_NATIVE
  {
    size_t i;
    int status;
    stralloc joined_argv;
    PROCESS_INFORMATION piProcessInfo; /* receives PROCESS_INFORMATION */
    STARTUPINFOA siStartInfo;          /* STARTUPINFO pointer */
    SECURITY_ATTRIBUTES saAttr;        /* process security attributes */
    BOOL retval = FALSE;

    stralloc_init(&joined_argv);

    for(i = 0; argv[i]; ++i) {
      const char* arg = argv[i];
      int quote = !!arg[str_chr(argv[i], ' ')];

      if(joined_argv.len)
        stralloc_catc(&joined_argv, ' ');

      if(quote)
        stralloc_catc(&joined_argv, '"');

      stralloc_cats(&joined_argv, arg);

      if(quote)
        stralloc_catc(&joined_argv, '"');
    }

    stralloc_nul(&joined_argv);

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    /* TODO: should we set explicit security attributes? (#2046, comment 5)
     */
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
                            &saAttr,
                            NULL, /* primary thread security attributes */
                            TRUE, /* handles are inherited */
                            /*(TODO: set CREATE_NEW CONSOLE/PROCESS_GROUP
                               to make GetExitCodeProcess() work?) */
                            CREATE_NO_WINDOW,
                            NULL,
                            cwd,
                            &siStartInfo,
                            &piProcessInfo);

    if(retval == FALSE) {
      int error = GetLastError();

      buffer_puts(buffer_2, last_error_str());
      buffer_putnlflush(buffer_2);

      pid = -1;
    } else {
      pid = piProcessInfo.dwProcessId;
    }
  }
#else
  {
    int status;

    if((pid = fork()) == 0) {
      fd_type i;

      for(i = 0; i <= 2; ++i) {
        if(fds[i] != i) {
          dup2(fds[i], i);
          close(fds[i]);
        }
      }

      if(execvp(filename, (char* const*)argv) == -1)
        errmsg_warn("execve failed: ", 0);
      exit(127);
    }
  }
#endif
  return pid;
}
