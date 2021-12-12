/**
 * @defgroup   errmsg
 * @brief      ERRMSG module.
 * @{
 */
/* this header file comes from libowfat, http://www.fefe.de/libowfat/ */
#ifndef ERRMSG_H
#define ERRMSG_H

/* for exit(): */
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* These use file descriptor 2, not buffer_2!
 * Call buffer_flush(buffer_2) before calling these! */

extern const char* errmsg_argv0;

const char* errmsg_iam(const char* who); /* set errmsg_argv0 */

/* terminate with NULL. */
/* newline is appended automatically. */
void errmsg_warn(const char* message, ...);
void errmsg_warnsys(const char* message, ...);

void errmsg_info(const char* message, ...);
void errmsg_infosys(const char* message, ...);

#if !defined(__BORLANDC__) && !defined(__LCC__)
#define carp(...) errmsg_warn(__VA_ARGS__, (char*)0)
#define carpsys(...) errmsg_warnsys(__VA_ARGS__, (char*)0)
#define die(n, ...) \
  do { \
    errmsg_warn(__VA_ARGS__, (char*)0); \
    exit(n); \
  } while(0)
#define diesys(n, ...) \
  do { \
    errmsg_warnsys(__VA_ARGS__, (char*)0); \
    exit(n); \
  } while(0)
#define msg(...) errmsg_info(__VA_ARGS__, (char*)0);
#define msgsys(...) errmsg_infosys(__VA_ARGS__, (char*)0);
#endif

#ifdef __cplusplus
}
#endif

#endif
/** @} */
