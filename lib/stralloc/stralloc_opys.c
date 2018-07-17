/*
 * reimplementation of Daniel Bernstein's unix library.
 * placed in the public domain by Uwe Ohse, uwe@ohse.de.
 */
#include "../str.h"
#include "../stralloc.h"

int
stralloc_copys (stralloc * sa, const char *src)
{
	return stralloc_copyb (sa, src, str_len (src));
}
