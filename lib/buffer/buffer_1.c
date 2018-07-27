#if !(defined(_WIN32) || defined(_WIN64))
#include <unistd.h>
#endif

#include "../io_internal.h"
#include "../buffer.h"
#if defined(_WIN32) || defined(_WIN64)
#include "../io_internal.h"
#endif

char buffer_1_space[BUFFER_INSIZE];
static buffer it = BUFFER_INIT(write, 1, buffer_1_space, sizeof buffer_1_space);
buffer* buffer_1 = &it;
