/* Compatibility shim: lib/iom.h (this repo) includes <mcfgthread/c11thread.h>,
 * but upstream mcfgthread (https://github.com/lhmouse/mcfgthread) ships its
 * C11 <threads.h>-compatible API as `mcfgthread/c11.h`, not `c11thread.h` --
 * that name has never existed upstream. Forward to the real header rather
 * than editing lib/iom.h's include. */
#include "c11.h"
