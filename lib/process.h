/**
 * @defgroup   process
 * @brief      PROCESS module.
 * @{
 */
#ifndef PROCESS_H
#define PROCESS_H

#include "typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

int process_create(const char* filename,
                   char* const argv[],
                   fd_t std[3],
                   const char* cwd);
int process_wait(int);

#ifdef __cplusplus
}
#endif
#endif /* defined(PROCESS_H) */
/** @} */
