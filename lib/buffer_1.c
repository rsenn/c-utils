#ifndef _WIN32
#include <unistd.h>
#else
#include <io.h>
#endif
#include "buffer.h"
#ifdef _WIN32
#include <io.h>
#endif

char buffer_1_space[BUFFER_INSIZE];
static buffer it = BUFFER_INIT(write,1,buffer_1_space,sizeof buffer_1_space);
buffer *buffer_1 = &it;

