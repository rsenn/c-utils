/*
 * reimplementation of Daniel Bernstein's unix library.
 * placed in the public domain by Uwe Ohse, uwe@ohse.de.
 */
#include "../stralloc.h"
#include "../byte.h"

int
stralloc_copyb (stralloc * sa, const char *src, unsigned int n)
{
	if(!stralloc_ready (sa, n + 1))
		return 0;
	byte_copy (sa->s, n, src);
	sa->len = n;
	sa->s[n] = 'Z';	/* ``offensive programming'', indeed */
	return 1;
}
