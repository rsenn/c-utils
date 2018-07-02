#if !(defined(_WIN32) || defined(_WIN64))
#include <unistd.h>
#else
#include <io.h>
#endif
#include "../buffer.h"

char buffer_2_space[BUFFER_INSIZE];
static buffer it = BUFFER_INIT(write, 2, buffer_2_space, sizeof buffer_2_space);
buffer* buffer_2 = &it;
