#ifndef _WIN32
#include <unistd.h>
#endif
#include "buffer.h"
#if defined(__MINGW32__) || defined(_WIN32) || defined(__MINGW64__) || defined(_WIN64)
#include <io.h>
#endif

char buffer_1_space[BUFFER_INSIZE];
static buffer it = BUFFER_INIT((ssize_t(*)())write, 1, buffer_1_space, sizeof buffer_1_space);
buffer *buffer_1 = &it;

