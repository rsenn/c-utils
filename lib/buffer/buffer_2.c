#include "../buffer.h"
#include "../windoze.h"
#include "../io_internal.h"

#if !WINDOWS_NATIVE
#include <unistd.h>
#endif

char buffer_2_space[BUFFER_INSIZE];
static buffer buffer_2_it =
    BUFFER_INIT(write, 2, buffer_2_space, sizeof buffer_2_space);
buffer* buffer_2 = &buffer_2_it;
