#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif
#include "buffer.h"

char buffer_2_space[BUFFER_INSIZE];
static buffer it = BUFFER_INIT((ssize_t(*)())write,2,buffer_2_space,sizeof buffer_2_space);
buffer *buffer_2 = &it;

