makefile-gen-bins () 
{ 
    for x in $bins;
    do
        srcs=$(readelf -a "$x" |grep '\.c'|awkp 8|grep '\.c');
        test -z "$srcs" && continue;
        X=$(echo "$srcs"|grep-e-expr );
        l=$(grep -E "^(lib/|)$X\$" files.list);
        SOURCES=$(grep -v / <<<"$l"; grep / <<<"$l"|sort -f -u );
        ARCHIVES=$(echo "$SOURCES" | sed 's,_.*\.c,.a, ; s,.*/,,'|uniq |addprefix build/cmake/ |sort -fu |filter-test -e | cut-dirname);
        [ -n "$ARCHIVES" ] && a=$(addprefix "\$(BUILDDIR)" $ARCHIVES) || a=;
        echo "\$(BUILDDIR)${x##*/}\$(M64_)\$(EXESUFFIX)\$(EXEEXT): \$(BUILDDIR)${x##*/}.o" $a;
    done
}
