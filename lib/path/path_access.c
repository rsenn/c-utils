#include "../path.h"

#if WINDOWS_NATIVE
#include <windows.h>

BOOL
hasAccessRight(LPCSTR path, DWORD genericAccessRights) {
  SECURITY_DESCRIPTOR secDesc;

  DWORD len = 0;
  if(GetFileSecurityA(
         path, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION, NULL, NULL, &len) ==
     FALSE) {
    MMBLOG_FILE_FUNC_LINE(CRITICAL, "Failed to get security information for file " << path << std::endl);
  }

  SecurityDescriptorWrapper sd{len};
  if(GetFileSecurityA(path,
                      OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION,
                      sd.get(),
                      len,
                      &len) == FALSE) {
    MMBLOG_FILE_FUNC_LINE(CRITICAL, "Failed to get security information for file " << path << std::endl);
  }

  HANDLE hToken = NULL;
  HANDLE hImpersonatedToken = NULL;
  if(OpenProcessToken(GetCurrentProcess(),
                      TOKEN_IMPERSONATE | TOKEN_QUERY | TOKEN_DUPLICATE | STANDARD_RIGHTS_READ,
                      &hToken) == FALSE) {
    MMBLOG_FILE_FUNC_LINE(CRITICAL, "Failed to get security information for file " << path << std::endl);
  }

  if(DuplicateToken(hToken, SecurityImpersonation, &hImpersonatedToken) == FALSE) {
    CloseHandle(hToken);
    MMBLOG_FILE_FUNC_LINE(CRITICAL, "Failed to get security information for file " << path << std::endl);
  }

  GENERIC_MAPPING mapping = {0xFFFFFFFF};
  PRIVILEGE_SET privileges = {0};
  DWORD grantedAccess = 0;
  DWORD privilegesLen = sizeof(privileges);

  mapping.GenericRead = FILE_GENERIC_READ;
  mapping.GenericWrite = FILE_GENERIC_WRITE;
  mapping.GenericExecute = FILE_GENERIC_EXECUTE;
  mapping.GenericAll = FILE_ALL_ACCESS;
  BOOL result = FALSE;

  MapGenericMask(&genericAccessRights, &mapping);
  auto success = AccessCheck(sd.get(),
                             hImpersonatedToken,
                             genericAccessRights,
                             &mapping,
                             &privileges,
                             &privilegesLen,
                             &grantedAccess,
                             &result);
  CloseHandle(hImpersonatedToken);
  CloseHandle(hToken);

  if(success == FALSE)
    MMBLOG_FILE_FUNC_LINE(CRITICAL, "Failed to get security information for file " << path << std::endl);

  return result == TRUE;
}

static inline bool
hasReadAccess(LPCSTR path) {
  return hasAccessRight(path, GENERIC_READ);
}

static inline bool
hasWriteAccess(LPCSTR path) {
  return hasAccessRight(path, GENERIC_READ | GENERIC_WRITE);
}

static inline bool
hasExecuteAccess(LPCSTR path) {
  return hasAccessRight(path, GENERIC_EXECUTE);
}
#endif

int
path_access(const char* path, int rights) {
#if WINDOWS_NATIVE
  switch(rights & (R_OK | W_OK | X_OK)) {
    case R_OK: return hasReadAccess(path);
    case R_OK | W_OK:
    case W_OK: return hasWriteAccess(path);

    case X_OK: return hasExecuteAccess(path);
    case X_OK | R_OK: return hasExecuteAccess(path) && hasReadAccess(path);
    case X_OK | W_OK: return hasExecuteAccess(path) && hasWriteAccess(path);
    case X_OK | R_OK | W_OK: return hasExecuteAccess(path) && hasReadAccess(path) && hasWriteAccess(path);
  }
#else
  if(access(path, rights) == 0)
    return 1;
#endif
  return 0;
}
