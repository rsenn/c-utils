#include <unistd.h>
#include "buffer.h"
#ifdef __MINGW32__
#include <io.h>
#endif

char buffer_2_space[BUFFER_INSIZE];
static buffer it = BUFFER_INIT((ssize_t(*)())write,2,buffer_2_space,sizeof buffer_2_space);
buffer *buffer_2 = &it;

