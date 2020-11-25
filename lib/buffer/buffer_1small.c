#include "../windoze.h"
#include "../io_internal.h"

#if !WINDOWS_NATIVE
#include <unistd.h>
#endif

#include "../buffer.h"

char buffer_1small_space[128];
static buffer buffer_1small_it =
    BUFFER_INIT(write, 1, buffer_1small_space, sizeof buffer_1small_space);
buffer* buffer_1small = &buffer_1small_it;
