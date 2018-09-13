#!/bin/sh
: ${SUFFIX=.o}
: ${PIPE="tee Makefile.deps"}

[ $# -le 0 ] && set -- $(find . -name "*.c" -and -not -wholename "*build/*" )

OUTSTR='\$(BUILDDIR)$(basename "$x" .c)${SUFFIX}: ${y:-${x}}\n\t\$(CROSS_COMPILE)\$(CC) \$(CFLAGS) -c -o \$@ \$<'

if [ "$ADD" ]; then
  OUTSTR='\$(BUILDDIR)$(basename "$x" .c)${SUFFIX}: '$ADD'\n'$OUTSTR
fi

CMD='echo -e "'$OUTSTR'"'
CMD='y=$(gcc -MM -I. "$x" | sed ":lp; \\|\\\\$| { N; s|\\\\\\n\\s*||; b lp }; s|.*: ||; s|/[^.]\+/\.\.||g"); '$CMD
CMD='for x ; do '$CMD'; done'
CMD="$CMD${PIPE:+ | $PIPE}"


eval "$CMD"
