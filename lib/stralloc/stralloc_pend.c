/*
 * reimplementation of Daniel Bernstein's unix library.
 * placed in the public domain by Uwe Ohse, uwe@ohse.de.
 */
#include "../stralloc.h"
#include "../gen_alloci.h"

int
stralloc_append(stralloc *s, const char *t)
{
	return gen_alloc_append(&s->s,sizeof(*t), &s->len, &s->a, t);
}
