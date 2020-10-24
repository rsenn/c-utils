#ifndef PROCESS_H
#define PROCESS_H

#include "typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

int process_create(const char* filename, const char* argv[], fd_t std[3], const char* cwd);

#ifdef __cplusplus
}
#endif
#endif /* defined(PROCESS_H) */
