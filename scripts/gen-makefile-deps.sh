#!/bin/sh
for x in $(find * -name "*.c") ; do 
  echo -e "\$(BUILDDIR)$(basename "$x" .c).o: $x
\t\$(CROSS_COMPILE)\$(CC) \$(CPPFLAGS) \$(CFLAGS) -c -o \$@ \$<"
done   |tee Makefile.deps
