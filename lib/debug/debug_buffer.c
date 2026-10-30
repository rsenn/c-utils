#include "../debug.h"
#include "../io_internal.h"
#include "../windoze.h"

#if !WINDOWS_NATIVE
#include <unistd.h>
#endif

static char debug_space[BUFFER_INSIZE];

buffer debug_buffer = BUFFER_INIT(write, 2, debug_space, sizeof debug_space);
buffer* debug_buf = &debug_buffer;
