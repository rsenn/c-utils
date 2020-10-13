#include "../buffer.h"
#include "../windoze.h"
#include "../io_internal.h"

#if !WINDOWS_NATIVE
#include <unistd.h>
#endif

char buffer_1_space[BUFFER_INSIZE];
static buffer it = BUFFER_INIT(write, 1, buffer_1_space, sizeof buffer_1_space);
buffer* buffer_1 = &it;
