#!/bin/sh

{
nm -A --defined build/*/*/*.a >defined.tmp
nm -A --undefined build/*/*/*.a >undefined.tmp
} 2>/dev/null

IMP_SYMS=$(sed '/ U / s,.* U ,,p' -n  undefined.tmp | sort -u)

LIBS=$(cut -d: -f1 undefined.tmp |sed "s|.*/|| ;; s|\.a\$||" |sort -u)

echo $LIBS

for LIB in $LIBS; do 
  IMPORTS=$(set -- $(grep "/$LIB\.a:" undefined.tmp | sed -n "s|.* U ||p" |sort -u) ; IFS="|" ; echo "$*") 

  DEPS=$( grep -E " T ($IMPORTS)\$" defined.tmp|sed "s|:.*|| ;; s|.*/|| ;; /^$LIB\.a/d" |sort -u)
  echo $LIB: $DEPS
done
#for SYM in $IMP_SYMS; do 
#  echo "/ $SYM\$/ { s,^\([^:]*\):.* \([^ ]*\)$,\2: \1,; p }"
#done >f.sed


#sed -n -f f.sed defined.tmp

#cut -d: -f1 <undefined.tmp

