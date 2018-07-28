#!/bin/sh
for x in ${@:-$(find . -name "*.c" -and -not -wholename "*build/*" | sed "s|^\\./||")} ; do 
  echo -e "\$(BUILDDIR)$(basename "$x" .c).o: $x
\t\$(CROSS_COMPILE)\$(CC) \$(CFLAGS) -c -o \$@ \$<"
done   |tee Makefile.deps
