generate-bin-entries () 
{ 
    bins=$(ls -d build/cmake/* |filter-test -f |grep -v '[[:upper:].]');
    for x in $bins;
    do
        srcs=$(readelf -a "$x" |grep '\.c'|awkp 8|grep '\.c');
        test -z "$srcs" && continue;
        X=$(echo "$srcs"|grep-e-expr );
        l=$(grep -E "^(lib/|)$X\$" files.list);
        SOURCES=$(grep -v / <<<"$l"; grep / <<<"$l"|sort -f -u );
        echo "add_executable(" ${x##*/} $SOURCES ")";
    done
}
