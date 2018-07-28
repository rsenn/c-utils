#!/bin/sh
: ${SUFFIX=.o}
: ${PIPE="tee Makefile.deps"}

[ $# -le 0 ] && set -- $(find . -name "*.c" -and -not -wholename "*build/*" | sed "s|^\\./||")

OUTSTR='\$(BUILDDIR)$(basename "$x" .c)${SUFFIX}: $x\n\t\$(CROSS_COMPILE)\$(CC) \$(CFLAGS) -c -o \$@ \$<'

[ "$ADD" ] && OUTSTR='\$(BUILDDIR)$(basename "$x" .c)${SUFFIX}: '$ADD'\n'$OUTSTR

CMD='echo -e "'$OUTSTR'"'
CMD='for x ; do '$CMD'; done'
CMD="$CMD${PIPE:+ | $PIPE}"

eval "$CMD"
