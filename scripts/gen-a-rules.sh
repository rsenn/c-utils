gen-a-rules () 
{ 
    [ $# -le 0 ] && set --  $( ls -d lib/*_*.c|sed -n 's,.*/,, ; s,_.*,,p'|uniq);
    for a 
    do
        c=$(ls -d lib/"$a"_*.c);
        o=`echo "$c" |sed 's,.*/,$(BUILDDIR), ; s,\.c$,.o,'  `
        echo -e "\$(BUILDDIR)$a.a:" $o '\n\t$(CROSS_COMPILE)$(AR) rcs $@ $^\n';
    done
}
