#include "../str.h"

#undef str_equal

unsigned int str_equal(const void *s, const void *t)
{
 return str_diff(s, t) == 0;
}
