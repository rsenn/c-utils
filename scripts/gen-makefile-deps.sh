#!/bin/sh
: ${SUFFIX=.o}
: ${DEPSMK=build/deps.mk}
: ${PIPE="tee $DEPSMK"}
NL="
"
TAB="	"

[ $# -le 0 ] && set -- $(find . -name "*.c" -and -not -wholename "*3rd*/*" -and -not -wholename "*build/*" ) || PIPE="tee -a $DEPSMK"

#OUTSTR='\$(BUILDDIR)$(basename "$x" .c)${SUFFIX}: ${y:-${x}}$NL$TAB\$(CROSS_COMPILE)\$(CC) \$(CFLAGS) \$(CPPFLAGS) -c -o \$@ \$<'
OUTSTR='\$(BUILDDIR)$(basename "$x" .c)${SUFFIX}: ${y:-${x#./}}$NL$TAB\$(CROSS_COMPILE)\$(CC) \$(CFLAGS) \$(EXTRA_CFLAGS) \$(CPPFLAGS) \$(INCLUDES) \$(DEFS) -c -o \$@ \$<'

if [ "$ADD" ]; then
  OUTSTR='\$(BUILDDIR)$(basename "$x" .c)${SUFFIX}: '$ADD'\n'$OUTSTR
fi

CMD='echo "'$OUTSTR'"'
CMD='y=$(gcc -MM -I. "$x" | sed ":lp; \\|\\\\$| { N; s|\\\\\\n\\s*||; b lp }; s|.*: ||; s|/[^.]\+/\.\.||g"); '$CMD
CMD='y=; '$CMD
CMD='for x ; do '$CMD'; done'
CMD="$CMD"'; ./scripts/get-includes.sh 3rdparty'

eval "$CMD${PIPE:+ | $PIPE}"
