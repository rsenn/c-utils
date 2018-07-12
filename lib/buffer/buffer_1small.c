#if !(defined(_WIN32) || defined(_WIN64))
#include <unistd.h>
#else
#include "../io_internal.h"
#endif
#include "../io_internal.h"
#include "../buffer.h"

char buffer_1_space[128];
static buffer it = BUFFER_INIT(write, 1, buffer_1_space, sizeof buffer_1_space);
buffer* buffer_1small = &it;
