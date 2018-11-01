#!/bin/sh
: ${SUFFIX=.o}
: ${PIPE="tee Makefile.deps"}
NL="
"
TAB="	"

[ $# -le 0 ] && set -- $(find . -name "*.c" -and -not -wholename "*build/*" ) || PIPE="tee -a Makefile.deps"

#OUTSTR='\$(BUILDDIR)$(basename "$x" .c)${SUFFIX}: ${y:-${x}}$NL$TAB\$(CROSS_COMPILE)\$(CC) \$(CFLAGS) \$(CPPFLAGS) -c -o \$@ \$<'
OUTSTR='\$(BUILDDIR)$(basename "$x" .c)${SUFFIX}: ${y:-${x#./}}$NL$TAB\$(CROSS_COMPILE)\$(CC) \$(CFLAGS) \$(CPPFLAGS) \$(INCLUDES) \$(DEFS) -c -o \$@ \$<'

if [ "$ADD" ]; then
  OUTSTR='\$(BUILDDIR)$(basename "$x" .c)${SUFFIX}: '$ADD'\n'$OUTSTR
fi

CMD='echo "'$OUTSTR'"'
#CMD='y=$(gcc -MM -I. "$x" | sed ":lp; \\|\\\\$| { N; s|\\\\\\n\\s*||; b lp }; s|.*: ||; s|/[^.]\+/\.\.||g"); '$CMD
CMD='y=; '$CMD
CMD='for x ; do '$CMD'; done'
CMD="$CMD${PIPE:+ | $PIPE}"


eval "$CMD"
