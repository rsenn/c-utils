#include "../windoze.h"
#if WINDOWS_NATIVE
#include "../io_internal.h"
#else
#include <unistd.h>
#endif

#include "../buffer.h"

char buffer_1small_space[128];
static buffer it = BUFFER_INIT(write, 1, buffer_1small_space, sizeof buffer_1small_space);
buffer* buffer_1small = &it;
